TEST("Vector::find(const T &, size_t = 0)", []
{
	Vector<TYPE> v = {1,0,1,1,0,1,0,0,1,1,1,0,0,0,1,0,1,1,1,0,0};
	Vector<size_t> pos = {0,2,3,5,8,9,10,14,16,17,18};

	size_t i = 0;
	for (size_t p = 0; (p = v.find({1}, p)) != Vector<TYPE>::max; ++p) {
		assert(p == pos[i++]);
	}

	assert(i == pos.size());
});

TEST("Vector::find(std::function<bool(const T &)>, size_t = 0)", []
{
	Vector<TYPE> v = {2,29,21,10,30,14,20,25,6,7,3,24,17,9,11,19,1,15,8,26,5,12,22,16,18,28,4,23,27,13};
	Vector<size_t> pos = {1,2,7,9,10,12,13,14,15,16,17,20,27,28,29};

	auto f = [](const TYPE &t) -> bool {
		return t.n() & 1;
	};

	size_t i = 0;
	for (size_t p = 0; (p = v.find(f, p)) != Vector<TYPE>::max; ++p) {
		assert(p == pos[i++]);
	}

	assert(i == pos.size());
});

TEST("Vector::findLast(const T &, size_t = max)", []
{
	Vector<TYPE> v = {1,0,1,1,0,1,0,0,1,1,1,0,0,0,1,0,1,1,1,0,0};
	Vector<size_t> pos = {18,17,16,14,10,9,8,5,3,2,0};

	size_t i = 0;
	for (size_t p = v.size() - 1; (p = v.findLast({1}, p)) != Vector<TYPE>::max; --p) {
		assert(p == pos[i++]);
		if (p == 0) break;
	}

	assert(i == pos.size());
});

TEST("Vector::findLast(std::function<bool(const T &)>, size_t = max)", []
{
	Vector<TYPE> v = {2,29,21,10,30,14,20,25,6,7,3,24,17,9,11,19,1,15,8,26,5,12,22,16,18,28,4,23,27,13};
	Vector<size_t> pos = {29,28,27,20,17,16,15,14,13,12,10,9,7,2,1};

	auto f = [](const TYPE &t) -> bool {
		return t.n() & 1;
	};

	size_t i = 0;
	for (size_t p = v.size() - 1; (p = v.findLast(f, p)) != Vector<TYPE>::max; --p) {
		assert(p == pos[i++]);
	}

	assert(i == pos.size());
});