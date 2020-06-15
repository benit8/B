/*
** B, 2019
** Queue.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	template <typename T>
	class Queue;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/Memory.hpp"
#include "B/Containers/Sequence.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

template <typename T>
class Queue : public Sequence<T>
{
public:
	Queue()
	: Sequence<T>()
	{}

	T &front() { return this->at(m_start); }
	const T &front() const { return this->at(m_start); }
	T &back() { return this->at(m_start - this->size() - 1); }
	const T &back() const { return this->at(m_start - this->size() - 1); }

	void push(const T &value)
	{
		if (m_start + this->size() == this->capacity()) {
			if (m_start > 0) {
				Memory<T>::move(this->slot(0), this->slot(m_start), this->size());
				m_start = 0;
			}
			else { // m_start is 0
				this->reserve(this->size() + 1);
			}
		}

		new (this->slot(m_start + this->size())) T(value);
		this->m_size++;
	}

	void push(T &&value)
	{
		T t(std::move(value));
		push(t);
	}

	template <typename... Args>
	void emplace(Args&&... args)
	{
		push(T(std::forward<Args>(args)...));
	}

	void pop()
	{
		if (this->empty())
			return;

		this->slot(m_start)->~T();
		m_start++;
		this->m_size--;

		if (this->empty())
			m_start = 0;
	}

	Queue &operator =(const Queue &rhs)
	{
		this->clear();
		this->reserve(rhs.size());
		Memory<T>::copy(this->slot(0), rhs.slot(0), rhs.size());
		this->m_size = rhs.m_size;
		return *this;
	}

	Queue &operator =(Queue &&rhs)
	{
		this->reset();
		this->m_data = std::move(rhs.m_data);
		this->m_size = std::move(rhs.m_size);
		this->m_capacity = std::move(rhs.m_capacity);
		this->m_start = std::move(rhs.m_start);
		rhs.m_data = nullptr;
		rhs.m_size = rhs.m_capacity = rhs.m_start = 0;
		return *this;
	}

private:
	/// TL;DR: Used to minimize the number of memmove operations.
	usize m_start = 0;
	// When the queue is pop'd, we don't immediatly move its remaining contents
	// to the left, we increase this index instead. It is taken into account during
	// other common operations.
	// When we cannot insert anymore into the queue without reallocation, and this
	// index isn't 0, we memmove the queue contents to the left. If it equals 0,
	// then there's no more space in the queue and we reallocate.

	friend std::ostream &operator <<(std::ostream &os, const Queue<T> &q)
	{
		os << "Queue<" << typeid(T).name() << ">(" << q.size() << '/' << q.capacity() << ") ";
		if (q.null())
			return os << "null";

		os << '{';
		for (usize i = 0; i < q.m_start + q.size(); ++i) {
			if (i > 0)
				os << ", ";
			if (i < q.m_start)
				os << '-';
			else
				os << *q.slot(i);
		}
		return os << '}';
	}
};

}