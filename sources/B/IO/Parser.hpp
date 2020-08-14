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
T parse(Reader& rd, unsigned short base = 10)
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

	// Helpers
	auto parseDigit = [&] (char c) -> int {
		int d = 0;
		/**/ if (isdigit(c)) d = c - '0';
		else if (islower(c)) d = c - ('a' - 10);
		else if (isupper(c)) d = c - ('A' - 10);
		else return -1;
		return d >= base ? -1 : d;
	};

	// Parse base
	switch (base) {
	case 0: // Auto determine if 0
		if (!rd.peek('0'))
			base = 10;
		else {
			rd.ignore();
			if (tolower(rd.peek()) == 'x') {
				base = 16;
				rd.ignore();
			}
			else if (tolower(rd.peek()) == 'b') {
				base = 2;
				rd.ignore();
			}
			else {
				base = 8;
			}
		}
		break;
	case 2: [[fallthrough]];
	case 16:
		// Skip base prefixes if they're here
		if (rd.peek('0')) {
			rd.ignore();
			if (tolower(rd.peek()) == (base == 2 ? 'b' : 'x'))
				rd.ignore();
			else if (parseDigit(rd.peek()) < 0)
				return T();
		}
		break;
	default:
		break;
	}
	/// TODO: parse exponents
	// char exponentChar = base == 10 ? 'e' : 'p';

	// Compute cutoffs
	T cutoff = (positive ? NumericTraits<T>::max : NumericTraits<T>::lowest) / base;
	int maxAfterCutoff = 0;
	if constexpr (IsFloat<T>())
		maxAfterCutoff = fmod(positive ? NumericTraits<T>::max : NumericTraits<T>::lowest, base);
	else
		maxAfterCutoff = (positive ? NumericTraits<T>::max : NumericTraits<T>::lowest) % base;

	auto canAppendDigit = [&] (int digit) -> bool {
		if (positive ? (num < cutoff) : (num > cutoff)) // is below cutoff?
			return true;
		return num == cutoff && digit <= maxAfterCutoff;
	};

	// Parsing
	bool pastDecimal = false;
	int exponent = 0;
	for (;;) {
		if (!pastDecimal && rd.peek('.') && IsFloat<T>()) {
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

	// Treat decimal part, if applies
	if constexpr (IsFloat<T>()) {
		if (num == 0 || exponent <= NumericTraits<T>::minExponent)
			return positive ? 0.0 : -0.0;
		else if (exponent >= NumericTraits<T>::maxExponent)
			return positive ? __builtin_huge_val() : -__builtin_huge_val();

		if (exponent < 0) {
			for (int i = 0; i < -exponent; ++i)
				num /= base;
		}
		else if (exponent > 0) {
			for (int i = 0; i < exponent; ++i)
				num *= base;
		}
	}

	return num;
}

}