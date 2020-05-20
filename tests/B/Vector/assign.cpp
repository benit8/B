TEST("Vector::Vector()", []
{
	Vector<TYPE> v;
	assert(v.size() == 0);
	assert(v.capacity() >= Vector<TYPE>::minimumSize);
	assert(v.data() != nullptr);
});

TEST("Vector::Vector(size_t [< minimumSize])", []
{
	Vector<TYPE> v(4);
	assert(v.capacity() >= Vector<TYPE>::minimumSize && v.capacity() >= 4);
	assert(v.data() != nullptr);
	assert(v.size() == 4);
});

TEST("Vector::Vector(size_t [> minimumSize])", []
{
	Vector<TYPE> v(14);
	assert(v.capacity() >= Vector<TYPE>::minimumSize && v.capacity() >= 14);
	assert(v.data() != nullptr);
	assert(v.size() == 14);
});

TEST("Vector::Vector(size_t, const T &)", []
{
	Vector<TYPE> v(4, 100);
	assert(v.capacity() >= Vector<TYPE>::minimumSize && v.capacity() >= 4);
	assert(v.data() != nullptr);
	assert(v.size() == 4);

	assert((v[0] == v[1]) && (v[1] == v[2]) && (v[2] == v[3]) && (v[3] == 100));
});

TEST("Vector::Vector(const Vector &)", []
{
	Vector<TYPE> v1(6, 200);
	Vector<TYPE> v2(v1);
	assert(v2.capacity() >= v1.capacity());
	assert(v2.data() != nullptr);
	assert(v2.size() == v1.size());

	assert((v2[0] == v2[1]) && (v2[1] == v2[2]) && (v2[2] == v2[3]) && (v2[3] == v2[4]) && (v2[4] == v2[5]) && (v2[5] == 200));
});

TEST("Vector::Vector(Vector &&)", []
{
	Vector<TYPE> v1(5, 300);
	Vector<TYPE> v2(std::move(v1));

	assert(v2.capacity() >= Vector<TYPE>::minimumSize && v2.capacity() >= 5);
	assert(v2.data() != nullptr);
	assert(v2.size() == 5);
	assert((v2[0] == v2[1]) && (v2[1] == v2[2]) && (v2[2] == v2[3]) && (v2[3] == v2[4]) && (v2[4] == 300));

	// capacity freeing is up to the implementation
	// assert(v1.capacity() == 0);
	// assert(v1.data() == nullptr);
	assert(v1.size() == 0);
});

TEST("Vector::Vector(std::initializer_list)", []
{
	Vector<TYPE> v({7, 6, 5, 4, 3, 2, 1});

	assert(v.capacity() >= Vector<TYPE>::minimumSize && v.capacity() >= 7);
	assert(v.data() != nullptr);
	assert(v.size() == 7);
});

TEST("Vector::~Vector()", []
{
	Vector<TYPE> v({1, 2, 3, 4, 5});
	assert(v.size() == 5);
	assert(v.capacity() >= v.size());
	assert(v.data() != nullptr);

	v.~Vector<TYPE>();
	assert(v.size() == 0);
	assert(v.capacity() == 0);
	assert(v.data() == nullptr);
});