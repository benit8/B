/*
** B tests, 2020
** SingleList.cpp
*/

#include "B/Containers/SingleList.hpp"
#include "B/String.hpp"
#include <criterion/criterion.h>

using B::SingleList;

////////////////////////////////////////////////////////////////////////////////

template <typename T>
static inline bool checkListContent(const SingleList<T> &list, std::initializer_list<T> content)
{
	auto it1 = list.begin();
	auto it2 = content.begin();

	while (!it1.isEnd() && it2 != content.end()) {
		if (*it1 != *it2)
			return false;
		it1++;
		it2++;
	}
	return it1.isEnd() && it2 == content.end();
}

////////////////////////////////////////////////////////////////////////////////

Test(SingleList, EmptyConstructor)
{
	SingleList<int> list;

	cr_assert(list.empty());
	cr_assert_eq(list.size(), 0);
}

Test(SingleList, CopyConstructor)
{
	SingleList<int> original({4, 2, 3, 1});
	SingleList<int> copy(original);

	cr_assert_eq(copy, original);
	cr_assert_eq(copy.size(), original.size());
}

Test(SingleList, MoveConstructor)
{
	SingleList<int> original({4, 2, 3, 1});
	SingleList<int> moved(std::move(original));

	cr_assert(original.empty());
	cr_assert_not(moved.empty());
	cr_assert_eq(moved.size(), 4);
}

Test(SingleList, InitializerListConstructor)
{
	SingleList<int> list {9, 5, 7, 2, 3, 8};

	cr_assert_not(list.empty());
	cr_assert_eq(list.size(), 6);

	cr_assert(checkListContent(list, {9, 5, 7, 2, 3, 8}));
}

Test(SingleList, Destructor)
{
	SingleList<int> list {9, 5, 7, 2, 3, 8};
	cr_assert_not(list.empty());
	cr_assert_eq(list.size(), 6);

	list.~SingleList();
	cr_assert(list.empty());
}

/// Capacity ///////////////////////////////////////////////////////////////////

Test(SingleList, Size)
{
	SingleList<int> list {9, 5, 7, 2, 3, 8};
	cr_assert_eq(list.size(), 6);

	list.clear();
	cr_assert_eq(list.size(), 0);
}

Test(SingleList, Empty)
{
	SingleList<int> list;
	cr_assert(list.empty());

	list.push(4);
	cr_assert_not(list.empty());
}

Test(SingleList, Clear)
{
	SingleList<int> list {9, 5, 7, 2, 3, 8};
	cr_assert_neq(list.begin(), list.end());

	list.clear();
	cr_assert_eq(list.begin(), list.end());
}

/// Element Access /////////////////////////////////////////////////////////////

Test(SingleList, First)
{
	SingleList<int> list {9, 5, 7, 2, 3, 8};
	cr_assert_eq(list.first(), 9);
}

Test(SingleList, BeginEnd)
{
	SingleList<int> list {9, 5, 7, 2, 3, 8};
	auto it = list.begin();

	cr_expect_eq(*it, 9); it++;
	cr_expect_eq(*it, 5); it++;
	cr_expect_eq(*it, 7); it++;
	cr_expect_eq(*it, 2); it++;
	cr_expect_eq(*it, 3); it++;
	cr_expect_eq(*it, 8); it++;

	cr_assert(it.isEnd());
	cr_assert_eq(it, list.end());
}

/// Operations /////////////////////////////////////////////////////////////////

Test(SingleList, Assign)
{
	SingleList<int> list;
	list.assign({9, 5, 7, 2, 3, 8});

	cr_assert(checkListContent(list, {9, 5, 7, 2, 3, 8}));
}

Test(SingleList, CopyPush)
{
	B::String s("lorem");
	SingleList<B::String> list;

	list.push(s);

	cr_assert_not(list.empty());
	cr_assert(!s.empty());
	cr_assert_eq(s, "lorem");
	cr_assert_eq(list.first(), s);
}

Test(SingleList, MovePush)
{
	B::String s("lorem");
	SingleList<B::String> list;

	list.push(std::move(s));

	cr_assert_not(list.empty());
	cr_assert(s.empty());
	cr_assert_eq(list.first(), "lorem");
}

Test(SingleList, Pop)
{
	SingleList<int> list {9, 5, 7, 2, 3, 8};

	list.pop(); cr_assert(checkListContent(list, {5, 7, 2, 3, 8}));
	list.pop(); cr_assert(checkListContent(list, {7, 2, 3, 8}));
	list.pop(); cr_assert(checkListContent(list, {2, 3, 8}));
	list.pop(); cr_assert(checkListContent(list, {3, 8}));
	list.pop(); cr_assert(checkListContent(list, {8}));
	list.pop(); cr_assert(list.empty());
}

Test(SingleList, CopyInsertAfter)
{
	SingleList<B::String> correct {"lorem", "ipsum", "dolor", "sit", "amet"};
	SingleList<B::String> list {"lorem", "ipsum", "dolor", "amet"};
	auto it = list.begin(); it++; it++;

	B::String el("sit");
	auto res = list.insertAfter(it, el);

	cr_assert_eq(list, correct);
	cr_assert_not(el.empty());
	cr_assert_eq(*res, "sit");
}

Test(SingleList, MoveInsertAfter)
{
	SingleList<B::String> correct {"lorem", "ipsum", "dolor", "sit", "amet"};
	SingleList<B::String> list {"lorem", "ipsum", "dolor", "amet"};
	auto it = list.begin(); it++; it++;

	B::String el("sit");
	auto res = list.insertAfter(it, std::move(el));

	cr_assert_eq(list, correct);
	cr_assert(el.empty());
	cr_assert_eq(*res, "sit");
}

Test(SingleList, EraseSingle)
{
	SingleList<int> list {9, 5, 7, 2, 3, 8};
	auto it = list.begin(); it++; it++;

	list.erase(it);
	cr_assert(checkListContent(list, {9, 5, 2, 3, 8}));
}

Test(SingleList, EraseRange)
{
	SingleList<int> list {9, 5, 7, 2, 3, 8};
	auto it1 = list.begin(); it1++; it1++;
	auto it2 = it1; it2++; it2++;

	list.erase(it1, it2);
	cr_assert(checkListContent(list, {9, 5, 3, 8}));
}

Test(SingleList, Remove)
{
	SingleList<int> list {9, 1, 5, 7, 1, 2, 3, 1, 8, 1};
	cr_expect(list.contains(1));

	size_t r = list.remove(1);

	cr_assert_eq(r, 4);
	cr_assert_not(list.contains(1));
}

Test(SingleList, RemoveIf)
{
	SingleList<int> list {9, 5, 7, 2, 3, 8};

	size_t r = list.removeIf([] (int a) { return a & 1; });

	cr_assert_eq(r, 4);
	cr_assert(checkListContent(list, {2, 8}));
}

Test(SingleList, Unique)
{
	SingleList<int> list {5,6,2,1,4,3,5,4,6,1,3,5,4,6,1,5,6,1,4,3,5,4,6,1,5,6,1,4,3,5,6,1,4};

	list.sort();
	size_t r = list.unique();

	cr_assert_eq(r, 27);
	cr_assert(checkListContent(list, {1, 2, 3, 4, 5, 6}));
}

Test(SingleList, Splice)
{
	SingleList<int> list1 {9, 5, 7, 2, 3, 8};
	auto it = list1.begin(); it++; it++;
	auto list2 = list1.splice(it);

	cr_assert(checkListContent(list1, {9, 5}));
	cr_assert(checkListContent(list2, {7, 2, 3, 8}));
}

Test(SingleList, Reverse)
{
	SingleList<int> list {9, 5, 7, 2, 3, 8};

	list.reverse();

	cr_assert(checkListContent(list, {8, 3, 2, 7, 5, 9}));
}

Test(SingleList, Sort)
{
	SingleList<int> list {9, 5, 7, 2, 3, 8};

	list.sort();

	cr_assert(checkListContent(list, {2, 3, 5, 7, 8, 9}));
}

Test(SingleList, CustomSort)
{
	SingleList<int> list {9, 6, 5, 7, 2, 3, 8, 4};

	list.sort([] (int a, int b) -> bool {
		if ((a & 1) != (b & 1))
			return a & 1;
		return a < b;
	});

	cr_assert(checkListContent(list, {3, 5, 7, 9, 2, 4, 6, 8}));
}

Test(SingleList, Contains)
{
	SingleList<int> list {9, 5, 7, 2, 3, 8};

	cr_expect_not(list.contains(1));
	cr_expect(    list.contains(2));
	cr_expect(    list.contains(3));
	cr_expect_not(list.contains(4));
	cr_expect(    list.contains(5));
	cr_expect_not(list.contains(6));
	cr_expect(    list.contains(7));
	cr_expect(    list.contains(8));
	cr_expect(    list.contains(9));
	cr_expect_not(list.contains(10));
}

Test(SingleList, Find)
{
	SingleList<int> list {9, 5, 7, 2, 3, 8};

	auto it = list.find(7);

	cr_assert_not(it.isEnd());
	cr_assert_eq(*it, 7);
	it++;
	cr_assert_eq(*it, 2);
}

/// Operators //////////////////////////////////////////////////////////////////

Test(SingleList, MoveAssignOperator)
{
	SingleList<int> list1 {9, 5, 7, 2, 3, 8};
	SingleList<int> list2;

	list2 = std::move(list1);

	cr_assert(list1.empty());
	cr_assert_not(list2.empty());
	cr_assert(checkListContent(list2, {9, 5, 7, 2, 3, 8}));
}

Test(SingleList, EqualityOperator)
{
	SingleList<int> list1 {9, 5, 7, 2, 3, 8};
	SingleList<int> list2 {9, 5, 7, 2, 3, 8};

	bool equals = list1 == list2;

	cr_assert(equals);
}

Test(SingleList, InequalityOperator)
{
	SingleList<int> list1 {9, 5, 7, 2, 3, 8};
	SingleList<int> list2 {9, 5, 7, 2, 3, 7};

	bool inequals = list1 != list2;

	cr_assert(inequals);
}