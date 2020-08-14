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
{
	assign(other.begin(), other.end());
}

template <typename T>
SingleList<T>::SingleList(SingleList &&other)
{
	clear();
	std::swap(m_head, other.m_head);
}

template <typename T>
template <typename It>
SingleList<T>::SingleList(It first, It last)
{
	assign(first, last);
}

template <typename T>
SingleList<T>::SingleList(std::initializer_list<T> il)
{
	assign(il);
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
	for (auto *curr = m_head; curr != nullptr; curr = curr->next)
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
bool SingleList<T>::contains(const T &value) const
{
	return find(value) != end();
}

template <typename T>
void SingleList<T>::clear()
{
	for (Node *curr = m_head; curr != nullptr; ) {
		Node *next = curr->next;
		delete curr;
		curr = next;
	}
	m_head = nullptr;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
template <typename It>
void SingleList<T>::assign(It first, It last)
{
	clear();

	if (first == last)
		return;

	Node **ptrToCurrent = &m_head;
	for (auto it = first; it != last; ++it) {
		*ptrToCurrent = new Node(*it);
		ptrToCurrent = &(*ptrToCurrent)->next;
	}
}

template <typename T>
void SingleList<T>::assign(std::initializer_list<T> il)
{
	assign(il.begin(), il.end());
}

template <typename T>
void SingleList<T>::push(const T &value)
{
	auto *n = new Node(value);
	n->next = m_head;
	m_head = n;
}

template <typename T>
void SingleList<T>::push(T &&value)
{
	auto *n = new Node(std::move(value));
	n->next = m_head;
	m_head = n;
}

template <typename T>
void SingleList<T>::pop()
{
	if (empty())
		return;

	auto *second = m_head->next;
	delete m_head;
	m_head = second;
}

template <typename T>
typename SingleList<T>::Iterator SingleList<T>::insertAfter(SingleList<T>::Iterator it, const T &value)
{
	assert(!it.isEnd());

	auto *n = new Node(value);
	n->next = it.m_node->next;
	it.m_node->next = n;
	return Iterator(n, it.m_node);
}

template <typename T>
typename SingleList<T>::Iterator SingleList<T>::insertAfter(SingleList<T>::Iterator it, T &&value)
{
	assert(!it.isEnd());

	auto *n = new Node(std::move(value));
	n->next = it.m_node->next;
	it.m_node->next = n;
	return Iterator(n, it.m_node);
}

template <typename T>
void SingleList<T>::erase(SingleList<T>::Iterator it)
{
	assert(!it.isEnd());

	if (it.m_prev)
		it.m_prev->next = it.m_node->next;
	else
		m_head = it.m_node->next;

	delete it.m_node;
	it.m_node = nullptr;
}

template <typename T>
void SingleList<T>::erase(SingleList<T>::Iterator first, SingleList<T>::Iterator last)
{
	if (first == last)
		return;

	for (auto it = first; !it.isEnd() && it != last; ) {
		auto toErase = it;
		it++;
		delete toErase.m_node;
	}

	if (first.m_prev)
		first.m_prev->next = last.m_node;
	else
		m_head = last.m_node;
}

template <typename T>
size_t SingleList<T>::remove(const T &search)
{
	return removeIf([&search] (const T &value) {
		return value == search;
	});
}

template <typename T>
template <typename F>
size_t SingleList<T>::removeIf(F filter)
{
	size_t removed = 0;

	Node *prev = nullptr;
	Node *curr = m_head;
	while (curr != nullptr) {
		if (filter(curr->value)) {
			Node *toDel = curr;
			curr = curr->next;
			delete toDel;

			if (prev)
				prev->next = curr;
			else
				m_head = curr;

			removed++;
		}
		else {
			prev = curr;
			curr = curr->next;
		}
	}

	return removed;
}

template <typename T>
size_t SingleList<T>::unique()
{
	if (m_head && m_head->next == nullptr)
		return 0;

	size_t removed = 0;

	Node *prev = m_head;
	Node *curr = m_head->next;
	while (curr != nullptr) {
		if (prev->value == curr->value) {
			Node *toDel = curr;
			curr = curr->next;
			delete toDel;

			if (prev)
				prev->next = curr;
			else
				m_head = curr;

			removed++;
		}
		else {
			prev = curr;
			curr = curr->next;
		}
	}

	return removed;
}

template <typename T>
SingleList<T> SingleList<T>::splice(SingleList<T>::Iterator it)
{
	assert(!it.isEnd());

	SingleList<T> list;
	list.m_head = it.m_node;
	it.m_prev->next = nullptr;
	return list;
}

template <typename T>
void SingleList<T>::reverse()
{
	if (m_head && !m_head->next)
		return;

	Node *prev = nullptr;
	for (Node *curr = m_head; curr; ) {
		Node *next = curr->next;
		curr->next = prev;
		prev = curr;
		curr = next;
	}
	m_head = prev;
}

template <typename T>
void SingleList<T>::sort()
{
	sort([] (const T &a, const T &b) {
		return a < b;
	});
}

template <typename T>
void SingleList<T>::sort(Function<bool(const T&, const T&)> comparator)
{
	std::function<void(Node *&)> sortFromNode = [&] (Node *&head) -> void
	{
		auto splitInHalf = [] (Node *head) -> Pair<Node*, Node*>
		{
			Node *slow = head;
			Node *fast = head->next;
			while (fast) {
				fast = fast->next;
				if (fast) {
					slow = slow->next;
					fast = fast->next;
				}
			}
			auto head2 = slow->next;
			slow->next = nullptr;
			return {head, head2};
		};

		std::function<Node*(Node*, Node*)> sortedMerge = [&] (Node *a, Node *b) -> Node*
		{
			if (!a)
				return b;
			else if (!b)
				return a;

			if (comparator(a->value, b->value)) {
				a->next = sortedMerge(a->next, b);
				return a;
			}
			else {
				b->next = sortedMerge(a, b->next);
				return b;
			}
		};

		if (!head || !head->next)
			return;

		auto [a, b] = splitInHalf(head);
		sortFromNode(a);
		sortFromNode(b);
		head = sortedMerge(a, b);
	};

	sortFromNode(m_head);
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
typename SingleList<T>::Iterator SingleList<T>::find(const T &value)
{
	return find([&](auto &other) { return value == other; });
}

template <typename T>
typename SingleList<T>::Iterator SingleList<T>::find(Function<bool(const T&)> finder)
{
	Node *prev = nullptr;
	for (Node *curr = m_head; curr != nullptr; curr = curr->next) {
		if (finder(curr->value))
			return Iterator(curr, prev);
		prev = curr;
	}
	return end();
}

template <typename T>
typename SingleList<T>::ConstIterator SingleList<T>::find(const T &value) const
{
	return find([&](auto &other) { return value == other; });
}

template <typename T>
typename SingleList<T>::ConstIterator SingleList<T>::find(Function<bool(const T&)> finder) const
{
	Node *prev = nullptr;
	for (Node *curr = m_head; curr != nullptr; curr = curr->next) {
		if (finder(curr->value))
			return ConstIterator(curr, prev);
		prev = curr;
	}
	return end();
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
	std::swap(m_head, other.m_head);
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

	Node *a = m_head;
	Node *b = other.m_head;
	while (a != nullptr && b != nullptr) {
		if (a->value != b->value)
			return false;
		a = a->next;
		b = b->next;
	}
	return a == b;
}

template <typename T>
bool SingleList<T>::operator ==(std::initializer_list<T> il)
{
	if (empty() && il.size() == 0)
		return true;

	Node *node = m_head;
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
	if (empty() && other.empty())
		return false;

	Node *a = m_head;
	Node *b = other.m_head;
	while (a != nullptr && b != nullptr) {
		if (a->value != b->value)
			return true;
		a = a->next;
		b = b->next;
	}
	return a != b;
}

template <typename T>
bool SingleList<T>::operator !=(std::initializer_list<T> il)
{
	if (empty() && il.size() == 0)
		return false;

	auto a = begin();
	auto b = il.begin();
	while (!a.isEnd() && b != il.end()) {
		if (*a != *b)
			return true;
		a++;
		b++;
	}
	return a.isEnd() != (b == il.end());
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
Writer &operator <<(Writer &wrt, const SingleList<T> &l)
{
	wrt << "SingleList<" << typeid(T).name() << ">{";

	size_t len = 0;
	for (auto it = l.begin(); !it.isEnd(); len++) {
		wrt << *it;
		it++;
		if (!it.isEnd())
			wrt.put(',');
	}

	return wrt << '}' << '(' << len << ')';
}

////////////////////////////////////////////////////////////////////////////////

}