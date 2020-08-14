/*
** B tests, 2020
** Vector / Assign.cpp
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

/// Capacity ///////////////////////////////////////////////////////////////////

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

/// Element Access /////////////////////////////////////////////////////////////

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
	cr_assert_not_null((const uint*)v.data());

	v.~Vector<uint>();
	cr_assert_null((const uint*)v.data());
}

Test(Vector, Data)
{
	Vector<uint> v({7, 5, 6, 1, 9, 4, 3});

	uint* a = v.data();
	a[2] = 10;

	cr_assert_eq(v.at(2), 10);
}

Test(Vector, FindElement)
{
	Vector<uint> v = {1,0,1,1,0,1,0,0,1,1,1,0,0,0,1,0,1,1,1,0,0};
	Vector<size_t> pos = {0,2,3,5,8,9,10,14,16,17,18};

	size_t i = 0;
	for (size_t p = 0; (p = v.find(1, p)) != Vector<uint>::max; ++p) {
		cr_expect_eq(p, pos[i++]);
	}

	cr_assert_eq(i, pos.size());
}

Test(Vector, FindWithPredicate)
{
	Vector<uint> v = {2,29,21,10,30,14,20,25,6,7,3,24,17,9,11,19,1,15,8,26,5,12,22,16,18,28,4,23,27,13};
	Vector<size_t> pos = {1,2,7,9,10,12,13,14,15,16,17,20,27,28,29};

	auto f = [] (uint t) -> bool {
		return t & 1;
	};

	size_t i = 0;
	for (size_t p = 0; (p = v.find(f, p)) != Vector<uint>::max; ++p) {
		cr_expect_eq(p, pos[i++]);
	}

	cr_assert_eq(i, pos.size());
}

Test(Vector, FindLastElement)
{
	Vector<uint> v = {1,0,1,1,0,1,0,0,1,1,1,0,0,0,1,0,1,1,1,0,0};
	Vector<size_t> pos = {18,17,16,14,10,9,8,5,3,2,0};

	size_t i = 0;
	for (size_t p = v.size() - 1; (p = v.findLast(1, p)) != Vector<uint>::max; --p) {
		cr_expect_eq(p, pos[i++]);
		if (p == 0)
			break;
	}

	cr_assert_eq(i, pos.size());
}

Test(Vector, FindLastWithPredicate)
{
	Vector<uint> v = {2,29,21,10,30,14,20,25,6,7,3,24,17,9,11,19,1,15,8,26,5,12,22,16,18,28,4,23,27,13};
	Vector<size_t> pos = {29,28,27,20,17,16,15,14,13,12,10,9,7,2,1};

	auto f = [] (uint t) -> bool {
		return t & 1;
	};

	size_t i = 0;
	for (size_t p = v.size() - 1; (p = v.findLast(f, p)) != Vector<uint>::max; --p) {
		cr_expect_eq(p, pos[i++]);
	}

	cr_assert_eq(i, pos.size());
}

/// Modifiers //////////////////////////////////////////////////////////////////

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
	cr_assert_eq(v.size(), 4);
	cr_assert(v[0] == 1 &&
	          v[1] == 5 &&
	          v[2] == 9 &&
	          v[3] == 7);
}

Test(Vector, AppendElementCopy)
{
	Vector<uint> v;
	cr_assert(v.empty());

	v.append(uint(4));
	cr_assert_eq(v.size(), 1);
	cr_assert_eq(v[0], 4);

	v.append(2, 7);
	cr_assert_eq(v.size(), 8);
	cr_assert_eq(v[0], 4);
	cr_assert_eq(v[7], 2);

	v.append(uint(8));
	cr_assert_eq(v.size(), 9);
	cr_assert_geq(v.capacity(), 9);
	cr_assert_eq(v[8], 8);
}

Test(Vector, AppendElementMove)
{
	Vector<uint> v(3, 4);
	uint i = 5;
	v.append(std::move(i));

	cr_assert_eq(v.size(), 4);
	cr_assert_eq(v[3], 5);
}

Test(Vector, AppendVectorCopy)
{
	Vector<uint> v1({4, 5, 6});
	Vector<uint> v2({1, 2, 3});

	v1.append(v2);

	cr_assert_eq(v1, Vector<uint>({4, 5, 6, 1, 2, 3}));
	cr_assert_eq(v2, Vector<uint>({1, 2, 3}));
}

Test(Vector, AppendVectorMove)
{
	Vector<uint> v1({4, 5, 6});
	Vector<uint> v2({1, 2, 3});

	v1.append(std::move(v2));

	cr_assert_eq(v1, Vector<uint>({4, 5, 6, 1, 2, 3}));
	cr_assert(v2.empty());
}

Test(Vector, AppendInitializerList)
{
	Vector<uint> v1({4, 5, 6});

	v1.append({1, 2, 3});

	cr_assert_eq(v1, Vector<uint>({4, 5, 6, 1, 2, 3}));
}

Test(Vector, PrependElementCopy)
{
	Vector<uint> v;
	cr_assert(v.empty());

	v.prepend(uint(4));
	cr_assert_eq(v.size(), 1);
	cr_assert_eq(v[0], 4);

	v.prepend(2, 7);
	cr_assert_eq(v.size(), 8);
	cr_assert_eq(v[0], 2);
	cr_assert_eq(v[7], 4);

	v.prepend(uint(8));
	cr_assert_eq(v.size(), 9);
	cr_assert_geq(v.capacity(), 9);
	cr_assert_eq(v[0], 8);
}

Test(Vector, PrependElementMove)
{
	Vector<uint> v(3, 1);
	uint i = 5;
	v.prepend(std::move(i));

	cr_assert_eq(v.size(), 4);
	cr_assert_eq(v[0], 5);
}

Test(Vector, PrependVectorCopy)
{
	Vector<uint> v1({4, 5, 6});
	Vector<uint> v2({1, 2, 3});

	v1.prepend(v2);

	cr_assert_eq(v1, Vector<uint>({1, 2, 3, 4, 5, 6}));
	cr_assert_eq(v2, Vector<uint>({1, 2, 3}));
}

Test(Vector, PrependVectorMove)
{
	Vector<uint> v1({4, 5, 6});
	Vector<uint> v2({1, 2, 3});

	v1.prepend(std::move(v2));

	cr_assert_eq(v1, Vector<uint>({1, 2, 3, 4, 5, 6}));
	cr_assert(v2.empty());
}

Test(Vector, PrependInitializerList)
{
	Vector<uint> v1({4, 5, 6});

	v1.prepend({1, 2, 3});

	cr_assert_eq(v1, Vector<uint>({1, 2, 3, 4, 5, 6}));
}

Test(Vector, InsertElementCopy)
{
	// One element, begin
	Vector<uint> v1({5, 8, 9, 3, 1});
	v1.insert(0, uint(0));
	cr_assert_eq(v1, Vector<uint>({0, 5, 8, 9, 3, 1}));

	// One element, end
	Vector<uint> v2({5, 8, 9, 3, 1});
	v2.insert(5, uint(0));
	cr_assert_eq(v2, Vector<uint>({5, 8, 9, 3, 1, 0}));

	// One element, anywhere
	Vector<uint> v3({5, 8, 9, 3, 1});
	v3.insert(2, uint(0));
	cr_assert_eq(v3, Vector<uint>({5, 8, 0, 9, 3, 1}));

	// Multiple elements, begin
	Vector<uint> v4({5, 8, 9, 3, 1});
	v4.insert(0, 0, 5);
	cr_assert_eq(v4, Vector<uint>({0, 0, 0, 0, 0, 5, 8, 9, 3, 1}));

	// Multiple elements, end
	Vector<uint> v5({5, 8, 9, 3, 1});
	v5.insert(5, 0, 5);
	cr_assert_eq(v5, Vector<uint>({5, 8, 9, 3, 1, 0, 0, 0, 0, 0}));

	// Multiple elements, anywhere
	Vector<uint> v6({5, 8, 9, 3, 1});
	v6.insert(2, 0, 5);
	cr_assert_eq(v6, Vector<uint>({5, 8, 0, 0, 0, 0, 0, 9, 3, 1}));

	// Out of range
	Vector<uint> v7({5, 8, 9, 3, 1});
	v7.insert(16, uint(0));
	cr_assert_eq(v7, Vector<uint>({5, 8, 9, 3, 1}));
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

	cr_assert_eq(v1.size(), 10);
	cr_assert_eq(v2.size(), 5);
	cr_assert_eq(v1, Vector<uint>({0, 1, 5, 6, 7, 8, 9, 2, 3, 4}));
}

Test(Vector, InsertVectorMove)
{
	Vector<uint> v1({0, 1, 2, 3, 4});
	Vector<uint> v2({5, 6, 7, 8, 9});

	v1.insert(2, std::move(v2));

	cr_assert_eq(v1.size(), 10);
	cr_assert_eq(v2.size(), 0);
	cr_assert_eq(v1, Vector<uint>({0, 1, 5, 6, 7, 8, 9, 2, 3, 4}));
}

Test(Vector, InsertInitializerList)
{
	Vector<uint> v1({0, 1, 2, 3, 4});

	v1.insert(2, {5, 6, 7, 8, 9});

	cr_assert_eq(v1.size(), 10);
	cr_assert_eq(v1, Vector<uint>({0, 1, 5, 6, 7, 8, 9, 2, 3, 4}));
}

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

Test(Vector, Clear)
{
	Vector<uint> v({1, 2, 3, 4, 5});
	cr_assert_geq(v.capacity(), Vector<uint>::minimumSize);
	cr_assert_eq(v.size(), 5);

	v.clear();

	cr_assert_geq(v.capacity(), Vector<uint>::minimumSize);
	cr_assert_eq(v.size(), 0);
}

Test(Vector, Erase)
{
	// Simple erase
	Vector<uint> v1({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
	v1.erase(0);
	cr_assert_eq(v1, Vector<uint>({2, 3, 4, 5, 6, 7, 8, 9, 10}));

	// Erase length
	Vector<uint> v2({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
	v2.erase(6, 4);
	cr_assert_eq(v2, Vector<uint>({1, 2, 3, 4, 5, 6}));

	// Erase length, not at the end (test memmove())
	Vector<uint> v3({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
	v3.erase(2, 4);
	cr_assert_eq(v3, Vector<uint>({1, 2, 7, 8, 9, 10}));

	// Erase too much
	Vector<uint> v4({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
	v4.erase(5, 16);
	cr_assert_eq(v4, Vector<uint>({1, 2, 3, 4, 5}));
}

Test(Vector, Filter)
{
	Vector<uint> v({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

	v.filter([] (const uint& a) {
		return a & 1;
	});

	cr_assert_eq(v, Vector<uint>({1, 3, 5, 7, 9}));
}

Test(Vector, Sort)
{
	Vector<uint> v({59,97,51,75,90,79,31,16,76,70,38,62,61,18,25,73,55,81,57,12,37,65,39,4,27,17,52,82,28,69,68,83,46,87,44,56,50,7,35,67,23,6,77,24,54,48,93,15,91,29,14,100,86,72,43,49,5,89,26,36,71,60,40,53,3,2,9,96,80,84,20,63,1,19,21,10,8,45,34,92,94,47,66,22,30,42,88,33,41,32,13,99,74,78,98,64,11,85,95,58});

	v.sort();

	for (size_t i = 1; i < v.size(); ++i) {
		cr_expect_leq(v[i - 1], v[i]);
	}
}

Test(Vector, SortWithComparator)
{
	Vector<uint> v({3, 7, 1, 6, 2, 5, 8, 10, 9, 4});

	v.sort([] (const uint& a, const uint& b) -> bool {
		if ((a & 1) != (b & 1))
			return a & 1;
		return a < b;
	});

	cr_assert_eq(v, Vector<uint>({1, 3, 5, 7, 9, 2, 4, 6, 8, 10}));
}

Test(Vector, Swap)
{
	Vector<uint> v1({0, 2, 4, 6, 8}), v2({1, 3, 5, 7, 9}), v1_(v1), v2_(v2);
	cr_assert_eq(v1, v1_);
	cr_assert_eq(v2, v2_);

	v1.swap(v2);

	cr_assert_eq(v2, v1_);
	cr_assert_eq(v1, v2_);
}

Test(Vector, Shuffle)
{
	Vector<uint> v({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
	Vector<uint> w(v);

	v.shuffle();

	cr_assert_neq(v, w);
}

Test(Vector, Reverse)
{
	Vector<uint> v({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

	v.reverse();

	cr_assert_eq(v, Vector<uint>({9, 8, 7, 6, 5, 4, 3, 2, 1, 0}));
}