/*
** B, 2019
** Common.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	template <typename T>
	class Type;
}

////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <cstring>
#include <fcntl.h>
#include <type_traits>
#include <utility>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

enum OpenMode
{
	NotOpen   = -1,
	ReadOnly  = O_RDONLY,  // 0
	WriteOnly = O_WRONLY,  // 1
	ReadWrite = O_RDWR,    // 2
	Binary    = 4,
	Create    = O_CREAT,   // 64
	ForceNew  = O_EXCL,    // 128
	Truncate  = O_TRUNC,   // 512
	Append    = O_APPEND,  // 1024
};

enum SeekMode
{
	Start   = SEEK_SET,
	Current = SEEK_CUR,
	End     = SEEK_END,
};

////////////////////////////////////////////////////////////////////////////////

inline OpenMode operator|(OpenMode lhs, OpenMode rhs)
{
	return OpenMode(static_cast<int>(lhs) | static_cast<int>(rhs));
}

inline OpenMode operator&(OpenMode lhs, OpenMode rhs)
{
	return OpenMode(static_cast<int>(lhs) & static_cast<int>(rhs));
}

////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline constexpr auto min(const T &a, const T &b)
{
	return (a < b) ? a : b;
}

template<typename T>
inline constexpr auto max(const T &a, const T &b)
{
	return (a > b) ? a : b;
}

template<typename T, typename... Ts>
inline constexpr auto min(const T &a, const T &b, const Ts&... z)
{
	return min(a, min(b, z...));
}

template<typename T, typename... Ts>
inline constexpr auto max(const T &a, const T &b, const Ts&... z)
{
	return max(a, max(b, z...));
}

template<typename T>
inline constexpr auto clamp(const T &value, const T &mini, const T &maxi)
{
	return max(mini, min(value, maxi));
}

////////////////////////////////////////////////////////////////////////////////

}