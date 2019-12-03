/*
** B, 2019
** Traversable.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include <type_traits>
#include "B/Container.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

template <typename C, typename T>
class ContainerIterator
{
public:
	ContainerIterator &operator =(const ContainerIterator &other) { m_index = other.m_index; return *this; }
	ContainerIterator &operator ++() { ++m_index; return *this; }
	ContainerIterator &operator --() { --m_index; return *this; }
	ContainerIterator operator +(int value) { return ContainerIterator(m_container, m_index + value); }
	ContainerIterator operator -(int value) { return ContainerIterator(m_container, m_index - value); }
	int operator -(const ContainerIterator &other) { return m_index - other.m_index; }
	bool operator ==(ContainerIterator &other) const { return m_index == other.m_index; }
	bool operator !=(ContainerIterator &other) const { return m_index != other.m_index; }
	bool operator <(ContainerIterator &other) const { return m_index < other.m_index; }
	bool operator >(ContainerIterator &other) const { return m_index > other.m_index; }
	bool operator <=(ContainerIterator &other) const { return m_index <= other.m_index; }
	bool operator >=(ContainerIterator &other) const { return m_index >= other.m_index; }

	T &operator *()
	{
		if constexpr (std::is_base_of<Container<T>, C>::value)
			return *m_container.slot(m_index);
		else
			return m_container[m_index];
	}

	T *operator ->()
	{
		if constexpr (std::is_base_of<Container<T>, C>::value)
			return m_container.slot(m_index);
		else
			return &m_container[m_index];
	}

	bool isEnd() const { return m_index >= m_container.size(); }
	usize index() const { return m_index; }

protected:
	ContainerIterator(C &container, usize index)
	: m_container(container)
	, m_index(index)
	{}

	C &m_container;
	usize m_index = 0;

	friend C;
};

}