/*
** B, 2019
** Sequence.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	template <typename T>
	class Sequence;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/Common.hpp"
#include "B/Memory.hpp"
#include "B/Containers/Container.hpp"
// #include "B/IO/Print.hpp"
#include "tinyformat.hpp"

#include <stdexcept>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

template <typename T>
class Sequence : public Container<T>
{
public:
	virtual ~Sequence()
	{
		reset();
	}

	virtual T* data() { return m_data; }
	virtual const T* data() const { return m_data; }
	virtual usize capacity() const { return m_capacity; }
	virtual usize size() const override { return m_size; }
	virtual bool null() const { return data() == nullptr; }
	virtual bool empty() const override { return null() || size() == 0; }

	virtual T& at(usize i)
	{
		if (i >= size())
			throw std::out_of_range(tfm::format("Sequence::at(): i(%zu) >= size(%zu)", i, size()));
		return data()[i];
	}

	virtual const T& at(usize i) const
	{
		if (i >= size())
			throw std::out_of_range(tfm::format("Sequence::at(): i(%zu) >= size(%zu)", i, size()));
		return data()[i];
	}

	virtual void clear() override
	{
		if (!null())
			Memory<T>::destruct(data(), size());
		m_size = 0;
	}

	virtual void reset()
	{
		if (!null()) {
			Memory<T>::destruct(data(), size());
			delete[] m_data;
			m_data = nullptr;
		}
		m_size = m_capacity = 0;
	}

	virtual void reserve(usize newSize)
	{
		if (newSize <= capacity() && capacity() > 0)
			return;
		newSize = recommend(newSize);
		T* newData = new T[newSize];
		assert(newData != nullptr);
		if (!null()) {
			Memory<T>::move(newData, data(), size());
			delete[] m_data;
		}
		m_data = newData;
		m_capacity = newSize;
	}

	virtual void resize(usize newSize, const T& filler = T())
	{
		if (newSize == size())
			return;
		if (newSize > capacity())
			reserve(newSize);
		if (newSize > size())
			Memory<T>::set(slot(size()), filler, newSize - size());
		else // newSize < size()
			Memory<T>::destruct(slot(newSize), size() - newSize);
		m_size = newSize;
	}

	virtual void shrink()
	{
		if (empty() || size() == capacity())
			return;
		T* newData = new T[size()];
		Memory<T>::move(newData, m_data, size());
		delete[] m_data;
		m_data = newData;
		m_capacity = size();
	}

protected:
	Sequence() = default;

	T* slot(usize i)
	{
		if (i >= capacity())
			throw std::out_of_range(tfm::format("Sequence::slot(): i(%zu) >= capacity(%zu)", i, capacity()));
		return &m_data[i];
	}

	const T* slot(usize i) const
	{
		if (i >= capacity())
			throw std::out_of_range(tfm::format("Sequence::slot(): i(%zu) >= capacity(%zu)", i, capacity()));
		return &m_data[i];
	}

	virtual usize recommend(usize size) const
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
			throw std::length_error(tfm::format("Cannot allocate for more than %zu elements.", maxSize));
		if (size >= maxSize / 2)
			return maxSize;
		return roundUp(B::max(capacity() * growthFactor, size));
	}

public:
	static const usize max;
	static const usize maxSize;
	static const usize minimumSize;
	static const usize growthFactor;

protected:
	T* m_data = nullptr;
	usize m_capacity = 0;
	usize m_size = 0;
};

template <typename T> const usize Sequence<T>::max = -1;
template <typename T> const usize Sequence<T>::maxSize = max / sizeof(T);
template <typename T> const usize Sequence<T>::minimumSize = 8;
template <typename T> const usize Sequence<T>::growthFactor = 2;

////////////////////////////////////////////////////////////////////////////////

}