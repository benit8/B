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

#define ENABLE_ENUM_OPERATORS(T) \
template<> \
struct IsBitmaskEnum<T> \
{ \
	static constexpr bool enable = true; \
}

template<typename E>
struct IsBitmaskEnum
{
	static constexpr bool enable = false;
};

template <typename E>
inline constexpr bool IsBitmaskEnumV = IsBitmaskEnum<E>::enable;

////////////////////////////////////////////////////////////////////////////////

template<typename E>
typename std::enable_if_t<IsBitmaskEnumV<E>, E>
operator |(E lhs, E rhs)
{
	using U = typename std::underlying_type<E>::type;
	return static_cast<E>(static_cast<U>(lhs) | static_cast<U>(rhs));
}

template<typename E>
typename std::enable_if_t<IsBitmaskEnumV<E>, E>
operator &(E lhs, E rhs)
{
	using U = typename std::underlying_type<E>::type;
	return static_cast<E>(static_cast<U>(lhs) & static_cast<U>(rhs));
}

template<typename E>
typename std::enable_if_t<IsBitmaskEnumV<E>, E>
operator ^(E lhs, E rhs)
{
	using U = typename std::underlying_type<E>::type;
	return static_cast<E>(static_cast<U>(lhs) ^ static_cast<U>(rhs));
}

template<typename E>
typename std::enable_if_t<IsBitmaskEnumV<E>, E>
operator ~(E rhs)
{
	using U = typename std::underlying_type<E>::type;
	return static_cast<E>(~static_cast<U>(rhs));
}

template<typename E>
typename std::enable_if_t<IsBitmaskEnumV<E>, E&>
operator |=(E &lhs, E rhs)
{
	using U = typename std::underlying_type<E>::type;
	return lhs = static_cast<E>(static_cast<U>(lhs) | static_cast<U>(rhs));
}

template<typename E>
typename std::enable_if_t<IsBitmaskEnumV<E>, E&>
operator &=(E &lhs, E rhs)
{
	using U = typename std::underlying_type<E>::type;
	return lhs = static_cast<E>(static_cast<U>(lhs) & static_cast<U>(rhs));
}

template<typename E>
typename std::enable_if_t<IsBitmaskEnumV<E>, E&>
operator ^=(E &lhs, E rhs)
{
	using U = typename std::underlying_type<E>::type;
	return lhs = static_cast<E>(static_cast<U>(lhs) ^ static_cast<U>(rhs));
}

////////////////////////////////////////////////////////////////////////////////

template <typename E>
struct BitmaskEnum
{
	constexpr BitmaskEnum(const E &v)
	: value(v)
	{
		static_assert(IsBitmaskEnumV<E>);
	}

	static constexpr E null = static_cast<E>(0);

	inline constexpr bool any() const { return value != null; }
	inline constexpr bool none() const { return value == null; }
	inline constexpr bool anyOf(const E &mask) const { return (value & mask) != null; }
	inline constexpr bool allOf(const E &mask) const { return (value & mask) == mask; }
	inline constexpr bool noneOf(const E &mask) const { return (value & mask) == null; }
	inline constexpr bool anyExcept(const E &mask) const { return (value & ~mask) != null; }
	inline constexpr bool noneExcept(const E &mask) const { return (value & ~mask) == null; }

	inline constexpr operator E() const { return value; }
	inline constexpr operator bool() const { return any(); }

	const E value;
};

////////////////////////////////////////////////////////////////////////////////

}