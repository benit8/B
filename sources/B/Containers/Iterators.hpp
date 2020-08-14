/*
** B, 2019
** Iterators.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	template <typename S, typename T>
	class SequenceIterator;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/Containers/Sequence.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

template <typename S, typename T>
class SequenceIterator
{
public:
	SequenceIterator& operator =(const SequenceIterator& other) { m_index = other.m_index; return *this; }
	SequenceIterator& operator ++() { ++m_index; return *this; }
	SequenceIterator& operator --() { --m_index; return *this; }
	SequenceIterator operator +(int value) { return SequenceIterator(m_sequence, m_index + value); }
	SequenceIterator operator -(int value) { return SequenceIterator(m_sequence, m_index - value); }
	int operator -(const SequenceIterator& other) { return m_index - other.m_index; }
	bool operator ==(SequenceIterator& other) const { return m_index == other.m_index; }
	bool operator !=(SequenceIterator& other) const { return m_index != other.m_index; }
	bool operator <(SequenceIterator& other) const { return m_index < other.m_index; }
	bool operator >(SequenceIterator& other) const { return m_index > other.m_index; }
	bool operator <=(SequenceIterator& other) const { return m_index <= other.m_index; }
	bool operator >=(SequenceIterator& other) const { return m_index >= other.m_index; }

	T& operator *()
	{
		return m_sequence[m_index];
	}

	T* operator ->()
	{
		return &m_sequence[m_index];
	}

	bool isEnd() const { return m_index >= m_sequence.size(); }
	usize index() const { return m_index; }

protected:
	SequenceIterator(S& sequence, usize index)
	: m_sequence(sequence)
	, m_index(index)
	{}

	S& m_sequence;
	usize m_index = 0;

	friend S;
};

}