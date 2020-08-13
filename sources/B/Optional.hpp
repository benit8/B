/*
** B, 2020
** Optional.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	template <typename T>
	class Optional;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/Types.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

template <typename T>
class alignas(T) Optional
{
public:
	Optional() = default;

	Optional(const T &value)
	: m_hasValue(true)
	{
		new (&m_storage) T(value);
	}

	template <typename U>
	Optional(const U &value)
	: m_hasValue(true)
	{
		new (&m_storage) T(value);
	}

	Optional(T &&value)
	: m_hasValue(true)
	{
		new (&m_storage) T(std::move(value));
	}

	Optional(Optional &&other)
	: m_hasValue(other.m_hasValue)
	{
		if (other.m_hasValue) {
			new (&m_storage) T(other.releaseValue());
		}
	}

	~Optional()
	{
		clear();
	}

	////////////////////////////////////////////////////////

	bool hasValue() const { return m_hasValue; }

	T &value()
	{
		assert(m_hasValue);
		return *reinterpret_cast<T*>(&m_storage);
	}

	const T &value() const
	{
		assert(m_hasValue);
		return *reinterpret_cast<const T*>(&m_storage);
	}

	T &valueOr(const T &otherwise) const
	{
		return m_hasValue ? value() : otherwise;
	}

	void clear()
	{
		if (m_hasValue) {
			value().~T();
			m_hasValue = false;
		}
	}

	T releaseValue()
	{
		assert(m_hasValue);
		T released = std::move(value());
		value().~T();
		m_hasValue = false;
		return released;
	}

	////////////////////////////////////////////////////////

	Optional &operator =(const Optional &other)
	{
		if (this != &other) {
			clear();
			m_hasValue = other.m_hasValue;
			if (other.m_hasValue)
				new (&m_storage) T(other.value());
		}
		return *this;
	}

	Optional &operator =(Optional &&other)
	{
		if (this == &other) {
			clear();
			m_hasValue = other.m_hasValue;
			if (other.m_hasValue)
				new (&m_storage) T(other.releaseValue());
		}
		return *this;
	}

private:
	u8 m_storage[sizeof(T)] { 0 };
	bool m_hasValue = false;
};

}