/*
** B, 2019
** Array.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	template <typename T, usize S>
	struct Array;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/Iterators.hpp"
#include "B/Types.hpp"
#include "B/Vector.hpp"
#include "B/IO/Print.hpp"

#include <stdexcept>
#include <initializer_list>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

template <typename T, usize S>
struct Array
{
	T _data[S];

public:
	using Iterator = SequenceIterator<Array, T>;
	using ConstIterator = SequenceIterator<const Array, const T>;

public:
	Iterator begin() { return Iterator(*this, 0); }
	Iterator end() { return Iterator(*this, S); }
	ConstIterator begin() const { return ConstIterator(*this, 0); }
	ConstIterator end() const { return ConstIterator(*this, S); }

	constexpr usize size() const { return S; }
	constexpr bool empty() const { return S == 0; }

	T &front() { return data()[0]; }
	T &back() { return data()[S - 1]; }
	T *data() { return _data; }
	T &at(usize i)
	{
		if (i >= S)
			throw std::runtime_error(format("Array::at: i:%$ >= size:%$", i, S));
		return data()[i];
	}

	const T &front() const { return data()[0]; }
	const T &back() const { return data()[S - 1]; }
	const T *data() const { return _data; }
	const T &at(usize i) const
	{
		if (i >= S)
			throw std::runtime_error(format("Array::at: i:%$ >= size:%$", i, S));
		return data()[i];
	}

	void fill(const T &value)
	{
		for (usize i = 0; i < S; ++i)
			data()[i] = value;
	}

	T &operator [](usize i) { return data()[i]; }
	const T &operator [](usize i) const { return data()[i]; }

	Vector<T> toVector() const { return Vector<T>(data(), size()); }
};

}