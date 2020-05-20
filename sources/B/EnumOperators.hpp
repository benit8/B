/*
** B, 2020
** EnumOperators.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include <type_traits>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

template<typename E>
struct EnableEnumOperators
{
	static const bool enable = false;
};

#define ENABLE_ENUM_OPERATORS(T) \
template<> \
struct EnableEnumOperators<T> \
{ \
	static const bool enable = true; \
}

////////////////////////////////////////////////////////////////////////////////

template<typename E>
typename std::enable_if<EnableEnumOperators<E>::enable, E>::type operator |(E lhs, E rhs)
{
	using U = typename std::underlying_type<E>::type;
	return static_cast<E>(static_cast<U>(lhs) | static_cast<U>(rhs));
}

template<typename E>
typename std::enable_if<EnableEnumOperators<E>::enable, E>::type operator &(E lhs, E rhs)
{
	using U = typename std::underlying_type<E>::type;
	return static_cast<E>(static_cast<U>(lhs) & static_cast<U>(rhs));
}

template<typename E>
typename std::enable_if<EnableEnumOperators<E>::enable, E>::type operator ^(E lhs, E rhs)
{
	using U = typename std::underlying_type<E>::type;
	return static_cast<E>(static_cast<U>(lhs) ^ static_cast<U>(rhs));
}

template<typename E>
typename std::enable_if<EnableEnumOperators<E>::enable, E>::type operator ~(E rhs)
{
	using U = typename std::underlying_type<E>::type;
	return static_cast<E>(~static_cast<U>(rhs));
}

template<typename E>
typename std::enable_if<EnableEnumOperators<E>::enable, E>::type &operator |=(E &lhs, E rhs)
{
	using U = typename std::underlying_type<E>::type;
	return lhs = static_cast<E>(static_cast<U>(lhs) | static_cast<U>(rhs));
}

template<typename E>
typename std::enable_if<EnableEnumOperators<E>::enable, E>::type &operator &=(E &lhs, E rhs)
{
	using U = typename std::underlying_type<E>::type;
	return lhs = static_cast<E>(static_cast<U>(lhs) & static_cast<U>(rhs));
}

template<typename E>
typename std::enable_if<EnableEnumOperators<E>::enable, E>::type &operator ^=(E &lhs, E rhs)
{
	using U = typename std::underlying_type<E>::type;
	return lhs = static_cast<E>(static_cast<U>(lhs) ^ static_cast<U>(rhs));
}

////////////////////////////////////////////////////////////////////////////////

}