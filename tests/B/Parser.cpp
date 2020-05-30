/*
** B tests, 2020
** Parser.cpp
*/

#include "B/IO/Parser.hpp"
#include <criterion/criterion.h>

////////////////////////////////////////////////////////////////////////////////

using ft = B::Traits<double>;

#define EXPECT(A, B)  do { auto n = (A); cr_expect_eq(n, B, #A ": expected %d, got %d", B, n); } while (0)
#define EXPECTF(A, B) do { auto n = (A); cr_expect_float_eq(n, B, ft::epsilon(), #A ": expected %f, got %f", B, n); } while (0)

////////////////////////////////////////////////////////////////////////////////

Test(Parser, Empty)
{
	EXPECT(B::parse<int>(""), 0);
	EXPECT(B::parse<int>("	"), 0);
	EXPECT(B::parse<int>("  "), 0);
}

Test(Parser, Trash)
{
	EXPECT(B::parse<int>("Lorem ipsum"), 0);
	EXPECT(B::parse<int>("I have 2 apples"), 0);
	EXPECT(B::parse<int>("++--+5646"), 0);
}

Test(Parser, Whitespaces)
{
	EXPECT(B::parse<int>("	5"), 5);
	EXPECT(B::parse<int>("  14  "), 14);
}

Test(Parser, Sign)
{
	EXPECT(B::parse<int>("+0"), 0);
	EXPECT(B::parse<int>("-0"), 0);
}

Test(Parser, Base)
{
	EXPECT(B::parse<int>("0b00101011", 0), 43);
	EXPECT(B::parse<int>("00101011", 2), 43);
	EXPECT(B::parse<int>("00101011", 8), 33289);
	EXPECT(B::parse<int>("00101011", 10), 101011);
	EXPECT(B::parse<int>("00101011", 16), 1052689);

	EXPECT(B::parse<int>("0664", 0), 436);
	EXPECT(B::parse<int>("0664", 2), 0);
	EXPECT(B::parse<int>("0664", 8), 436);
	EXPECT(B::parse<int>("0664", 10), 664);
	EXPECT(B::parse<int>("0664", 12), 940);
	EXPECT(B::parse<int>("0664", 16), 1636);

	EXPECT(B::parse<int>("17395", 0), 17395);
	EXPECT(B::parse<int>("17395", 2), 1);
	EXPECT(B::parse<int>("17395", 8), 123);
	EXPECT(B::parse<int>("17395", 10), 17395);
	EXPECT(B::parse<int>("17395", 16), 95125);

	EXPECT(B::parse<int>("0x35a2bd", 0), 3515069);
	EXPECT(B::parse<int>("35a2bd", 2), 0);
	EXPECT(B::parse<int>("35a2bd", 8), 29);
	EXPECT(B::parse<int>("35a2bd", 10), 35);
	EXPECT(B::parse<int>("35a2bd", 12), 72323);
	EXPECT(B::parse<int>("35a2bd", 16), 3515069);
}

Test(Parser, Integers)
{
	EXPECT(B::parse<int>("0"), 0);
	EXPECT(B::parse<int>("0.123"), 0);
	EXPECT(B::parse<int>("123"), 123);
	EXPECT(B::parse<int>("-456"), -456);
}

Test(Parser, Floats)
{
	EXPECTF(B::parse<double>("0"), 0);
	EXPECTF(B::parse<double>("123"), 123);
	EXPECTF(B::parse<double>("0.123"), 0.123);
	EXPECTF(B::parse<double>("-456.12"), -456.12);
	EXPECTF(B::parse<double>("0.600000"), 0.600000);
	EXPECTF(B::parse<double>("-0.720000"), -0.720000);
}