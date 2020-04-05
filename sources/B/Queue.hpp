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

#include "B/Sequence.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

template <typename T>
class Queue : public Sequence<T>
{
public:
	Queue();

	T &front();
	T &back();
	const T &front() const;
	const T &back() const;

	void push(const T &value);
	void push(T &&value);
	template <typename... Args>
	void emplace(Args&&... args);
	void pop();

	Queue &operator =(const Queue &rhs);
	Queue &operator =(Queue &&rhs);

private:
	/// TL;DR: Used to minimize the number of memmove operations.
	usize m_start = 0;
	// When the queue is pop'd, we don't immediatly move its remaining contents
	// to the left, we increase this index instead. It is taken into account during
	// other common operations.
	// When we cannot insert anymore into the queue without reallocation, and this
	// index isn't 0, we memmove the queue contents to the left. If it equals 0,
	// then there's no more space in the queue and we reallocate.

	friend std::ostream &operator <<<>(std::ostream&, const Queue<T>&);
};

}

////////////////////////////////////////////////////////////////////////////////

#include "B/Queue.inl"