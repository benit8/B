/*
** B, 2020
** Memory.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "B/Traits.hpp"
#include "B/Types.hpp"

#include <cassert>
#include <cstring>
#include <utility>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

template<typename T>
class Memory
{
public:
	static void move(T *destination, T *source, usize count)
	{
		assert(destination);
		assert(source);

		if (destination == source)
			return;

		if constexpr (Traits<T>::isTrivial())
			::memmove(destination, source, count * sizeof(T));
		else {
			if (destination < source) {
				for (usize i = 0; i < count; ++i) {
					new (&destination[i]) T(std::move(source[i]));
					source[i].~T();
				}
			}
			else {
				for (usize i = count; i > 0; --i) {
					new (&destination[i - 1]) T(std::move(source[i - 1]));
					source[i - 1].~T();
				}
			}
		}
	}

	static void copy(T *destination, const T *source, usize count)
	{
		assert(destination);
		assert(source);

		if (destination == source)
			return;

		if constexpr (Traits<T>::isTrivial())
			::memmove(destination, source, count * sizeof(T));
		else {
			if (destination < source) {
				for (usize i = 0; i < count; ++i)
					new (&destination[i]) T(source[i]);
			}
			else {
				for (usize i = count; i > 0; --i)
					new (&destination[i - 1]) T(source[i - 1]);
			}
		}
	}

	static void set(T *destination, const T &value, usize count)
	{
		assert(destination);

		if constexpr (Traits<T>::isTrivial())
			::memset(destination, value, count * sizeof(T));
		else {
			for (usize i = 0; i < count; ++i)
				new (&destination[i]) T(value);
		}
	}

	static bool compare(const T *a, const T *b, usize count)
	{
		assert(a);
		assert(b);

		if (a == b)
			return true;

		if constexpr (Traits<T>::isTrivial())
			return ::memcmp(a, b, count * sizeof(T)) == 0;

		for (usize i = 0; i < count; ++i) {
			if (a[i] != b[i])
				return false;
		}
		return true;
	}

	static void destruct(T *data, usize count)
	{
		assert(data);

		if constexpr (Traits<T>::isTrivial())
			return;

		for (usize i = 0; i < count; ++i)
			data[i].~T();
	}
};

}