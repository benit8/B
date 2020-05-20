/*
** B, 2019
** Stack.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	template <typename T>
	class Stack;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/Containers/Sequence.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

template <typename T>
class Stack : public Sequence<T>
{
public:
	Stack()
	: Container()
	{}

	T &top() { return this->at(this->size() - 1); }
	const T &top() const { return this->at(this->size() - 1); }

	void push(const T &value)
	{
		this->reserve(this->size() + 1);
		new (this->slot(this->size())) T(value);
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
		this->slot(this->size() - 1)->~T();
		this->m_size--;
	}

private:
	friend std::ostream &operator <<(std::ostream &os, const Stack<T> &s)
	{
		os << "Stack<" << typeid(T).name() << ">(" << s.size() << "/" << s.capacity() << ") ";
		if (s.null())
			return os << "null";

		os << '{';
		for (usize i = 0; i < s.size(); ++i) {
			if (i > 0)
				os << ", ";
			os << *s.slot(i);
		}
		return os << '}';
	}
};

}