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
struct Traits<short> : public GenericTraits<short>
{
	static constexpr bool isTrivial() { return true; }
};

template<>
struct Traits<int> : public GenericTraits<int>
{
	static constexpr bool isTrivial() { return true; }
};

template<>
struct Traits<long> : public GenericTraits<long>
{
	static constexpr bool isTrivial() { return true; }
};

template<>
struct Traits<long long> : public GenericTraits<long long>
{
	static constexpr bool isTrivial() { return true; }
};

template<>
struct Traits<unsigned char> : public GenericTraits<unsigned char>
{
	static constexpr bool isTrivial() { return true; }
};

template<>
struct Traits<unsigned short> : public GenericTraits<unsigned short>
{
	static constexpr bool isTrivial() { return true; }
};

template<>
struct Traits<unsigned int> : public GenericTraits<unsigned int>
{
	static constexpr bool isTrivial() { return true; }
};

template<>
struct Traits<unsigned long> : public GenericTraits<unsigned long>
{
	static constexpr bool isTrivial() { return true; }
};

template<>
struct Traits<unsigned long long> : public GenericTraits<unsigned long long>
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