/*
** B, 2019
** Map.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	template <typename Key, typename Value>
	class Map;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/Memory.hpp"
#include "B/Containers/Associative.hpp"
#include "B/Containers/Iterators.hpp"
#include "B/Containers/Pair.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

template <typename Key, typename Value>
class Map : public Associative<Key, Value>
{
public:
	using Iterator = SequenceIterator<Map, Pair<Key, Value>>;
	using ConstIterator = SequenceIterator<const Map, const Pair<Key, Value>>;

public:
	Map();
	Map(const Map& other);
	Map(Map&& other);
	Map(std::initializer_list<Pair<Key, Value>> il);

	Iterator begin();
	Iterator end();
	ConstIterator begin() const;
	ConstIterator end() const;

	bool contains(const Key& key) const;
	bool insert(const Key& key, const Value& value);
	bool erase(const Key& key);
	Iterator find(const Key& key);
	ConstIterator find(const Key& key) const;

private:
	bool search(const Key& search, usize& index) const override;
	bool insert(usize pos, const Key& key, const Value& value);

	friend Iterator;
	friend ConstIterator;
};

}

////////////////////////////////////////////////////////////////////////////////

#include "Map.inl"