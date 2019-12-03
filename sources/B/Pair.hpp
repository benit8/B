/*
** B, 2019
** Pair.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B
{

template <typename Key, typename Value>
struct Pair
{
	Key key;
	Value value;

	Pair();
	Pair(const Key &k, const Value &v);
	Pair(const Pair &other) = default;
	Pair(Pair &&other) = default;
};


template <typename Key, typename Value>
Pair<Key, Value>::Pair()
: key()
, value()
{}

template <typename Key, typename Value>
Pair<Key, Value>::Pair(const Key &k, const Value &v)
: key(k)
, value(v)
{}

}