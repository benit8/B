/*
** B, 2019
** Queue.inl
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

template <typename T>
Queue<T>::Queue()
: Container<T>()
{
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
T &Queue<T>::front()
{
	return *this->slot(m_start);
}

template <typename T>
T &Queue<T>::back()
{
	return *this->slot(m_start + this->size() - 1);
}

template <typename T>
const T &Queue<T>::front() const
{
	return *this->slot(m_start);
}

template <typename T>
const T &Queue<T>::back() const
{
	return *this->slot(m_start + this->size() - 1);
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
void Queue<T>::push(const T &value)
{
	if (m_start + this->size() == this->capacity()) {
		if (m_start > 0) {
			Type<T>::move(this->slot(0), this->slot(m_start), this->size());
			m_start = 0;
		}
		else { // m_start is 0
			this->reserve(this->size() + 1);
		}
	}

	new (this->slot(m_start + this->size())) T(value);
	this->m_size++;
}

template <typename T>
void Queue<T>::push(T &&value)
{
	T t(std::move(value));
	push(t);
}

template <typename T>
template <typename... Args>
void Queue<T>::emplace(Args&&... args)
{
	push(T(std::forward<Args>(args)...));
}

template <typename T>
void Queue<T>::pop()
{
	if (this->empty())
		return;

	this->slot(m_start)->~T();
	m_start++;
	this->m_size--;

	if (this->empty())
		m_start = 0;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
Queue<T> &Queue<T>::operator =(const Queue<T> &rhs)
{
	this->clear();
	this->reserve(rhs.size());
	Type<T>::copy(this->slot(0), rhs.slot(0), rhs.size());
	this->m_size = rhs.m_size;
	return *this;
}

template <typename T>
Queue<T> &Queue<T>::operator =(Queue<T> &&rhs)
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

////////////////////////////////////////////////////////////////////////////////

template <typename T>
std::ostream &operator <<(std::ostream &os, const Queue<T> &q)
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

////////////////////////////////////////////////////////////////////////////////

}