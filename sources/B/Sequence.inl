/*
** B, 2019
** Sequence.inl
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

template <typename T> const usize Sequence<T>::max = -1;
template <typename T> const usize Sequence<T>::maxSize = max / sizeof(T);
template <typename T> const usize Sequence<T>::minimumSize = 8;
template <typename T> const usize Sequence<T>::growthFactor = 2;

////////////////////////////////////////////////////////////////////////////////

template <typename T>
Sequence<T>::Sequence()
: m_data(nullptr)
, m_capacity(0)
, m_size(0)
{
}

template <typename T>
Sequence<T>::~Sequence()
{
	reset();
}

////////////////////////////////////////////////////////////

template <typename T>
T *Sequence<T>::data()
{
	return m_data;
}

template <typename T>
const T *Sequence<T>::data() const
{
	return m_data;
}

template <typename T>
usize Sequence<T>::size() const
{
	return m_size;
}

template <typename T>
usize Sequence<T>::capacity() const
{
	return m_capacity;
}

template <typename T>
bool Sequence<T>::null() const
{
	return data() == nullptr;
}

template <typename T>
bool Sequence<T>::empty() const
{
	return null() || size() == 0;
}

////////////////////////////////////////////////////////////

template <typename T>
void Sequence<T>::clear()
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
void Sequence<T>::reset()
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
void Sequence<T>::reserve(usize newSize)
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
void Sequence<T>::resize(usize newSize, const T &filler)
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
void Sequence<T>::shrink()
{
	if (empty() || size() == capacity())
		return;
	T *newData = new T[size()];
	Type<T>::move(newData, m_data, size());
	delete[] m_data;
	m_data = newData;
	m_capacity = size();
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
T &Sequence<T>::at(usize i)
{
	if (i >= this->size())
		throw std::out_of_range(format("Tried to access out of range element: %$/%$", i, this->size()));
	return *this->slot(i);
}

template <typename T>
const T &Sequence<T>::at(usize i) const
{
	if (i >= this->size())
		throw std::out_of_range(format("Tried to access out of range element: %$/%$", i, this->size()));
	return *this->slot(i);
}

////////////////////////////////////////////////////////////

template <typename T>
const T *Sequence<T>::slot(usize i) const
{
	if (i >= capacity())
		throw std::out_of_range(format("Tried to access out of range slot: %$/%$", i, capacity()));
	return &m_data[i];
}

template <typename T>
T *Sequence<T>::slot(usize i)
{
	if (i >= capacity())
		throw std::out_of_range(format("Tried to access out of range slot: %$/%$", i, capacity()));
	return &m_data[i];
}

template <typename T>
usize Sequence<T>::recommend(usize size) const
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

////////////////////////////////////////////////////////////////////////////////

}