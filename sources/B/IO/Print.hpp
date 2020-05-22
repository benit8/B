/*
** B, 2019
** Print.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "B/Types.hpp"

#include <cstring>
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

namespace detail
{

// Forward declaration
template <typename... Args>
void parseFormat(std::ostream&, const char *&, const Args&...);


// Format specifiers flags
static const struct flagEntry
{
	const char *letters;
	std::ios_base &(*flag)(std::ios_base&);
}
flagTable[] = {
	{"XOE", std::uppercase},
	{"XxOo", std::showbase},
	{"Xx", std::hex},
	{"Oo", std::oct},
	{"Ee", std::scientific}
};

static const size_t flagTableSize = sizeof(flagTable) / sizeof(flagTable[0]);


// Used before each format specifier
template <typename...>
void resetStreamFlags(std::ostream &os)
{
	os << std::boolalpha
	   << std::noshowbase
	   << std::noshowpos
	   << std::nouppercase
	   << std::right
	   << std::fixed
	   << std::dec;
	os.width(0);
	os.fill(' ');
	os.precision(6);
}

// Used to parse integer located in the format options
template <typename...>
int scanForInteger(const char *&fmt)
{
	int i = 0;
	for (; *fmt >= '0' && *fmt <= '9'; fmt++)
		i = (i * 10) + (*fmt - '0');
	fmt--;
	return i;
}

// Compiler requires this, but it's not actually used
template <typename...>
void printOne(std::ostream &, const char *&) {
	assert(false && "You probably didn't give enough arguments to B::print");
}


// Pop the top value of `args` into `var`, print it and recurse into parsing the format
template <typename T, typename... Args>
void printOne(std::ostream &os, const char *&fmt, const T &var, const Args&... args)
{
	os << var;
	parseFormat(os, ++fmt, args...);
}

template <typename... Args>
bool printFormatted(std::ostream &os, const char *&fmt, const Args&... args)
{
	switch (*fmt) {
		case '%': // Literal percent character
			printOne(os, fmt, '%', args...);
			break;
		case 'm': // C errno description
			printOne(os, fmt, std::strerror(errno), args...);
			break;
		case 'X': case 'x':
		case 'O': case 'o':
		case 'E': case 'e':
		case '$': // Any value
			printOne(os, fmt, args...);
			break;
		default: { // Print back the format specifier if it's invalid
			intptr_t pos = 0;
			for (; *(fmt - pos) != '%'; pos++);
			os.write(fmt - pos, pos + 1);
		}	return false;
	}

	return true;
}

template <typename...>
void parseFormatFlags(std::ostream &os, const char *&fmt)
{
	while (*(++fmt)) {
		if (*fmt == '-')
			os << std::left;
		else if (*fmt == '+')
			os << std::showpos;
		else if (*fmt == '0')
			os.fill('0');
		else if (*fmt >= '1' && *fmt <= '9')
			os.width(scanForInteger(fmt));
		else if (*fmt == '.')
			os.precision(scanForInteger(++fmt));
		else
			break;
	}

	for (usize i = 0; i < flagTableSize; ++i) {
		if (strchr(flagTable[i].letters, *fmt) != NULL)
			os << flagTable[i].flag;
	}
}

template <typename... Args>
void parseFormat(std::ostream &os, const char *&fmt, const Args&... args)
{
	for (; *fmt != '\0'; fmt++) {
		if (*fmt == '%' && *(fmt + 1)) {
			resetStreamFlags(os);
			parseFormatFlags(os, fmt);
			if (printFormatted(os, fmt, args...))
				break; // Break because the function above recurses back into this one
		}
		else {
			os << *fmt;
		}
	}
}

}

////////////////////////////////////////////////////////////////////////////////

/// Print to an std::ostream
template <typename... Args>
void print(std::ostream &os, const char *formatString, const Args&... args)
{
	const char *fmt = formatString;
	detail::parseFormat(os, fmt, args...);
}

/// Print to the standard output
template <typename... Args>
void print(const char *formatString, const Args&... args)
{
	print(std::cout, formatString, args...);
}

/// Print to the standard error output
template <typename... Args>
void eprint(const char *formatString, const Args&... args)
{
	print(std::cerr, formatString, args...);
}

/// Outputs to a string
template <typename... Args>
std::string format(const char *formatString, const Args&... args)
{
	std::ostringstream oss;
	print(oss, formatString, args...);
	return oss.str();
}

////////////////////////////////////////////////////////////////////////////////

}