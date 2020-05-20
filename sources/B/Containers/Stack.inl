/*
** B, 2019
** Stack.inl
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

template <typename T>
Stack<T>::Stack()
: Container<T>()
{}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
T &Stack<T>::top()
{
	return *this->slot(this->size() - 1);
}

template <typename T>
const T &Stack<T>::top() const
{
	return *this->slot(this->size() - 1);
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
void Stack<T>::push(const T &value)
{
	this->reserve(this->size() + 1);
	new (this->slot(this->size())) T(value);
	this->m_size++;
}

template <typename T>
void Stack<T>::push(T &&value)
{
	T t(std::move(value));
	push(t);
}

template <typename T>
template <typename... Args>
void Stack<T>::emplace(Args&&... args)
{
	push(T(std::forward<Args>(args)...));
}

template <typename T>
void Stack<T>::pop()
{
	if (this->empty())
		return;
	this->slot(this->size() - 1)->~T();
	this->m_size--;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
std::ostream &operator <<(std::ostream &os, const Stack<T> &s)
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

////////////////////////////////////////////////////////////////////////////////

}