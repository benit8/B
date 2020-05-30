/*
** B, 2020
** Parser.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "B/StringView.hpp"
#include "B/Traits.hpp"
#include "B/IO/Reader.hpp"

#include <cmath>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

template <typename T>
T parse(StringView str, unsigned short base = 10)
{
	BufferReader r(str);
	return parse<T>(r, base);
}

template <typename T>
T parse(Reader &rd, unsigned short base = 10)
{
	T num = 0;

	// Skip whitespaces
	for (; isspace(rd.peek()); rd.get());
	if (rd.eof())
		return 0;

	/// TODO: Infinity/NaN

	// Parse sign
	bool positive = true;
	if (rd.peek('-') || rd.peek('+')) {
		positive = rd.peek('+');
		rd.ignore();
	}

	// Parse base
	if (base == 0) {
		if (!rd.peek('0'))
			base = 10;
		else {
			rd.ignore();
			if (tolower(rd.peek()) != 'x')
				base = 8;
			else {
				base = 16;
				rd.ignore();
			}
		}
	}
	/// TODO: parse exponents
	// char exponentChar = base == 10 ? 'e' : 'p';

	// Compute cutoffs
	T cutoff = (positive ? Traits<T>::max() : Traits<T>::lowest()) / base;
	int maxAfterCutoff = 0;
	if constexpr (Traits<T>::isDecimal())
		maxAfterCutoff = fmod(positive ? Traits<T>::max() : Traits<T>::lowest(), base);
	else
		maxAfterCutoff = (positive ? Traits<T>::max() : Traits<T>::lowest()) % base;

	// Helpers
	auto parseDigit = [base] (char c) -> int {
		int d = 0;
		/**/ if (isdigit(c)) d = c - '0';
		else if (islower(c)) d = c - ('a' - 10);
		else if (isupper(c)) d = c - ('A' - 10);
		else return -1;
		return static_cast<T>(d) >= base ? -1 : d;
	};

	auto canAppendDigit = [&] (int digit) -> bool {
		if (positive ? (num < cutoff) : (num > cutoff)) // is below cutoff?
			return true;
		return num == cutoff && digit <= maxAfterCutoff;
	};

	bool pastDecimal = false;
	int exponent = 0;
	for (;;) {
		if (!pastDecimal && rd.peek('.') && Traits<T>::isDecimal()) {
			pastDecimal = true;
			rd.ignore();
			continue;
		}

		int digit = parseDigit(rd.get());
		if (digit < 0)
			break;

		if (canAppendDigit(digit)) {
			num = (num * base) + (positive ? digit : -digit);
			if (pastDecimal)
				exponent--;
		}

		// Continue to consume digits even if the number overflows
	}

	/// TODO: parse exponents
	// if (tolower(rd.peek()) == exponentChar) {
	// 	rd.ignore();
	// }

	if constexpr (Traits<T>::isDecimal()) {
		if (num == 0 || exponent <= Traits<T>::minExponent())
			return positive ? 0.0 : -0.0;
		else if (exponent >= Traits<T>::maxExponent())
			return positive ? __builtin_huge_val() : -__builtin_huge_val();

		T value = static_cast<T>(num);
		if (exponent < 0) {
			for (int i = 0; i < -exponent; ++i)
				value /= base;
		}
		else if (exponent > 0) {
			for (int i = 0; i < exponent; ++i)
				value *= base;
		}
		return value;
	}

	return static_cast<T>(num);
}

}