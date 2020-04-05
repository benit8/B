/*
** B, 2020
** Traits.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct GenericTraits
{
	static constexpr bool isTrivial() { return false; }
	static bool equals(const T &a, const T &b) { return a == b; }
};

////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct Traits : GenericTraits<T>
{
};

////////////////////////////////////////////////////////////////////////////////

template<>
struct Traits<char> : public GenericTraits<char>
{
	static constexpr bool isTrivial() { return true; }
};

template<>
struct Traits<int> : public GenericTraits<int>
{
	static constexpr bool isTrivial() { return true; }
};

template<>
struct Traits<unsigned> : public GenericTraits<unsigned>
{
	static constexpr bool isTrivial() { return true; }
};

template<typename T>
struct Traits<T*> : public GenericTraits<T*>
{
	static constexpr bool isTrivial() { return true; }
	static bool equals(const T *a, const T *b) { return a == b; }
};

}