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

#include "B/Container.hpp"
#include "B/Common.hpp"
#include "B/IO/Print.hpp"

#include <stdexcept>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

template <typename T>
class Sequence : public Container<T>
{
public:
	virtual ~Sequence();

	virtual T *data();
	virtual const T *data() const;
	virtual usize capacity() const;
	virtual usize size() const override;
	virtual bool empty() const override;
	virtual bool null() const;
	virtual void clear() override;
	virtual void reset();
	virtual void reserve(usize newSize);
	virtual void resize(usize newSize, const T &filler = T());
	virtual void shrink();

	virtual T &at(usize i);
	virtual const T &at(usize i) const;
	virtual T &operator [](usize i) { return at(i); }
	virtual const T &operator [](usize i) const { return at(i); }

protected:
	Sequence();

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

////////////////////////////////////////////////////////////////////////////////

}

////////////////////////////////////////////////////////////////////////////////

#include "Sequence.inl"