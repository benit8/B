/*
** B, 2019
** Associative.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	template <typename K, typename V>
	class Associative;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/Containers/Sequence.hpp"
#include "B/Containers/Pair.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

template <typename K, typename V>
class Associative : public Sequence<Pair<K, V>>
{
public:
	virtual V& at(const K& key)
	{
		usize index = -1;
		if (!this->search(key, index))
			this->insertAt(index, key, V());
		return this->slot(index)->value;
	}

	virtual const V& at(const K& key) const
	{
		usize index = -1;
		if (!this->search(key, index))
			throw std::out_of_range("Tried to access inexistant element");
		return this->slot(index)->value;
	}

	// V& operator [](const K& key) { return at(key); }
	// const V& operator [](const K& key) const { return at(key); }

	virtual bool contains(const K& key) const = 0;
	virtual bool insert(const K& key, const V& value) = 0;
	virtual bool erase(const K& key) = 0;

protected:
	virtual bool search(const K& search, usize& index) const = 0;
	virtual bool insertAt(usize pos, const K& key, const V& value) = 0;
};

////////////////////////////////////////////////////////////////////////////////

}