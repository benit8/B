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

#include "B/Sequence.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

template <typename T>
class Stack : public Sequence<T>
{
public:
	Stack();

	T &top();
	const T &top() const;

	void push(const T &value);
	void push(T &&value);
	template <typename... Args>
	void emplace(Args&&... args);
	void pop();

private:
	friend std::ostream &operator <<<>(std::ostream&, const Stack<T>&);
};

}

////////////////////////////////////////////////////////////////////////////////

#include "B/Stack.inl"