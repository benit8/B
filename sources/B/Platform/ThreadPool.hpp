/*
** B, 2019
** ThreadPool.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	class ThreadPool;
}

////////////////////////////////////////////////////////////////////////////////

#include <atomic>
#include <exception>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

namespace detail
{

template <typename T>
class AtomicQueue
{
public:
	bool push(T const &value) {
		std::unique_lock<std::mutex> lock(m_mutex);
		m_q.push(value);
		return true;
	}

	// Deletes the retrieved element, do not use for non integral types
	bool pop(T &v) {
		std::unique_lock<std::mutex> lock(m_mutex);
		if (m_q.empty())
			return false;
		v = m_q.front();
		m_q.pop();
		return true;
	}

	bool empty() {
		std::unique_lock<std::mutex> lock(m_mutex);
		return m_q.empty();
	}

private:
	std::queue<T> m_q;
	std::mutex m_mutex;
};

}

////////////////////////////////////////////////////////////////////////////////

class ThreadPool
{
public:
	ThreadPool()
	: m_nWaiting(0)
	, m_done(false)
	, m_stopped(false)
	{}

	ThreadPool(int nThreads)
	: ThreadPool()
	{
		resize(nThreads);
	}

	// The destructor waits for all the functions in the queue to be finished
	~ThreadPool()
	{
		stop(true);
	}

	////////////////////////////////////////////////////////////////////////

	// Number of running threads
	usize size() const {
		return m_threads.size();
	}

	// Number of idling threads
	usize idleCount() const {
		return m_nWaiting;
	}

	std::thread &getThread(usize i) const {
		return *m_threads[i];
	}

	// Change the number of threads in the pool
	// Should be called from one thread, otherwise be careful to not interleave, also with stop()
	// nThreads must be >= 0
	void resize(usize newSize)
	{
		if (m_stopped || m_done)
			return;

		usize size = m_threads.size();
		if (newSize == size)
			return;

		if (newSize > size) {
			m_threads.resize(newSize);
			m_flags.resize(newSize);

			for (usize i = size; i < newSize; ++i) {
				m_flags[i] = std::make_shared<std::atomic<bool>>(false);
				setThread(i);
			}
		}
		else {
			for (usize i = size - 1; i >= newSize; --i) {
				*m_flags[i] = true;  // This thread will finish
				m_threads[i]->detach();
			}
			{
				// Stop the detached threads that were waiting
				std::unique_lock<std::mutex> lock(m_mutex);
				m_cv.notify_all();
			}
			m_threads.resize(newSize); // Safe to delete because the threads are detached
			m_flags.resize(newSize); // Safe to delete because the threads have copies of shared_ptr of the flags, not originals
		}
	}

	// Empty the queue
	void clearQueue()
	{
		std::function<void(int id)> *_f;
		while (m_q.pop(_f))
			delete _f;
	}

	// Pops a functional wrapper to the original function
	std::function<void(int)> pop()
	{
		std::function<void(int id)> *_f = nullptr;
		m_q.pop(_f);
		std::unique_ptr<std::function<void(int id)>> func(_f); // At return, delete the function even if an exception occurred
		std::function<void(int)> f;
		if (_f)
			f = *_f;
		return f;
	}

	// Wait for all computing threads to finish and stop all threads
	// May be called asynchronously to not pause the calling thread while waiting
	// If wait == true, all the functions in the queue are run, otherwise the queue is cleared without running the functions
	void stop(bool wait = false)
	{
		if (!wait) {
			if (m_stopped)
				return;
			// Command the threads to stop
			for (usize i = 0, n = size(); i < n; ++i)
				*m_flags[i] = true;
			clearQueue();
			m_stopped = true;
		}
		else {
			if (m_done || m_stopped)
				return;
			// Give the waiting threads a command to finish
			m_done = true;
		}

		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cv.notify_all();  // Stop all waiting threads
		}

		// Wait for the computing threads to finish
		for (usize i = 0; i < m_threads.size(); ++i) {
			if (m_threads[i]->joinable())
				m_threads[i]->join();
		}

		// If there were no threads in the pool but some functors in the queue,
		// the functors are not deleted by the threads therefore delete them here
		clearQueue();
		m_threads.clear();
		m_flags.clear();
	}

	template<typename F, typename... Rest>
	auto push(F &&f, Rest&&... rest) -> std::future<decltype(f(0, rest...))>
	{
		auto pck = std::make_shared<std::packaged_task<decltype(f(0, rest...))(int)>>(
			std::bind(std::forward<F>(f), std::placeholders::_1, std::forward<Rest>(rest)...)
		);
		auto _f = new std::function<void(int id)>([pck](int id) {
			(*pck)(id);
		});
		m_q.push(_f);
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.notify_one();
		return pck->get_future();
	}

	// Run the user's function that expects argument `int id` of the running thread. returned value is templatized
	// Operator returns std::future, where the user can get the result and rethrow the catched exceptions
	template<typename F>
	auto push(F &&f) -> std::future<decltype(f(0))>
	{
		auto pck = std::make_shared<std::packaged_task<decltype(f(0))(int)>>(std::forward<F>(f));
		auto _f = new std::function<void(int id)>([pck](int id) {
			(*pck)(id);
		});
		m_q.push(_f);
		std::unique_lock<std::mutex> lock(m_mutex);
		m_cv.notify_one();
		return pck->get_future();
	}


private:
	ThreadPool(const ThreadPool &);
	ThreadPool(ThreadPool &&);
	ThreadPool & operator=(const ThreadPool &);
	ThreadPool & operator=(ThreadPool &&);

	void setThread(int i)
	{
		// A copy of the shared ptr to the flag
		std::shared_ptr<std::atomic<bool>> flag(m_flags[i]);

		auto f = [this, i, flag/* a copy of the shared ptr to the flag */]() {
			std::atomic<bool> &_flag = *flag;
			std::function<void(int id)> *_f;
			bool isPop = m_q.pop(_f);
			while (true) {
				// If there is anything in the queue
				while (isPop) {
					// At return, delete the function even if an exception occurred
					std::unique_ptr<std::function<void(int id)>> func(_f);
					(*_f)(i);
					if (_flag)
						return; // The thread is wanted to stop, return even if the queue is not empty yet
					else
						isPop = m_q.pop(_f);
				}
				// The queue is empty here, wait for the next command
				std::unique_lock<std::mutex> lock(m_mutex);
				m_nWaiting++;
				m_cv.wait(lock, [this, &_f, &isPop, &_flag]() {
					isPop = m_q.pop(_f);
					return isPop || m_done || _flag;
				});
				m_nWaiting--;
				if (!isPop)
					return; // If the queue is empty and isDone == true or *flag then return
			}
		};
		m_threads[i].reset(new std::thread(f));
	}

private:
	std::vector<std::unique_ptr<std::thread>> m_threads;
	std::vector<std::shared_ptr<std::atomic<bool>>> m_flags;
	detail::AtomicQueue<std::function<void(int id)> *> m_q;
	std::atomic<usize> m_nWaiting;
	std::atomic<bool> m_done;
	std::atomic<bool> m_stopped;

	std::mutex m_mutex;
	std::condition_variable m_cv;
};

}