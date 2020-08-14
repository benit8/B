/*
** B, 2019
** Map.inl
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

template <typename Key, typename Value>
Map<Key, Value>::Map()
{
}

template <typename Key, typename Value>
Map<Key, Value>::Map(const Map& other)
{
	this->reserve(other.size());
	for (auto it = other.begin(); !it.isEnd(); ++it)
		this->insert(it->key, it->value);
}

template <typename Key, typename Value>
Map<Key, Value>::Map(Map&& other)
{
	this->m_data = std::move(other.m_data);
	this->m_capacity = std::move(other.m_capacity);
	this->m_size = std::move(other.m_size);

	other.reset();
}

template <typename Key, typename Value>
Map<Key, Value>::Map(std::initializer_list<Pair<Key, Value>> il)
{
	this->reserve(il.size());
	for (auto it = il.begin(); it != il.end(); ++it)
		this->insert(it->key, it->value);
}

////////////////////////////////////////////////////////////

template <typename Key, typename Value>
typename Map<Key, Value>::Iterator Map<Key, Value>::begin()
{
	return Map::Iterator(*this, 0);
}

template <typename Key, typename Value>
typename Map<Key, Value>::ConstIterator Map<Key, Value>::begin() const
{
	return Map::ConstIterator(*this, 0);
}

template <typename Key, typename Value>
typename Map<Key, Value>::Iterator Map<Key, Value>::end()
{
	return Map::Iterator(*this, this->size());
}

template <typename Key, typename Value>
typename Map<Key, Value>::ConstIterator Map<Key, Value>::end() const
{
	return Map::ConstIterator(*this, this->size());
}

////////////////////////////////////////////////////////////

template <typename Key, typename Value>
bool Map<Key, Value>::contains(const Key& key) const
{
	auto it = find(key);
	return !it.isEnd();
}

template <typename Key, typename Value>
bool Map<Key, Value>::insert(const Key& key, const Value& value)
{
	auto it = find(key);
	if (!it.isEnd())
		return false;

	usize pos = it.index();
	this->reserve(this->size() + 1);
	if (!this->empty() && pos < this->size())
		Memory<Pair<Key, Value>>::move(this->slot(pos + 1), this->slot(pos), this->size() - pos);
	new (this->slot(pos)) Pair<Key, Value>(key, value);
	this->m_size++;
	return true;
}

template <typename Key, typename Value>
bool Map<Key, Value>::erase(const Key& key)
{
	auto it = find(key);
	if (it.isEnd())
		return false;

	usize pos = it.index();
	this->slot(pos)->~Pair<Key, Value>();
	Memory<Pair<Key, Value>>::move(this->slot(pos), this->slot(pos + 1), this->size() - pos - 1);
	this->m_size--;
	return true;
}

template <typename Key, typename Value>
typename Map<Key, Value>::Iterator Map<Key, Value>::find(const Key& key)
{
	usize index = -1;
	return Iterator(*this, search(key, index) ? index : this->size());
}

template <typename Key, typename Value>
typename Map<Key, Value>::ConstIterator Map<Key, Value>::find(const Key& key) const
{
	usize index = -1;
	return ConstIterator(*this, search(key, index) ? index : this->size());
}

////////////////////////////////////////////////////////////

template <typename Key, typename Value>
bool Map<Key, Value>::search(const Key& search, usize& index) const
{
	// Quick 'n easy checks
	if (this->empty() || search < this->slot(0)->key) {
		index = 0;
		return false;
	}
	else if (!this->empty() && search >= this->slot(this->size() - 1)->key) {
		index = this->size();
		return false;
	}

	usize lowerBound = 0;
	usize upperBound = this->size() - 1;

	for (index = -1; ; ) {
		if (upperBound - lowerBound <= 1)
			break;

		index = lowerBound + (upperBound - lowerBound) / 2;
		if (this->slot(index)->key <= search)
			lowerBound = index + 1;
		else
			upperBound = index;
	}

	bool found = this->slot(lowerBound)->key == search;
	index = search < this->slot(lowerBound)->key ? lowerBound : upperBound;
	return found;
}

template <typename Key, typename Value>
bool Map<Key, Value>::insert(usize pos, const Key& key, const Value& value)
{
	this->reserve(this->size() + 1);
	if (pos < this->size())
		Memory<Pair<Key, Value>>::move(this->slot(pos + 1), this->slot(pos), this->size() - pos);
	new (this->slot(pos)) Pair<Key, Value>(key, value);
	this->m_size++;
	return true;
}

////////////////////////////////////////////////////////////////////////////////

template <typename Key, typename Value>
std::ostream& operator <<(std::ostream& os, const Map<Key, Value>& m)
{
	os << "Map<" << typeid(Key).name() << ", " << typeid(Value).name() << ">(" << m.size() << '/' << m.capacity() << ") ";
	if (m.null())
		return os << "null";

	os << '{';
	for (auto pair : m) {
		os << " (" << pair.key << ':' << pair.value << ')';
	}
	return os << " }";
}

////////////////////////////////////////////////////////////////////////////////

}