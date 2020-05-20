TEST("Vector::at(size_t) const", []
{
	Vector<TYPE> v({7, 5, 6, 1, 6, 9, 4, 3});
	assert(v.at(0) == 7);
	assert(v.at(1) == 5);
	assert(v.at(2) == 6);
	assert(v.at(3) == 1);
	assert(v.at(4) == 6);
	assert(v.at(5) == 9);
	assert(v.at(6) == 4);
	assert(v.at(7) == 3);

	bool exceptionCaught = false;
	try {
		v.at(8);
	}
	catch (std::out_of_range &) {
		exceptionCaught = true;
	}
	assert(exceptionCaught);
});

TEST("Vector::at(size_t)", []
{
	Vector<TYPE> v(8);
	v.at(3) = 1;
	assert(v[3] == 1);
});


TEST("Vector::front() const", []
{
	Vector<TYPE> v({7, 5, 6, 1, 6, 9, 4, 3});
	assert(v.front() == v.at(0));
});

TEST("Vector::front()", []
{
	Vector<TYPE> v({7, 5, 6, 1, 6, 9, 4, 3});
	v.front() = -1;
	assert(v[0] == -1);
});


TEST("Vector::back() const", []
{
	Vector<TYPE> v({7, 5, 6, 1, 6, 9, 4, 3});
	assert(v.back() == v.at(v.size() - 1));
});

TEST("Vector::back()", []
{
	Vector<TYPE> v({7, 5, 6, 1, 6, 9, 4, 3});
	v.back() = -1;
	assert(v[v.size() - 1] == -1);
});


TEST("Vector::data() const", []
{
	Vector<TYPE> v({7, 5, 6, 1, 9, 4, 3});
	assert(v.data() != nullptr);

	v.~Vector<TYPE>();
	assert(v.data() == nullptr);
});

TEST("Vector::data()", []
{
	Vector<TYPE> v({7, 5, 6, 1, 9, 4, 3});
	TYPE *a = v.data();
	a[2] = -1;

	assert(v[2] == -1);
});