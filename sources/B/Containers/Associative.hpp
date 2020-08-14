/*
** B, 2019
** Associative.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	template <typename Key, typename Value>
	class Associative;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/Containers/Sequence.hpp"
#include "B/Containers/Pair.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

template <typename Key, typename Value>
class Associative : public Sequence<Pair<Key, Value>>
{
public:
	virtual Value& at(const Key& key)
	{
		usize index = -1;
		if (!this->search(key, index))
			this->insertAt(index, key, Value());
		return this->slot(index)->value;
	}

	virtual const Value& at(const Key& key) const
	{
		usize index = -1;
		if (!this->search(key, index))
			throw std::out_of_range("Tried to access inexistant element");
		return this->slot(index)->value;
	}

	// Value& operator [](const Key& key) { return at(key); }
	// const Value& operator [](const Key& key) const { return at(key); }

	virtual bool contains(const Key& key) const = 0;
	virtual bool insert(const Key& key, const Value& value) = 0;
	virtual bool erase(const Key& key) = 0;

protected:
	virtual bool search(const Key& search, usize& index) const = 0;
	virtual bool insertAt(usize pos, const Key& key, const Value& value) = 0;
};

////////////////////////////////////////////////////////////////////////////////

}