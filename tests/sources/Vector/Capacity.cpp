/*
** B tests, 2020
** Vector / Capacity.cpp
*/

#include "B/Containers/Vector.hpp"
#include <criterion/criterion.h>

using B::Vector;

////////////////////////////////////////////////////////////////////////////////

Test(Vector, Empty)
{
	Vector<uint> v1;
	cr_assert(v1.empty());

	Vector<uint> v2(3, 10);
	cr_assert_not(v2.empty());
}

Test(Vector, Contains)
{
	Vector<uint> v({1, 3, 5, 7, 9});

	cr_assert(v.contains(3));
	cr_assert_not(v.contains(2));
}

Test(Vector, Resize)
{
	Vector<uint> arr;
	Vector<uint> cpy(arr);

	// remains unchanged when requested size is the same as current size
	arr.resize(0);
	cr_assert_eq(arr.size(), cpy.size());
	cr_assert_eq(arr.capacity(), cpy.capacity());

	// expand size but not capacity
	arr.resize(3, 5);
	cr_assert_gt(arr.size(), cpy.size());
	cr_assert_geq(arr.capacity(), cpy.capacity());
	cr_assert_geq(arr.capacity(), Vector<uint>::minimumSize);

	// expand size and capacity
	arr.resize(10, 3);
	cr_assert_eq(arr.size(), 10);
	cr_assert_geq(arr.capacity(), 10);

	// reduce size
	arr.resize(2);
	cr_assert_eq(arr.size(), 2);
	cr_assert_geq(arr.capacity(), 10);
}

Test(Vector, Reserve)
{
	Vector<uint> v;

	v.reserve(20);
	cr_assert_geq(v.capacity(), 20);

	v.reserve(10); // does nothing
	cr_assert_geq(v.capacity(), 20);
}

Test(Vector, Shrink)
{
	Vector<uint> v(10, 100);

	v.shrink();
	cr_assert_eq(v.size(), 10);
	cr_assert_eq(v.capacity(), 10);
}