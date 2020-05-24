/*
** B tests, 2020
** Vector / assign.cpp
*/

#include "B/Containers/Vector.hpp"
#include <criterion/criterion.h>

using B::Vector;

////////////////////////////////////////////////////////////////////////////////

Test(Vector, EmptyConstructor)
{
	Vector<uint> v;

	cr_expect_eq(v.size(), 0);
	cr_expect_geq(v.capacity(), Vector<uint>::minimumSize);

	cr_assert_not_null(v.data());
}

Test(Vector, SizeConstructor_LessThanMinimumSize)
{
	Vector<uint> v(4);

	cr_expect_geq(v.capacity(), Vector<uint>::minimumSize);
	cr_expect_not_null(v.data());

	cr_assert_geq(v.capacity(), 4);
	cr_assert_eq(v.size(), 4);
}

Test(Vector, SizeConstructor_MoreThanMinimumSize)
{
	Vector<uint> v(14);

	cr_expect_geq(v.capacity(), Vector<uint>::minimumSize);
	cr_expect_not_null(v.data());

	cr_assert_geq(v.capacity(), 14);
	cr_assert_eq(v.size(), 14);
}

Test(Vector, FillConstructor)
{
	Vector<uint> v(4, 100);

	cr_expect_geq(v.capacity(), Vector<uint>::minimumSize);
	cr_expect_not_null(v.data());

	cr_assert_geq(v.capacity(), 4);
	cr_assert_eq(v.size(), 4);

	cr_assert((v[0] == v[1])
	       && (v[1] == v[2])
	       && (v[2] == v[3])
	       && (v[3] == 100));
}

Test(Vector, CopyConstructor)
{
	Vector<uint> v1(6, 200);
	Vector<uint> v2(v1);

	cr_assert_geq(v2.capacity(), v1.capacity());
	cr_assert_not_null(v2.data());
	cr_assert_geq(v2.size(), v1.size());

	cr_assert((v2[0] == v2[1])
	       && (v2[1] == v2[2])
	       && (v2[2] == v2[3])
	       && (v2[3] == v2[4])
	       && (v2[4] == v2[5])
	       && (v2[5] == 200));
}

Test(Vector, MoveConstructor)
{
	Vector<uint> v1(5, 300);
	Vector<uint> v2(std::move(v1));

	cr_assert_geq(v2.capacity(), Vector<uint>::minimumSize);
	cr_assert_geq(v2.capacity(), 5);
	cr_assert_not_null(v2.data());
	cr_assert_eq(v2.size(), 5);

	cr_assert((v2[0] == v2[1])
	       && (v2[1] == v2[2])
	       && (v2[2] == v2[3])
	       && (v2[3] == v2[4])
	       && (v2[4] == 300));

	cr_expect_eq(v1.capacity(), 0);
	cr_expect_eq(v1.size(), 0);
	cr_expect_null(v1.data());
}

Test(Vector, InitializerListConstructor)
{
	Vector<uint> v({7, 6, 5, 4, 3, 2, 1});

	cr_assert_geq(v.capacity(), Vector<uint>::minimumSize);
	cr_assert_geq(v.capacity(), 7);
	cr_assert_not_null(v.data());
	cr_assert_eq(v.size(), 7);
}

Test(Vector, Destructor)
{
	Vector<uint> v({1, 2, 3, 4, 5});
	v.~Vector<uint>();

	cr_assert_eq(v.size(), 0);
	cr_assert_eq(v.capacity(), 0);
	cr_assert_null(v.data());
}