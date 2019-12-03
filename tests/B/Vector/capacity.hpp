TEST("Vector::empty()", []
{
	Vector<TYPE> v1;
	assert(v1.empty());

	Vector<TYPE> v2(3, 10);
	assert(!v2.empty());
});

TEST("Vector::contains()", []
{
	Vector<TYPE> v({1, 3, 5, 7, 9});
	assert(v.contains(3));
	assert(!v.contains(2));
});

TEST("Vector::resize(size_t, const T &)", []
{
	Vector<TYPE> arr;
	Vector<TYPE> cpy(arr);

	// remains unchanged when requested size is the same as current size
	arr.resize(0);
	assert(arr.size() == cpy.size());
	assert(arr.capacity() == cpy.capacity());

	// expand size but not capacity
	arr.resize(3, 5);
	assert(arr.size() > cpy.size());
	assert(arr.capacity() >= cpy.capacity());
	assert(arr.capacity() >= Vector<TYPE>::minimumSize);

	// expand size and capacity
	arr.resize(10, 3);
	assert(arr.size() == 10);
	assert(arr.capacity() >= 10);

	// reduce size
	arr.resize(2);
	assert(arr.size() == 2);
	assert(arr.capacity() >= 10);
});

TEST("Vector::reserve(size_t)", []
{
	Vector<TYPE> v;
	assert(v.capacity() == Vector<TYPE>::minimumSize);

	v.reserve(20);
	assert(v.capacity() >= 20);

	v.reserve(10); // does nothing
	assert(v.capacity() >= 20);
});

TEST("Vector::shrink()", []
{
	Vector<TYPE> v(10, 100);
	assert(v.size() == 10);
	assert(v.capacity() >= 10);

	v.shrink();
	assert(v.size() == 10);
	assert(v.capacity() == 10);
});