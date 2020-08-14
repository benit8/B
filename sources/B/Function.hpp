/*
** B, 2020
** Function.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	template <typename>
	class Function;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/Traits.hpp"

#include <cassert>
#include <memory>
#include <utility>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

/// Callable signature interface
template<typename Res, typename... Args>
class Callable
{
public:
	virtual ~Callable() = default;
	virtual Res call(Args... args) const = 0;
};

/// Function and lambda closure wrapper
template<typename T, typename Res, typename... Args>
class Closure final : public Callable<Res, Args...>
{
public:
	Closure(const T& handler)
	: m_handler(handler)
	{}

	Closure(const Closure& other)
	: m_handler(other.handler)
	{}

	Closure(Closure&& other)
	: m_handler(std::move(other.handler))
	{}

	Res call(Args... args) const final override
	{
		return m_handler(args...);
	}

private:
	const T m_handler;
};


/// Function, lambda, functor...
template <typename Res, typename... Args>
class Function<Res(Args...)>
{
public:
	Function() = default;
	Function(std::nullptr_t) {}

	Function(const Function& other)
	: m_callable(other.m_callable)
	{}

	Function(Function&& other)
	: m_callable(std::move(other.m_callable))
	{
		other = nullptr;
	}

	Function(Res(*function)(Args...))
	: m_callable(new Closure<Res(Args...), Res, Args...>(function))
	{}

	// Captured lambda specialization
	template<typename L>
	Function(const L& lambda)
	: m_callable(new Closure<decltype(lambda), Res, Args...>(lambda))
	{}

	////////////////////////////////////////////////////////

	Res operator()(Args... args) const
	{
		assert(m_callable != nullptr);
		return m_callable->call(std::forward<Args>(args)...);
	}

	////////////////////////////////////////////////////////

	Function& operator =(const Function& other)
	{
		m_callable = other.m_callable;
		return *this;
	}

	Function& operator =(Function&& other)
	{
		if (other.m_callable) {
			m_callable = std::move(other.m_callable);
			other.m_callable = nullptr;
		}
		return *this;
	}

	Function& operator =(std::nullptr_t)
	{
		m_callable = nullptr;
		return *this;
	}

private:
	Callable<Res, Args...>* m_callable = nullptr;
};

}