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

typedef int8_t   i8;
typedef uint8_t  u8;
typedef int16_t  i16;
typedef uint16_t u16;
typedef int32_t  i32;
typedef uint32_t u32;
typedef int64_t  i64;
typedef uint64_t u64;

typedef ssize_t isize;
typedef size_t  usize;

typedef float       f32;
typedef double      f64;
typedef long double f128;

typedef u8 byte;

////////////////////////////////////////////////////////////////////////////////

/// From SerenityOS' AK::TypedTransfer
template<typename T>
class Type
{
public:
	static void move(T *destination, T *source, size_t count)
	{
		assert(destination);
		assert(source);

		if (destination == source)
			return;

		if constexpr (Traits<T>::isTrivial())
			::memmove(destination, source, count * sizeof(T));
		else {
			if (destination < source) {
				for (size_t i = 0; i < count; ++i) {
					new (&destination[i]) T(std::move(source[i]));
					source[i].~T();
				}
			}
			else {
				for (size_t i = count; i > 0; --i) {
					new (&destination[i - 1]) T(std::move(source[i - 1]));
					source[i - 1].~T();
				}
			}
		}
	}

	static void copy(T *destination, const T *source, size_t count)
	{
		assert(destination);
		assert(source);

		if (destination == source)
			return;

		if constexpr (Traits<T>::isTrivial())
			::memmove(destination, source, count * sizeof(T));
		else {
			if (destination < source) {
				for (size_t i = 0; i < count; ++i)
					new (&destination[i]) T(source[i]);
			}
			else {
				for (size_t i = count; i > 0; --i)
					new (&destination[i - 1]) T(source[i - 1]);
			}
		}
	}

	static void set(T *destination, const T &value, size_t count)
	{
		assert(destination);
		if constexpr (Traits<T>::isTrivial())
			::memset(destination, value, count * sizeof(T));
		else {
			for (size_t i = 0; i < count; ++i)
				new (&destination[i]) T(value);
		}
	}

	static bool compare(const T *a, const T *b, size_t count)
	{
		assert(a);
		assert(b);

		if (a == b)
			return true;

		if constexpr (Traits<T>::isTrivial())
			return ::memcmp(a, b, count * sizeof(T)) == 0;

		for (size_t i = 0; i < count; ++i) {
			if (a[i] != b[i])
				return false;
		}
		return true;
	}
};

////////////////////////////////////////////////////////////////////////////////

}