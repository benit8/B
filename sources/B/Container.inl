/*
** B, 2019
** Container.inl
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

template <typename T> const usize Container<T>::max = -1;
template <typename T> const usize Container<T>::maxSize = max / sizeof(T);
template <typename T> const usize Container<T>::minimumSize = 8;
template <typename T> const usize Container<T>::growthFactor = 2;

////////////////////////////////////////////////////////////////////////////////

template <typename T>
Container<T>::Container()
: m_data(nullptr)
, m_capacity(0)
, m_size(0)
{
}

template <typename T>
Container<T>::~Container()
{
	reset();
}

////////////////////////////////////////////////////////////

template <typename T>
T *Container<T>::data()
{
	return m_data;
}

template <typename T>
const T *Container<T>::data() const
{
	return m_data;
}

template <typename T>
usize Container<T>::size() const
{
	return m_size;
}

template <typename T>
usize Container<T>::capacity() const
{
	return m_capacity;
}

template <typename T>
bool Container<T>::null() const
{
	return data() == nullptr;
}

template <typename T>
bool Container<T>::empty() const
{
	return null() || size() == 0;
}

////////////////////////////////////////////////////////////

template <typename T>
void Container<T>::clear()
{
	if (!null()) {
		if constexpr (!std::is_trivial<T>::value) {
			for (usize i = 0; i < size(); ++i)
				slot(i)->~T();
		}
	}
	m_size = 0;
}

template <typename T>
void Container<T>::reset()
{
	if (!null()) {
		if constexpr (!std::is_trivial<T>::value) {
			for (usize i = 0; i < size(); ++i)
				slot(i)->~T();
		}
		delete[] m_data;
		m_data = nullptr;
	}
	m_size = m_capacity = 0;
}

template <typename T>
void Container<T>::reserve(usize newSize)
{
	if (newSize <= capacity() && capacity() > 0)
		return;
	newSize = recommend(newSize);
	T *newData = new T[newSize];
	assert(newData != nullptr);
	if (!null()) {
		Type<T>::move(newData, m_data, size());
		delete[] m_data;
	}
	m_data = newData;
	m_capacity = newSize;
}

template <typename T>
void Container<T>::resize(usize newSize, const T &filler)
{
	if (newSize == size())
		return;
	if (newSize > capacity())
		reserve(newSize);
	if (newSize > size())
		Type<T>::set(&m_data[size()], filler, newSize - size());
	m_size = newSize;
}

template <typename T>
void Container<T>::shrink()
{
	if (empty() || size() == capacity())
		return;
	T *newData = new T[size()];
	Type<T>::move(newData, m_data, size());
	delete[] m_data;
	m_data = newData;
	m_capacity = size();
}

////////////////////////////////////////////////////////////

template <typename T>
usize Container<T>::recommend(usize size) const
{
	// Round up to the nearest power
	auto roundUp = [] (usize n, usize p = 8) {
		return ((n + p - 1) / p) * p;
	};

	if (empty())
		return B::max(minimumSize, roundUp(size));
	if (size < minimumSize)
		return minimumSize;
	if (size > maxSize)
		throw std::length_error(format("Cannot allocate for more than %$ elements.", maxSize));
	if (size >= maxSize / 2)
		return maxSize;
	return roundUp(B::max(capacity() * growthFactor, size));
}

template <typename T>
const T *Container<T>::slot(usize i) const
{
	if (i >= capacity())
		throw std::out_of_range(format("Tried to access out of range slot: %$/%$", i, capacity()));
	return &m_data[i];
}

template <typename T>
T *Container<T>::slot(usize i)
{
	if (i >= capacity())
		throw std::out_of_range(format("Tried to access out of range slot: %$/%$", i, capacity()));
	return &m_data[i];
}

////////////////////////////////////////////////////////////////////////////////

}