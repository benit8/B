/*
** B, 2020
** Traits.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

/**
 * Summary:
 *
 * IntegralConstant<class T, T>
 *  => An integral constant value that can be accessed by invoke
 *     `T` could be `bool`, `int`, `char`...
 *
 * BoolConstant<bool>
 * TrueType, FalseType
 *  => Evaluates to `true` or `false` when invoked
 *
 * Conditional<bool C, TypeIfTrue, TypeIfFalse>(::Type)
 *  => Declares a member `Type` as `TypeIfTrue` if `C` is `true`, else `TypeIfFalse`
 *
 * Or<T, ...>, And<T, ...>, Not<T>
 *  => Logical helpers for composite types
 *
 * AddConst<T>(::Type)
 * RemoveConst<T>(::Type)
 * AddVolatile<T>(::Type)
 * RemoveVolatile<T>(::Type)
 * AddCV<T>(::Type)
 * RemoveCV<T>(::Type)
 *  => Adds/Removes `const` and/or `volatile` qualifiers from/to any type
 *
 * IsVoid<T>
 * IsPointer<T>
 * IsNull<T>
 * IsIntegral<T>
 * IsFloat<T>
 * IsArray<T>
 * IsEnum<T>
 * IsUnion<T>
 * IsClass<T>
 * IsFunction<T>
 * IsMemberPointer<T>, IsMemberObjectPointer<T>, IsMemberFunctionPointer<T>
 * IsLReference<T>, IsRReference<T>, IsReference<T>
 * IsArithmetic<T>
 * IsFundamental<T>
 * IsCompound<T>
 * IsScalar<T>
 * IsSigned<T>, IsUnsigned<T>
 *
 * EnableIf<bool, T>
 *
 */

////////////////////////////////////////////////////////////////////////////////

#include <cstddef>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

template<class T, T v>
struct IntegralConstant
{
	static constexpr T value = v;
	using Type = IntegralConstant;
	constexpr operator T() const noexcept { return value; }
	constexpr T operator()() const noexcept { return value; }
};

////////////////////////////////////////////////////////////////////////////////

template <bool v>
using BoolConstant = IntegralConstant<bool, v>;

typedef BoolConstant<true> TrueType;
typedef BoolConstant<false> FalseType;

////////////////////////////////////////////////////////////////////////////////

template <bool Cond, typename IfTrue, typename IfFalse>
struct Conditional { typedef IfTrue Type; };

template <typename IfTrue, typename IfFalse>
struct Conditional<false, IfTrue, IfFalse> { typedef IfFalse Type; };

////////////////////////////////////////////////////////////////////////////////

template <typename...>
struct Or;

template <>
struct Or<> : public FalseType {};

template <typename T>
struct Or<T> : public T {};

template <typename T1, typename T2>
struct Or<T1, T2> : public Conditional<T1::value, T1, T2>::Type {};

template <typename T1, typename T2, typename T3, typename... Tn>
struct Or<T1, T2, T3, Tn...> : public Conditional<T1::value, T1, Or<T2, T3, Tn...>>::Type {};

////////////////////////////////////////////////////////////////////////////////

template <typename...>
struct And;

template <>
struct And<> : public TrueType {};

template <typename T>
struct And<T> : public T {};

template <typename T1, typename T2>
struct And<T1, T2> : public Conditional<T1::Value, T1, T2>::Type {};

template <typename T1, typename T2, typename T3, typename... Tn>
struct And<T1, T2, T3, Tn...> : public Conditional<T1::value, T1, And<T2, T3, Tn...>>::Type {};

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct Not : public BoolConstant<!bool(T::value)> {};

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct AddConst { typedef T const Type; };
template <typename T>
struct RemoveConst { typedef T Type; };
template <typename T>
struct RemoveConst<T const> { typedef T Type; };

template <typename T>
struct AddVolatile { typedef T volatile Type; };
template <typename T>
struct RemoveVolatile { typedef T Type; };
template <typename T>
struct RemoveVolatile<T volatile> { typedef T Type; };

template <typename T>
struct AddCV { typedef typename AddConst<typename AddVolatile<T>::Type>::Type Type; };
template <typename T>
struct RemoveCV { typedef typename RemoveConst<typename RemoveVolatile<T>::Type>::Type Type; };

template <typename T>
struct AddPointer { typedef T* Type; };
template <typename T>
struct RemovePointer { typedef T Type; };
template <typename T> struct RemovePointer<T*> { typedef T Type; };
template <typename T> struct RemovePointer<T* const> { typedef T Type; };
template <typename T> struct RemovePointer<T* volatile> { typedef T Type; };
template <typename T> struct RemovePointer<T* const volatile> { typedef T Type; };

////////////////////////////////////////////////////////////////////////////////

namespace detail
{
	template <typename T>
	struct IsVoid : public FalseType {};

	template <> struct IsVoid<void> : public TrueType {};
}

template <typename T>
struct IsVoid : public detail::IsVoid<typename RemoveCV<T>::Type>::Type {};

////////////////////////////////////////////////////////////////////////////////

namespace detail
{
	template <typename T>
	struct IsPointer : public FalseType {};

	template <typename T>
	struct IsPointer<T*> : public TrueType {};
}

template <typename T>
struct IsPointer : public detail::IsPointer<typename RemoveCV<T>::Type>::Type {};

////////////////////////////////////////////////////////////////////////////////

namespace detail
{
	template <typename T>
	struct IsNull : public FalseType {};

	template <>
	struct IsNull<std::nullptr_t> : public TrueType {};
}

template <typename T>
struct IsNull : public detail::IsNull<typename RemoveCV<T>::Type>::Type {};

////////////////////////////////////////////////////////////////////////////////

namespace detail
{
	template <typename>
	struct IsIntegral : public FalseType {};

	template <> struct IsIntegral<bool>               : public TrueType {};
	template <> struct IsIntegral<char>               : public TrueType {};
	template <> struct IsIntegral<signed char>        : public TrueType {};
	template <> struct IsIntegral<unsigned char>      : public TrueType {};
	template <> struct IsIntegral<wchar_t>            : public TrueType {};
	template <> struct IsIntegral<char16_t>           : public TrueType {};
	template <> struct IsIntegral<char32_t>           : public TrueType {};
	template <> struct IsIntegral<short>              : public TrueType {};
	template <> struct IsIntegral<unsigned short>     : public TrueType {};
	template <> struct IsIntegral<int>                : public TrueType {};
	template <> struct IsIntegral<unsigned int>       : public TrueType {};
	template <> struct IsIntegral<long>               : public TrueType {};
	template <> struct IsIntegral<unsigned long>      : public TrueType {};
	template <> struct IsIntegral<long long>          : public TrueType {};
	template <> struct IsIntegral<unsigned long long> : public TrueType {};
}

template <typename T>
struct IsIntegral : public detail::IsIntegral<typename RemoveCV<T>::Type>::Type {};

////////////////////////////////////////////////////////////////////////////////

namespace detail
{
	template <typename>
	struct IsFloat : public FalseType {};

	template <> struct IsFloat<float>       : public TrueType {};
	template <> struct IsFloat<double>      : public TrueType {};
	template <> struct IsFloat<long double> : public TrueType {};
}

template <typename T>
struct IsFloat : public detail::IsFloat<typename RemoveCV<T>::Type>::Type {};

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct IsArray : public FalseType {};

template <typename T>
struct IsArray<T[]> : public TrueType {};

template <typename T, std::size_t S>
struct IsArray<T[S]> : public TrueType {};

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct IsEnum : public BoolConstant<__is_enum(T)> {};

template <typename T>
struct IsUnion : public BoolConstant<__is_union(T)> {};

template <typename T>
struct IsClass : public BoolConstant<__is_class(T)> {};

////////////////////////////////////////////////////////////////////////////////

template <typename>
struct IsFunction : public FalseType {};

#define MAKE_IS_FUNCTION_WITH_REFS(REFS)                           \
	template <typename Res, typename... Args>                      \
	struct IsFunction<Res(Args...) REFS> : public TrueType {};     \
	template <typename Res, typename... Args>                      \
	struct IsFunction<Res(Args..., ...) REFS> : public TrueType {};

#define MAKE_IS_FUNCTION_WITH_CV(CV) \
	MAKE_IS_FUNCTION_WITH_REFS(CV)   \
	MAKE_IS_FUNCTION_WITH_REFS(CV&) \
	MAKE_IS_FUNCTION_WITH_REFS(CV&&)

MAKE_IS_FUNCTION_WITH_CV()
MAKE_IS_FUNCTION_WITH_CV(const)
MAKE_IS_FUNCTION_WITH_CV(volatile)
MAKE_IS_FUNCTION_WITH_CV(const volatile)

#undef MAKE_IS_FUNCTION_WITH_CV
#undef MAKE_IS_FUNCTION_WITH_REFS

////////////////////////////////////////////////////////////////////////////////

namespace detail
{
	template <typename>
	struct IsMemberObjectPointer : public FalseType {};
	template <typename>
	struct IsMemberFunctionPointer : public FalseType {};

	template <typename T, typename C>
	struct IsMemberObjectPointer<T C::*> : public Not<IsFunction<T>>::Type {};
	template <typename T, typename C>
	struct IsMemberFunctionPointer<T C::*> : public IsFunction<T>::Type {};
}

template <typename T>
struct IsMemberObjectPointer : public detail::IsMemberObjectPointer<typename RemoveCV<T>::Type>::Type {};
template <typename T>
struct IsMemberFunctionPointer : public detail::IsMemberFunctionPointer<typename RemoveCV<T>::Type>::Type {};
template <typename T>
struct IsMemberPointer : public Or<IsMemberObjectPointer<T>, IsMemberFunctionPointer<T>>::Type {};

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct IsConst : public FalseType {};
template <typename T>
struct IsConst<T const> : public TrueType {};

template <typename T>
struct IsVolatile : public FalseType {};
template <typename T>
struct IsVolatile<T volatile> : public TrueType {};

////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct IsLReference : public FalseType {};
template <typename T>
struct IsLReference<T&> : public TrueType {};

template <typename T>
struct IsRReference : public FalseType {};
template <typename T>
struct IsRReference<T&&> : public TrueType {};

template <typename T>
struct IsReference : public Or<IsLReference<T>, IsRReference<T>>::Type {};

template <typename T>
struct IsArithmetic : public Or<IsIntegral<T>, IsFloat<T>>::Type {};

template <typename T>
struct IsFundamental : public Or<IsArithmetic<T>, IsVoid<T>, IsNull<T>>::Type {};

template <typename T>
struct IsCompound : public Not<IsFundamental<T>>::Type {};

template <typename T>
struct IsScalar : public Or<IsArithmetic<T>, IsEnum<T>, IsPointer<T>, IsMemberPointer<T>, IsNull<T>>::Type {};

////////////////////////////////////////////////////////////////////////////////

namespace detail
{
	template <typename T, bool = IsArithmetic<T>::value>
	struct IsSigned : public FalseType {};
	template <typename T>
	struct IsSigned<T, true> : public IntegralConstant<bool, T(-1) < T(0)> {};
}

template <typename T>
struct IsSigned : public detail::IsSigned<T>::Type {};

template <typename T>
struct IsUnsigned : public And<IsArithmetic<T>, Not<IsSigned<T>>> {};

////////////////////////////////////////////////////////////////////////////////

template <bool b, class T = void>
struct EnableIf {};

template <class T>
struct EnableIf<true, T> { typedef T Type; };

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template <typename T>
struct NumericTraits
{
	// static constexpr T min = T();
	// static constexpr T max = T();
	// static constexpr T lowest = T();
};

template<>
struct NumericTraits<bool>
{
	static constexpr bool min = false;
	static constexpr bool max = true;
	static constexpr bool lowest = min;
};

template<>
struct NumericTraits<char>
{
	static constexpr char min = -__SCHAR_MAX__ - 1;
	static constexpr char max = __SCHAR_MAX__;
	static constexpr char lowest = min;
};

template<>
struct NumericTraits<signed char>
{
	static constexpr signed char min = -__SCHAR_MAX__ - 1;
	static constexpr signed char max = __SCHAR_MAX__;
	static constexpr signed char lowest = min;
};

template<>
struct NumericTraits<short>
{
	static constexpr short min = -__SHRT_MAX__ - 1;
	static constexpr short max = __SHRT_MAX__;
	static constexpr short lowest = min;
};

template<>
struct NumericTraits<int>
{
	static constexpr int min = -__INT_MAX__ - 1;
	static constexpr int max = __INT_MAX__;
	static constexpr int lowest = min;
};

template<>
struct NumericTraits<long>
{
	static constexpr long min = -__LONG_MAX__ - 1;
	static constexpr long max = __LONG_MAX__;
	static constexpr long lowest = min;
};

template<>
struct NumericTraits<long long>
{
	static constexpr long long min = -__LONG_LONG_MAX__ - 1;
	static constexpr long long max = __LONG_LONG_MAX__;
	static constexpr long long lowest = min;
};

////////////////////////////////////////////////////////////

template<>
struct NumericTraits<unsigned char>
{
	static constexpr unsigned char min = 0;
	static constexpr unsigned char max = __SCHAR_MAX__ * 2u + 1;
	static constexpr unsigned char lowest = min;
};

template<>
struct NumericTraits<unsigned short>
{
	static constexpr unsigned short min = 0;
	static constexpr unsigned short max = __SHRT_MAX__ * 2u + 1;
	static constexpr unsigned short lowest = min;
};

template<>
struct NumericTraits<unsigned int>
{
	static constexpr unsigned min = 0;
	static constexpr unsigned max = __INT_MAX__ * 2u + 1;
	static constexpr unsigned lowest = min;
};

template<>
struct NumericTraits<unsigned long>
{
	static constexpr unsigned long min = 0;
	static constexpr unsigned long max = __LONG_MAX__ * 2ul + 1;
	static constexpr unsigned long lowest = min;
};

template<>
struct NumericTraits<unsigned long long>
{
	static constexpr unsigned long long min = 0;
	static constexpr unsigned long long max = __LONG_LONG_MAX__ * 2ull + 1;
	static constexpr unsigned long long lowest = min;
};

////////////////////////////////////////////////////////////

template<>
struct NumericTraits<float>
{
	static constexpr float min = __FLT_MIN__;
	static constexpr float max = __FLT_MAX__;
	static constexpr float lowest = -max;
	static constexpr int minExponent = __FLT_MIN_EXP__;
	static constexpr int maxExponent = __FLT_MAX_EXP__;
	static constexpr float epsilon = __FLT_EPSILON__;
};

template<>
struct NumericTraits<double>
{
	static constexpr double min = __DBL_MIN__;
	static constexpr double max = __DBL_MAX__;
	static constexpr double lowest = -max;
	static constexpr int minExponent = __DBL_MIN_EXP__;
	static constexpr int maxExponent = __DBL_MAX_EXP__;
	static constexpr double epsilon = __DBL_EPSILON__;
};

template<>
struct NumericTraits<long double>
{
	static constexpr long double min = __LDBL_MIN__;
	static constexpr long double max = __LDBL_MAX__;
	static constexpr long double lowest = -max;
	static constexpr int minExponent = __LDBL_MIN_EXP__;
	static constexpr int maxExponent = __LDBL_MAX_EXP__;
	static constexpr long double epsilon = __LDBL_EPSILON__;
};

////////////////////////////////////////////////////////////////////////////////

}