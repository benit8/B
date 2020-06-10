/*
** B tests, 2020
** Vector / ElementAccess.cpp
*/

#include "B/Containers/Vector.hpp"
#include <criterion/criterion.h>

using B::Vector;

////////////////////////////////////////////////////////////////////////////////

Test(Vector, At_const)
{
	Vector<uint> v({7, 5, 6, 1, 6, 9, 4, 3});
	cr_assert_eq(v.at(0), 7);
	cr_assert_eq(v.at(1), 5);
	cr_assert_eq(v.at(2), 6);
	cr_assert_eq(v.at(3), 1);
	cr_assert_eq(v.at(4), 6);
	cr_assert_eq(v.at(5), 9);
	cr_assert_eq(v.at(6), 4);
	cr_assert_eq(v.at(7), 3);

	cr_assert_throw(v.at(8), std::out_of_range);
}

Test(Vector, At)
{
	Vector<uint> v(8);

	v.at(3) = 1;
	cr_assert_eq(v[3], 1);
}

Test(Vector, Front_const)
{
	Vector<uint> v({7, 5, 6, 1, 6, 9, 4, 3});

	cr_assert_eq(v.front(), 7);
	cr_assert_eq(v.front(), v.at(0));
}

Test(Vector, Front)
{
	Vector<uint> v({7, 5, 6, 1, 6, 9, 4, 3});

	v.front() = 10;
	cr_assert_eq(v.at(0), 10);
}

Test(Vector, Back_const)
{
	Vector<uint> v({7, 5, 6, 1, 6, 9, 4, 3});

	cr_assert_eq(v.back(), 3);
	cr_assert_eq(v.back(), v.at(v.size() - 1));
}

Test(Vector, Back)
{
	Vector<uint> v({7, 5, 6, 1, 6, 9, 4, 3});

	v.back() = 10;
	cr_assert_eq(v.at(v.size() - 1), 10);
}

Test(Vector, Data_const)
{
	Vector<uint> v({7, 5, 6, 1, 9, 4, 3});
	cr_assert_not_null((const uint *)v.data());

	v.~Vector<uint>();
	cr_assert_null((const uint *)v.data());
}

Test(Vector, Data)
{
	Vector<uint> v({7, 5, 6, 1, 9, 4, 3});

	uint *a = v.data();
	a[2] = 10;

	cr_assert_eq(v.at(2), 10);
}