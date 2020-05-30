/*
** B tests, 2020
** Vector / modifiers.cpp
*/

#include "B/Containers/Vector.hpp"
#include <criterion/criterion.h>

using B::Vector;

////////////////////////////////////////////////////////////////////////////////

Test(Vector, AssignCopy)
{
	Vector<uint> v1({1, 5, 2, 9});
	Vector<uint> v2;

	v2.assign(v1);
	cr_assert_eq(v1, v2);
}

Test(Vector, AssignMove)
{
	Vector<uint> v1({1, 5, 2, 9});
	Vector<uint> v2;

	v2.assign(std::move(v1));
	cr_assert_eq(v2.size(), 4);
	cr_assert(v1.empty());
}

Test(Vector, AssignFill)
{
	Vector<uint> v({1, 5, 2, 9});

	v.assign(2, 10);
	cr_assert_eq(v.size(), 2);
	cr_assert_eq(v[0], 10);
	cr_assert_eq(v[0], v[1]);
}

Test(Vector, AssignInitializerList)
{
	Vector<uint> v(5, 10);

	v.assign({1, 5, 9, 7});
	cr_assert(v.size() == 4 &&
		v[0] == 1 &&
		v[1] == 5 &&
		v[2] == 9 &&
		v[3] == 7
	);
}

////////////////////////////////////////////////////////////////////////////////

Test(Vector, AppendElementCopy)
{
	Vector<uint> v;
	cr_assert(v.empty());

	v.append(uint(4));
	cr_assert(v.size() == 1);
	cr_assert(v[0] == 4);

	v.append(2, 7);
	cr_assert(v.size() == 8);
	cr_assert(v[0] == 4 && v[7] == 2);

	v.append(uint(8));
	cr_assert(v.size() == 9);
	cr_assert(v.capacity() >= 9);
	cr_assert(v[8] == 8);
}

Test(Vector, AppendElementMove)
{
	Vector<uint> v(3, 4);
	uint i = 5;
	v.append(std::move(i));

	cr_assert(v.size() == 4);
	cr_assert(v[3] == 5);
}

Test(Vector, AppendVectorCopy)
{
	Vector<uint> v1({4, 5, 6});
	Vector<uint> v2({1, 2, 3});

	v1.append(v2);

	cr_assert(v1 == Vector<uint>({4, 5, 6, 1, 2, 3}));
	cr_assert(v2 == Vector<uint>({1, 2, 3}));
}

Test(Vector, AppendVectorMove)
{
	Vector<uint> v1({4, 5, 6});
	Vector<uint> v2({1, 2, 3});

	v1.append(std::move(v2));

	cr_assert(v1 == Vector<uint>({4, 5, 6, 1, 2, 3}));
	cr_assert(v2.empty());
}

Test(Vector, AppendInitializerList)
{
	Vector<uint> v1({4, 5, 6});

	v1.append({1, 2, 3});

	cr_assert(v1 == Vector<uint>({4, 5, 6, 1, 2, 3}));
}

////////////////////////////////////////////////////////////////////////////////

Test(Vector, PrependElementCopy)
{
	Vector<uint> v;
	cr_assert(v.empty());

	v.prepend(uint(4));
	cr_assert(v.size() == 1);
	cr_assert(v[0] == 4);

	v.prepend(2, 7);
	cr_assert(v.size() == 8);
	cr_assert(v[0] == 2 && v[7] == 4);

	v.prepend(uint(8));
	cr_assert(v.size() == 9);
	cr_assert(v.capacity() >= 9);
	cr_assert(v[0] == 8);
}

Test(Vector, PrependElementMove)
{
	Vector<uint> v(3, 1);
	uint i = 5;
	v.prepend(std::move(i));

	cr_assert(v.size() == 4);
	cr_assert(v[0] == 5);
}

Test(Vector, PrependVectorCopy)
{
	Vector<uint> v1({4, 5, 6});
	Vector<uint> v2({1, 2, 3});

	v1.prepend(v2);

	cr_assert(v1 == Vector<uint>({1, 2, 3, 4, 5, 6}));
	cr_assert(v2 == Vector<uint>({1, 2, 3}));
}

Test(Vector, PrependVectorMove)
{
	Vector<uint> v1({4, 5, 6});
	Vector<uint> v2({1, 2, 3});

	v1.prepend(std::move(v2));

	cr_assert(v1 == Vector<uint>({1, 2, 3, 4, 5, 6}));
	cr_assert(v2.empty());
}

Test(Vector, PrependInitializerList)
{
	Vector<uint> v1({4, 5, 6});

	v1.prepend({1, 2, 3});

	cr_assert(v1 == Vector<uint>({1, 2, 3, 4, 5, 6}));
}

////////////////////////////////////////////////////////////////////////////////

Test(Vector, InsertElementCopy)
{
	// One element, begin
	Vector<uint> v1({5, 8, 9, 3, 1});
	v1.insert(0, uint(0));
	cr_assert(v1 == Vector<uint>({0, 5, 8, 9, 3, 1}));

	// One element, end
	Vector<uint> v2({5, 8, 9, 3, 1});
	v2.insert(5, uint(0));
	cr_assert(v2 == Vector<uint>({5, 8, 9, 3, 1, 0}));

	// One element, anywhere
	Vector<uint> v3({5, 8, 9, 3, 1});
	v3.insert(2, uint(0));
	cr_assert(v3 == Vector<uint>({5, 8, 0, 9, 3, 1}));

	// Multiple elements, begin
	Vector<uint> v4({5, 8, 9, 3, 1});
	v4.insert(0, 0, 5);
	cr_assert(v4 == Vector<uint>({0, 0, 0, 0, 0, 5, 8, 9, 3, 1}));

	// Multiple elements, end
	Vector<uint> v5({5, 8, 9, 3, 1});
	v5.insert(5, 0, 5);
	cr_assert(v5 == Vector<uint>({5, 8, 9, 3, 1, 0, 0, 0, 0, 0}));

	// Multiple elements, anywhere
	Vector<uint> v6({5, 8, 9, 3, 1});
	v6.insert(2, 0, 5);
	cr_assert(v6 == Vector<uint>({5, 8, 0, 0, 0, 0, 0, 9, 3, 1}));

	// Out of range
	Vector<uint> v7({5, 8, 9, 3, 1});
	v7.insert(16, uint(0));
	cr_assert(v7 == Vector<uint>({5, 8, 9, 3, 1}));
}

Test(Vector, InsertElementMove)
{
	cr_skip("TODO");
}

Test(Vector, InsertVectorCopy)
{
	Vector<uint> v1({0, 1, 2, 3, 4});
	Vector<uint> v2({5, 6, 7, 8, 9});

	v1.insert(2, v2);

	cr_assert(v1.size() == 10);
	cr_assert(v2.size() == 5);
	cr_assert(v1 == Vector<uint>({0, 1, 5, 6, 7, 8, 9, 2, 3, 4}));
}

Test(Vector, InsertVectorMove)
{
	Vector<uint> v1({0, 1, 2, 3, 4});
	Vector<uint> v2({5, 6, 7, 8, 9});

	v1.insert(2, std::move(v2));

	cr_assert(v1.size() == 10);
	cr_assert(v2.size() == 0);
	cr_assert(v1 == Vector<uint>({0, 1, 5, 6, 7, 8, 9, 2, 3, 4}));
}

Test(Vector, InsertInitializerList)
{
	Vector<uint> v1({0, 1, 2, 3, 4});

	v1.insert(2, {5, 6, 7, 8, 9});

	cr_assert(v1.size() == 10);
	cr_assert(v1 == Vector<uint>({0, 1, 5, 6, 7, 8, 9, 2, 3, 4}));
}

////////////////////////////////////////////////////////////////////////////////

Test(Vector, Emplace)
{
	cr_skip("REDO");

	// Vector<std::pair<int, char>> ascii;

	// for (int i = 'a'; i <= 'z'; ++i) {
	// 	ascii.emplace(i - 'a', i, (char)i);
	// }

	// std::cout << ascii << std::endl;
}

Test(Vector, EmplaceFront)
{
	cr_skip("TODO");
}

Test(Vector, EmplaceBack)
{
	cr_skip("TODO");
}

////////////////////////////////////////////////////////////////////////////////

Test(Vector, Clear)
{
	Vector<uint> v({1, 2, 3, 4, 5});
	cr_assert(v.capacity() >= Vector<uint>::minimumSize && v.size() == 5);
	v.clear();
	cr_assert(v.capacity() >= Vector<uint>::minimumSize && v.size() == 0);
}

Test(Vector, Erase)
{
	// Simple erase
	Vector<uint> v1({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
	v1.erase(0);
	cr_assert(v1 == Vector<uint>({2, 3, 4, 5, 6, 7, 8, 9, 10}));

	// Erase length
	Vector<uint> v2({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
	v2.erase(6, 4);
	cr_assert(v2 == Vector<uint>({1, 2, 3, 4, 5, 6}));

	// Erase length, not at the end (test memmove())
	Vector<uint> v3({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
	v3.erase(2, 4);
	cr_assert(v3 == Vector<uint>({1, 2, 7, 8, 9, 10}));

	// Erase too much
	Vector<uint> v4({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
	v4.erase(5, 16);
	cr_assert(v4 == Vector<uint>({1, 2, 3, 4, 5}));
}

Test(Vector, Filter)
{
	Vector<uint> v({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

	v.filter([](const uint &a) {
		return a & 1;
	});

	cr_assert(v == Vector<uint>({1, 3, 5, 7, 9}));
}

Test(Vector, Sort)
{
	Vector<uint> v({59,97,51,75,90,79,31,16,76,70,38,62,61,18,25,73,55,81,57,12,37,65,39,4,27,17,52,82,28,69,68,83,46,87,44,56,50,7,35,67,23,6,77,24,54,48,93,15,91,29,14,100,86,72,43,49,5,89,26,36,71,60,40,53,3,2,9,96,80,84,20,63,1,19,21,10,8,45,34,92,94,47,66,22,30,42,88,33,41,32,13,99,74,78,98,64,11,85,95,58});

	v.sort();

	for (size_t i = 1; i < v.size(); ++i) {
		if (v[i - 1] > v[i]) {
			std::cerr << v << std::endl;
			cr_assert(!"Vector not sorted");
		}
	}
}

Test(Vector, SortWithComparator)
{
	Vector<uint> v({3, 7, 1, 6, 2, 5, 8, 10, 9, 4});

	v.sort([] (const uint &a, const uint &b) -> int {
		bool ao = a & 1;
		bool bo = b & 1;

		if (ao && !bo) return -1;
		if (!ao && bo) return 1;
		return a < b ? -1 : (a == b ? 0 : 1);
	});

	cr_assert(v == Vector<uint>({1, 3, 5, 7, 9, 2, 4, 6, 8, 10}));
}

Test(Vector, Swap)
{
	Vector<uint> v1({0, 2, 4, 6, 8}), v2({1, 3, 5, 7, 9}), v1_(v1), v2_(v2);
	cr_assert(v1 == v1_ && v2 == v2_);

	v1.swap(v2);

	cr_assert(v2 == v1_);
	cr_assert(v1 == v2_);
}

Test(Vector, Shuffle)
{
	Vector<uint> v({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
	Vector<uint> w(v);

	v.shuffle();

	cr_assert(v != w);
}

Test(Vector, Reverse)
{
	Vector<uint> v({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

	v.reverse();

	cr_assert(v == Vector<uint>({9, 8, 7, 6, 5, 4, 3, 2, 1, 0}));
}