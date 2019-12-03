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

#include "B/Container.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

template <typename T>
class Stack : public Container<T>
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
	template <typename T_>
	friend std::ostream &operator <<(std::ostream&, const Stack<T_>&);
};

}

////////////////////////////////////////////////////////////////////////////////

#include "B/Stack.inl"