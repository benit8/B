/*
** B, 2020
** Result.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	template <typename V, typename E>
	class Result;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/Optional.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

template <typename V, typename E>
class Result
{
public:
	Result(const V &value)
	: m_value(value)
	{}

	Result(const E &error)
	: m_error(error)
	{}

	Result(const V &value, const E &error)
	: m_value(value)
	, m_error(error)
	{}

	Result(const Result &other)
	: m_value(other.value)
	, m_error(other.error)
	{}

	Result(Result &&other)
	: m_value(std::move(other.value))
	, m_error(std::move(other.error))
	{}

	~Result() = default;

	V &value() { return m_value.value(); }
	E &error() { return m_value.error(); }

	bool good() const { return !m_error.hasValue(); }
	explicit operator bool() const { return !m_error.hasValue(); }

private:
	Optional<V> m_value;
	Optional<E> m_error;
};

}