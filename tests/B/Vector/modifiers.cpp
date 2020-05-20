TEST("Vector::assign(const Vector &)", []
{
	Vector<TYPE> v1({1, 5, 2, 9});
	Vector<TYPE> v2;

	v2.assign(v1);
	assert(v1 == v2);
});

TEST("Vector::assign(Vector &&)", []
{
	Vector<TYPE> v1({1, 5, 2, 9});
	Vector<TYPE> v2;

	v2.assign(std::move(v1));
	assert(v2.size() == 4 && v1.empty());
});

TEST("Vector::assign(size_t, const T &)", []
{
	Vector<TYPE> v({1, 5, 2, 9});

	v.assign(2, 10);
	assert(v.size() == 2 && v[0] == v[1] && v[1] == 10);
});

TEST("Vector::assign(std::initializer_list)", []
{
	Vector<TYPE> v(5, 10);

	v.assign({1, 5, 9, 7});
	assert(v.size() == 4 &&
		v[0] == 1 &&
		v[1] == 5 &&
		v[2] == 9 &&
		v[3] == 7
	);
});

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

TEST("Vector::append(const T &, size_t = 1)", []
{
	Vector<TYPE> v;
	assert(v.empty());

	v.append(TYPE(4));
	assert(v.size() == 1);
	assert(v[0] == 4);

	v.append(2, 7);
	assert(v.size() == 8);
	assert(v[0] == 4 && v[7] == 2);

	v.append(TYPE(8));
	assert(v.size() == 9);
	assert(v.capacity() >= 9);
	assert(v[8] == 8);
});

TEST("Vector::append(T &&)", []
{
	Vector<TYPE> v(3, 4);
	TYPE i = 5;
	v.append(std::move(i));

	assert(v.size() == 4);
	assert(v[3] == 5);
});

TEST("Vector::append(const Vector &)", []
{
	Vector<TYPE> v1({4, 5, 6});
	Vector<TYPE> v2({1, 2, 3});

	v1.append(v2);

	assert(v1 == Vector<TYPE>({4, 5, 6, 1, 2, 3}));
	assert(v2 == Vector<TYPE>({1, 2, 3}));
});

TEST("Vector::append(Vector &&)", []
{
	Vector<TYPE> v1({4, 5, 6});
	Vector<TYPE> v2({1, 2, 3});

	v1.append(std::move(v2));

	assert(v1 == Vector<TYPE>({4, 5, 6, 1, 2, 3}));
	assert(v2.empty());
});

TEST("Vector::append(std::initializer_list)", []
{
	Vector<TYPE> v1({4, 5, 6});

	v1.append({1, 2, 3});

	assert(v1 == Vector<TYPE>({4, 5, 6, 1, 2, 3}));
});

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

TEST("Vector::prepend(const T &, size_t = 1)", []
{
	Vector<TYPE> v;
	assert(v.empty());

	v.prepend(TYPE(4));
	assert(v.size() == 1);
	assert(v[0] == 4);

	v.prepend(2, 7);
	assert(v.size() == 8);
	assert(v[0] == 2 && v[7] == 4);

	v.prepend(TYPE(8));
	assert(v.size() == 9);
	assert(v.capacity() >= 9);
	assert(v[0] == 8);
});

TEST("Vector::prepend(T &&)", []
{
	Vector<TYPE> v(3, 1);
	TYPE i = 5;
	v.prepend(std::move(i));

	assert(v.size() == 4);
	assert(v[0] == 5);
});

TEST("Vector::prepend(const Vector &)", []
{
	Vector<TYPE> v1({4, 5, 6});
	Vector<TYPE> v2({1, 2, 3});

	v1.prepend(v2);

	assert(v1 == Vector<TYPE>({1, 2, 3, 4, 5, 6}));
	assert(v2 == Vector<TYPE>({1, 2, 3}));
});

TEST("Vector::prepend(Vector &&)", []
{
	Vector<TYPE> v1({4, 5, 6});
	Vector<TYPE> v2({1, 2, 3});

	v1.prepend(std::move(v2));

	assert(v1 == Vector<TYPE>({1, 2, 3, 4, 5, 6}));
	assert(v2.empty());
});

TEST("Vector::prepend(std::initializer_list)", []
{
	Vector<TYPE> v1({4, 5, 6});

	v1.prepend({1, 2, 3});

	assert(v1 == Vector<TYPE>({1, 2, 3, 4, 5, 6}));
});

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

TEST("Vector::insert(size_t, const T &, size_t = 1)", []
{
	// One element, begin
	Vector<TYPE> v1({5, 8, 9, 3, 1});
	v1.insert(0, TYPE(0));
	assert(v1 == Vector<TYPE>({0, 5, 8, 9, 3, 1}));

	// One element, end
	Vector<TYPE> v2({5, 8, 9, 3, 1});
	v2.insert(5, TYPE(0));
	assert(v2 == Vector<TYPE>({5, 8, 9, 3, 1, 0}));

	// One element, anywhere
	Vector<TYPE> v3({5, 8, 9, 3, 1});
	v3.insert(2, TYPE(0));
	assert(v3 == Vector<TYPE>({5, 8, 0, 9, 3, 1}));

	// Multiple elements, begin
	Vector<TYPE> v4({5, 8, 9, 3, 1});
	v4.insert(0, 0, 5);
	assert(v4 == Vector<TYPE>({0, 0, 0, 0, 0, 5, 8, 9, 3, 1}));

	// Multiple elements, end
	Vector<TYPE> v5({5, 8, 9, 3, 1});
	v5.insert(5, 0, 5);
	assert(v5 == Vector<TYPE>({5, 8, 9, 3, 1, 0, 0, 0, 0, 0}));

	// Multiple elements, anywhere
	Vector<TYPE> v6({5, 8, 9, 3, 1});
	v6.insert(2, 0, 5);
	assert(v6 == Vector<TYPE>({5, 8, 0, 0, 0, 0, 0, 9, 3, 1}));

	// Out of range
	Vector<TYPE> v7({5, 8, 9, 3, 1});
	v7.insert(16, TYPE(0));
	assert(v7 == Vector<TYPE>({5, 8, 9, 3, 1}));
});

// TEST("Vector::insert(size_t, T &&)", []
// {
// });

TEST("Vector::insert(size_t, const Vector &)", []
{
	Vector<TYPE> v1({0, 1, 2, 3, 4});
	Vector<TYPE> v2({5, 6, 7, 8, 9});

	v1.insert(2, v2);

	assert(v1.size() == 10);
	assert(v2.size() == 5);
	assert(v1 == Vector<TYPE>({0, 1, 5, 6, 7, 8, 9, 2, 3, 4}));
});

TEST("Vector::insert(size_t, Vector &&)", []
{
	Vector<TYPE> v1({0, 1, 2, 3, 4});
	Vector<TYPE> v2({5, 6, 7, 8, 9});

	v1.insert(2, std::move(v2));

	assert(v1.size() == 10);
	assert(v2.size() == 0);
	assert(v1 == Vector<TYPE>({0, 1, 5, 6, 7, 8, 9, 2, 3, 4}));
});

TEST("Vector::insert(size_t, std::initializer_list)", []
{
	Vector<TYPE> v1({0, 1, 2, 3, 4});

	v1.insert(2, {5, 6, 7, 8, 9});

	assert(v1.size() == 10);
	assert(v1 == Vector<TYPE>({0, 1, 5, 6, 7, 8, 9, 2, 3, 4}));
});

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// TEST("Vector::emplace(size_t, Args&&...)", []
// {
// 	Vector<std::pair<int, char>> ascii;

// 	for (int i = 'a'; i <= 'z'; ++i) {
// 		ascii.emplace(i - 'a', i, (char)i);
// 	}

// 	std::cout << ascii << std::endl;
// });

// TEST("Vector::emplaceFront(Args&&...)", []
// {
// });

// TEST("Vector::emplaceBack(Args&&...)", []
// {
// });

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

TEST("Vector::clear()", []
{
	Vector<TYPE> v({1, 2, 3, 4, 5});
	assert(v.capacity() >= Vector<TYPE>::minimumSize && v.size() == 5);
	v.clear();
	assert(v.capacity() >= Vector<TYPE>::minimumSize && v.size() == 0);
});

TEST("Vector::erase(size_t, size_t = 1)", []
{
	// Simple erase
	Vector<TYPE> v1({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
	v1.erase(0);
	assert(v1 == Vector<TYPE>({2, 3, 4, 5, 6, 7, 8, 9, 10}));

	// Erase length
	Vector<TYPE> v2({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
	v2.erase(6, 4);
	assert(v2 == Vector<TYPE>({1, 2, 3, 4, 5, 6}));

	// Erase length, not at the end (test memmove())
	Vector<TYPE> v3({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
	v3.erase(2, 4);
	assert(v3 == Vector<TYPE>({1, 2, 7, 8, 9, 10}));

	// Erase too much
	Vector<TYPE> v4({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
	v4.erase(5, 16);
	assert(v4 == Vector<TYPE>({1, 2, 3, 4, 5}));
});

TEST("Vector::filter(std::function<int(const T&)>)", []
{
	Vector<TYPE> v({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

	v.filter([](const TYPE &a) {
		return a.n() & 1;
	});

	assert(v == Vector<TYPE>({1, 3, 5, 7, 9}));
});

TEST("Vector::sort()", []
{
	Vector<TYPE> v({59,97,51,75,90,79,31,16,76,70,38,62,61,18,25,73,55,81,57,12,37,65,39,4,27,17,52,82,28,69,68,83,46,87,44,56,50,7,35,67,23,6,77,24,54,48,93,15,91,29,14,100,86,72,43,49,5,89,26,36,71,60,40,53,3,2,9,96,80,84,20,63,1,19,21,10,8,45,34,92,94,47,66,22,30,42,88,33,41,32,13,99,74,78,98,64,11,85,95,58});

	v.sort();

	for (size_t i = 1; i < v.size(); ++i) {
		if (v[i - 1] > v[i]) {
			std::cerr << v << std::endl;
			assert(!"Vector not sorted");
		}
	}
});

TEST("Vector::sort(std::function<bool(const T&, const T&)>)", []
{
	Vector<TYPE> v({3, 7, 1, 6, 2, 5, 8, 10, 9, 4});

	v.sort([](const TYPE &a, const TYPE &b) -> int {
		bool ao = a.n() & 1;
		bool bo = b.n() & 1;

		if (ao && !bo) return -1;
		if (!ao && bo) return 1;
		return a < b ? -1 : (a == b ? 0 : 1);
	});

	assert(v == Vector<TYPE>({1, 3, 5, 7, 9, 2, 4, 6, 8, 10}));
});

TEST("Vector::swap(Vector &)", []
{
	Vector<TYPE> v1({0, 2, 4, 6, 8}), v2({1, 3, 5, 7, 9}), v1_(v1), v2_(v2);
	assert(v1 == v1_ && v2 == v2_);

	v1.swap(v2);

	assert(v2 == v1_);
	assert(v1 == v2_);
});

TEST("Vector::shuffle()", []
{
	Vector<TYPE> v({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
	Vector<TYPE> w(v);

	v.shuffle();

	assert(v != w);
});

TEST("Vector::reverse()", []
{
	Vector<TYPE> v({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});

	v.reverse();

	assert(v == Vector<TYPE>({9, 8, 7, 6, 5, 4, 3, 2, 1, 0}));
});