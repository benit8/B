/*
** B, 2019
** Container.cpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	template <typename T>
	class Container;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/Types.hpp"
#include "B/Common.hpp"
#include "B/IO/Print.hpp"

#include <stdexcept>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

template <typename T>
class Container
{
public:
	virtual ~Container();

	virtual T *data();
	virtual const T *data() const;
	virtual usize size() const;
	virtual usize capacity() const;
	virtual bool null() const;
	virtual bool empty() const;

	virtual void clear();
	virtual void reset();
	virtual void reserve(usize newSize);
	virtual void resize(usize newSize, const T &filler = T());
	virtual void shrink();

protected:
	Container();

	T *slot(usize i);
	const T *slot(usize i) const;

	virtual usize recommend(usize size) const;

public:
	static const usize max;
	static const usize maxSize;
	static const usize minimumSize;
	static const usize growthFactor;

protected:
	T *m_data = nullptr;
	usize m_capacity = 0;
	usize m_size = 0;
};

}

////////////////////////////////////////////////////////////////////////////////

#include "B/Container.inl"