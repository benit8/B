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

template<typename T>
struct NumericTraits
{
	static constexpr bool isSigned() { return false; }
	static constexpr bool isDecimal() { return false; }
	static constexpr T min() { return T(); }
	static constexpr T max() { return T(); }
	static constexpr T lowest() { return T(); }
};

template <typename T>
struct Traits : public GenericTraits<T>, public NumericTraits<T>
{
};

////////////////////////////////////////////////////////////////////////////////

template<>
struct Traits<bool>
{
	static constexpr bool isTrivial() { return true; }
	static constexpr bool isSigned() { return false; }
	static constexpr bool isDecimal() { return false; }
	static constexpr bool min() { return false; }
	static constexpr bool max() { return true; }
	static constexpr bool lowest() { return min(); }
};

template<>
struct Traits<char>
{
	static constexpr bool isTrivial() { return true; }
	static constexpr bool isSigned() { return true; }
	static constexpr bool isDecimal() { return false; }
	static constexpr char min() { return -__SCHAR_MAX__ - 1; }
	static constexpr char max() { return __SCHAR_MAX__; }
	static constexpr char lowest() { return min(); }
};

template<>
struct Traits<short>
{
	static constexpr bool isTrivial() { return true; }
	static constexpr bool isSigned() { return true; }
	static constexpr bool isDecimal() { return false; }
	static constexpr short min() { return -__SHRT_MAX__ - 1; }
	static constexpr short max() { return __SHRT_MAX__; }
	static constexpr short lowest() { return min(); }
};

template<>
struct Traits<int>
{
	static constexpr bool isTrivial() { return true; }
	static constexpr bool isSigned() { return true; }
	static constexpr bool isDecimal() { return false; }
	static constexpr int min() { return -__INT_MAX__ - 1; }
	static constexpr int max() { return __INT_MAX__; }
	static constexpr int lowest() { return min(); }
};

template<>
struct Traits<long>
{
	static constexpr bool isTrivial() { return true; }
	static constexpr bool isSigned() { return true; }
	static constexpr bool isDecimal() { return false; }
	static constexpr long min() { return -__LONG_MAX__ - 1; }
	static constexpr long max() { return __LONG_MAX__; }
	static constexpr long lowest() { return min(); }
};

template<>
struct Traits<long long>
{
	static constexpr bool isTrivial() { return true; }
	static constexpr bool isSigned() { return true; }
	static constexpr bool isDecimal() { return false; }
	static constexpr long long min() { return -__LONG_LONG_MAX__ - 1; }
	static constexpr long long max() { return __LONG_LONG_MAX__; }
	static constexpr long long lowest() { return min(); }
};

////////////////////////////////////////////////////////////////////////////////

template<>
struct Traits<unsigned char>
{
	static constexpr bool isTrivial() { return true; }
	static constexpr bool isSigned() { return false; }
	static constexpr bool isDecimal() { return false; }
	static constexpr unsigned char min() { return 0; }
	static constexpr unsigned char max() { return __SCHAR_MAX__ * 2u + 1; }
	static constexpr unsigned char lowest() { return min(); }
};

template<>
struct Traits<unsigned short>
{
	static constexpr bool isTrivial() { return true; }
	static constexpr bool isSigned() { return false; }
	static constexpr bool isDecimal() { return false; }
	static constexpr unsigned short min() { return 0; }
	static constexpr unsigned short max() { return __SHRT_MAX__ * 2u + 1; }
	static constexpr unsigned short lowest() { return min(); }
};

template<>
struct Traits<unsigned int>
{
	static constexpr bool isTrivial() { return true; }
	static constexpr bool isSigned() { return false; }
	static constexpr bool isDecimal() { return false; }
	static constexpr unsigned min() { return 0; }
	static constexpr unsigned max() { return __INT_MAX__ * 2u + 1; }
	static constexpr unsigned lowest() { return min(); }
};

template<>
struct Traits<unsigned long>
{
	static constexpr bool isTrivial() { return true; }
	static constexpr bool isSigned() { return false; }
	static constexpr bool isDecimal() { return false; }
	static constexpr unsigned long min() { return 0; }
	static constexpr unsigned long max() { return __LONG_MAX__ * 2ul + 1; }
	static constexpr unsigned long lowest() { return min(); }
};

template<>
struct Traits<unsigned long long>
{
	static constexpr bool isTrivial() { return true; }
	static constexpr bool isSigned() { return false; }
	static constexpr bool isDecimal() { return false; }
	static constexpr unsigned long long min() { return 0; }
	static constexpr unsigned long long max() { return __LONG_LONG_MAX__ * 2ull + 1; }
	static constexpr unsigned long long lowest() { return min(); }
};

////////////////////////////////////////////////////////////////////////////////

template<>
struct Traits<float>
{
	static constexpr bool isTrivial() { return true; }
	static constexpr bool isSigned() { return true; }
	static constexpr bool isDecimal() { return true; }
	static constexpr float min() { return __FLT_MIN__; }
	static constexpr float max() { return __FLT_MAX__; }
	static constexpr float lowest() { return -max(); }
	static constexpr int minExponent() { return __FLT_MIN_EXP__; }
	static constexpr int maxExponent() { return __FLT_MAX_EXP__; }
	static constexpr float epsilon() { return __FLT_EPSILON__; }
};

template<>
struct Traits<double>
{
	static constexpr bool isTrivial() { return true; }
	static constexpr bool isSigned() { return true; }
	static constexpr bool isDecimal() { return true; }
	static constexpr double min() { return __DBL_MIN__; }
	static constexpr double max() { return __DBL_MAX__; }
	static constexpr double lowest() { return -max(); }
	static constexpr int minExponent() { return __DBL_MIN_EXP__; }
	static constexpr int maxExponent() { return __DBL_MAX_EXP__; }
	static constexpr double epsilon() { return __DBL_EPSILON__; }
};

template<>
struct Traits<long double>
{
	static constexpr bool isTrivial() { return true; }
	static constexpr bool isSigned() { return true; }
	static constexpr bool isDecimal() { return true; }
	static constexpr long double min() { return __LDBL_MIN__; }
	static constexpr long double max() { return __LDBL_MAX__; }
	static constexpr long double lowest() { return -max(); }
	static constexpr int minExponent() { return __LDBL_MIN_EXP__; }
	static constexpr int maxExponent() { return __LDBL_MAX_EXP__; }
	static constexpr long double epsilon() { return __LDBL_EPSILON__; }
};

////////////////////////////////////////////////////////////////////////////////

template<typename T>
struct Traits<T*>
{
	static constexpr bool isTrivial() { return true; }
	static bool equals(const T *a, const T *b) { return a == b; }
};

////////////////////////////////////////////////////////////////////////////////

}