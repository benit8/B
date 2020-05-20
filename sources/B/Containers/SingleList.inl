/*
** B, 2020
** SingleList.inl
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

template <typename T>
SingleList<T>::SingleList(const SingleList &other)
: SingleList(other.begin(), other.end())
{}

template <typename T>
SingleList<T>::SingleList(SingleList &&other)
{
	*this = std::move(other);
}

template <typename T>
template <typename IT>
SingleList<T>::SingleList(IT first, IT last)
{
	assign(first, last);
}

template <typename T>
SingleList<T>::SingleList(std::initializer_list<T> il)
{
}

template <typename T>
SingleList<T>::~SingleList()
{
	clear();
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
size_t SingleList<T>::size() const
{
	size_t s = 0;
	for (auto *curr = m_head; curr; curr = curr->next)
		++s;
	return s;
}

template <typename T>
bool SingleList<T>::empty() const
{
	return m_head == nullptr;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
template <typename IT>
void SingleList<T>::assign(IT first, IT last)
{
	clear();

	if (first == last)
		return;

	Node **curr = &m_head;
	for (auto &it = first; it != last; ++it) {
		*curr = new Node(*it);
		curr = &curr->next;
	}
}

template <typename T>
void SingleList<T>::assign(std::initializer_list<T> il)
{
	clear();

	if (il.size() == 0)
		return;

	Node **curr = &m_head;
	for (auto el : il) {
		*curr = new Node(el);
		curr = &curr->next;
	}
}

template <typename T>
void SingleList<T>::clear()
{
	for (auto *curr = m_head; curr != nullptr; ) {
		auto *next = curr->next;
		delete curr;
		curr = next;
	}
	m_head = nullptr;
}

template <typename T>
void SingleList<T>::insert(const T &value)
{
	auto *n = new Node(value);
	n->next = m_head;
	m_head = n;
}

template <typename T>
void SingleList<T>::insert(T &&value)
{
	auto *n = new Node(std::move(value));
	n->next = m_head;
	m_head = n;
}

template <typename T>
void SingleList<T>::insertAfter(Iterator it, const T &value)
{
	auto *n = new Node(value);
	if (!it.isEnd())
		n->next = it.m_node->next;
	it.m_node->next = n;
}

template <typename T>
void SingleList<T>::insertAfter(Iterator it, T &&value)
{
	auto *n = new Node(std::move(value));
	if (!it.isEnd())
		n->next = it.m_node->next;
	it.m_node->next = n;
}

template <typename T>
void SingleList<T>::popFirst()
{
	if (empty())
		return;

	auto *second = m_head->next;
	delete m_head;
	m_head = second;
}

template <typename T>
void SingleList<T>::remove(const T &value)
{
	auto *prev = nullptr;
	auto *curr = m_head;
	while (curr != nullptr) {
		if (curr->value == value) {
			auto *toDel = curr;
			curr = curr->next;
			delete toDel;

			if (prev)
				prev->next = curr;
			else
				m_head = curr;
		}
		else {
			curr = curr->next;
		}
		prev = curr;
	}
}

template <typename T>
void SingleList<T>::remove(Iterator it)
{
	assert(!it.isEnd());

	if (it.m_node == m_head)
		m_head = it.m_node->next;
	if (it.m_prev)
		it.m_prev->next = it.m_node->next;
	delete it.m_node;
}

template <typename T>
bool SingleList<T>::contains(const T &value) const
{
	return find(value) != end();
}

template <typename T>
SingleList<T> SingleList<T>::splice(Iterator it)
{
	assert(!it.isEnd());

	SingleList<T> list;
	list.m_head = it.m_node;
	it.m_prev->next = nullptr;
	return list;
}

template <typename T>
void SingleList<T>::sort()
{
	/// TODO
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
template <typename F>
typename SingleList<T>::Iterator SingleList<T>::find(F finder)
{
	auto *prev = nullptr;
	for (auto *curr = m_head; curr != nullptr; curr = curr->next) {
		if (finder(curr->value))
			return Iterator(curr, prev);
		prev = curr;
	}
	return end();
}

template <typename T>
typename SingleList<T>::Iterator SingleList<T>::find(const T &value)
{
	return find([&](auto &other) { return value == other; });
}

template <typename T>
template <typename F>
typename SingleList<T>::ConstIterator SingleList<T>::find(F finder) const
{
	auto *prev = nullptr;
	for (auto *curr = m_head; curr != nullptr; curr = curr->next) {
		if (finder(curr->value))
			return ConstIterator(curr, prev);
		prev = curr;
	}
	return end();
}

template <typename T>
typename SingleList<T>::ConstIterator SingleList<T>::find(const T &value) const
{
	return find([&](auto &other) { return value == other; });
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
SingleList<T> &SingleList<T>::operator =(const SingleList &other)
{
	assign(other.begin(), other.end());
	return *this;
}

template <typename T>
SingleList<T> &SingleList<T>::operator =(SingleList &&other)
{
	clear();
	m_head = std::move(other.m_head);
	other.m_head = nullptr;
	return *this;
}

template <typename T>
SingleList<T> &SingleList<T>::operator =(std::initializer_list<T> il)
{
	assign(il);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool SingleList<T>::operator ==(const SingleList<T> &other)
{
	if (empty() && other.empty())
		return true;

	auto *a = m_head;
	auto *b = other.m_head;
	while (a != nullptr && b != nullptr) {
		if (a->value != b->value)
			return false;
		a = a->next;
		b = b->next;
	}
	return a != nullptr && b != nullptr && a->value == b->value;
}

template <typename T>
bool SingleList<T>::operator ==(std::initializer_list<T> il)
{
	if (empty() && il.size() == 0)
		return true;

	auto *node = m_head;
	for (auto el : il) {
		if (node == nullptr)
			return false;
		if (node->value != el)
			return false;
		node = node->next;
	}
	return node == nullptr;
}

template <typename T>
bool SingleList<T>::operator !=(const SingleList<T> &other)
{
	return !(*this == other);
}

template <typename T>
bool SingleList<T>::operator !=(std::initializer_list<T> il)
{
	return !(*this == il);
}

////////////////////////////////////////////////////////////////////////////////

}