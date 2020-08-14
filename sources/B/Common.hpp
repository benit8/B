/*
** B, 2019
** Common.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "B/EnumOperators.hpp"

#include <fcntl.h>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

enum class OpenMode : int
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

	StandardFlags = O_RDONLY | O_WRONLY | O_RDWR | O_CREAT | O_EXCL | O_TRUNC | O_APPEND,
};
ENABLE_ENUM_OPERATORS(OpenMode);

enum class SeekMode : int
{
	Start   = SEEK_SET,
	Current = SEEK_CUR,
	End     = SEEK_END,
};

////////////////////////////////////////////////////////////////////////////////

template<typename T1, typename T2>
inline constexpr auto min(const T1& a, const T2& b)
{
	return (a < b) ? a : b;
}

template<typename T1, typename T2>
inline constexpr auto max(const T1& a, const T2& b)
{
	return (a > b) ? a : b;
}

template<typename T1, typename T2, typename... Ts>
inline constexpr auto min(const T1& a, const T2& b, const Ts&... z)
{
	return min(a, min(b, z...));
}

template<typename T1, typename T2, typename... Ts>
inline constexpr auto max(const T1& a, const T2& b, const Ts&... z)
{
	return max(a, max(b, z...));
}

template<typename T1, typename T2, typename T3>
inline constexpr auto clamp(const T1& value, const T2& mini, const T3& maxi)
{
	return max(mini, min(value, maxi));
}

////////////////////////////////////////////////////////////////////////////////

}