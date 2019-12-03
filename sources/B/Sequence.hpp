/*
** B, 2019
** Sequence.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	template <typename T>
	class Sequence;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/Container.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

template <typename T>
class Sequence : public Container<T>
{
public:
	virtual T &at(usize i);
	virtual const T &at(usize i) const;

	T &operator [](usize i) { return at(i); }
	const T &operator [](usize i) const { return at(i); }
};

////////////////////////////////////////////////////////////////////////////////

template <typename T>
T &Sequence<T>::at(usize i)
{
	if (i >= this->size())
		throw std::out_of_range(format("Tried to access out of range element: %$/%$", i, this->size()));
	return *this->slot(i);
}

template <typename T>
const T &Sequence<T>::at(usize i) const
{
	if (i >= this->size())
		throw std::out_of_range(format("Tried to access out of range element: %$/%$", i, this->size()));
	return *this->slot(i);
}

////////////////////////////////////////////////////////////

}