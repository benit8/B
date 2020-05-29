/*
** B, 2019
** Types.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <sys/types.h>
#include <utility>

#include "Traits.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

using i8  = int8_t;
using u8  = uint8_t;
using i16 = int16_t;
using u16 = uint16_t;
using i32 = int32_t;
using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;

using isize = ssize_t;
using usize = size_t;

using f32  = float;
using f64  = double;
using f128 = long double;

using byte = u8;

////////////////////////////////////////////////////////////////////////////////

template<typename T>
class Type
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

////////////////////////////////////////////////////////////////////////////////

}