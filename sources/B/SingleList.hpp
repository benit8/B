/*
** B, 2020
** SingleList.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "Container.hpp"
#include "Iterators.hpp"

#include <initializer_list>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

template <typename L, typename T>
class ListIterator
{
public:
	ListIterator() = default;

private:
	explicit ListIterator(typename L::Node *node, typename L::Node *prev = nullptr)
	: m_node(node)
	, m_prev(prev)
	{
		if (prev != nullptr)
			assert(prev->next == node);
	}

	ListIterator &operator ++()
	{
		m_prev = m_node;
		m_node = m_node->next;
		return *this;
	}

	bool operator ==(ListIterator &other) const { return m_node == other.m_node; }
	bool operator !=(ListIterator &other) const { return m_node != other.m_node; }

	T &operator *() { return m_node->value; }
	T *operator ->() { return &m_node->value; }

	bool isEnd() const { return m_node == nullptr; }

private:
	friend L;

	typename L::Node *m_node = nullptr;
	typename L::Node *m_prev = nullptr;
};

////////////////////////////////////////////////////////////////////////////////

template <typename T>
class SingleList : public Container<T>
{
	struct Node
	{
		explicit Node(const T &v)
		: value(v)
		{}

		explicit Node(T &&v)
		: value(std::move(v))
		{}

		T value;
		Node *next = nullptr;
	};

public:
	using Iterator = ListIterator<SingleList, T>;
	using ConstIterator = ListIterator<const SingleList, const T>;

public:
	SingleList() = default;
	SingleList(const SingleList &list);
	SingleList(SingleList &&list);
	template <typename IT>
	SingleList(IT first, IT last);
	explicit SingleList(std::initializer_list<T> il);
	~SingleList();

	size_t size() const override;
	bool empty() const override;

	T &first() { return *m_head; }
	const T &first() const { return *m_head; }

	Iterator begin() { return Iterator(m_head); }
	Iterator end() { return Iterator(); }
	ConstIterator begin() const { return ConstIterator(m_head); }
	ConstIterator end() const { return ConstIterator(); }

	template <typename IT>
	void assign(IT first, IT last);
	void assign(std::initializer_list<T> il);
	void clear() override;
	void insert(const T &value);
	void insert(T &&value);
	void insertAfter(Iterator it, const T &value);
	void insertAfter(Iterator it, T &&value);
	void popFirst();
	void remove(const T &value);
	void remove(Iterator it);
	bool contains(const T &value) const;
	SingleList splice(Iterator it);
	void sort();

	template <typename F>
	Iterator find(F finder);
	Iterator find(const T &value);
	template <typename F>
	ConstIterator find(F finder) const;
	ConstIterator find(const T &value) const;

	SingleList &operator =(const SingleList &other);
	SingleList &operator =(SingleList &&other);
	SingleList &operator =(std::initializer_list<T> il);

	bool operator ==(const SingleList &other);
	bool operator ==(std::initializer_list<T> il);
	bool operator !=(const SingleList &other);
	bool operator !=(std::initializer_list<T> il);

private:
	friend Iterator;
	friend ConstIterator;

	Node *m_head = nullptr;
};

////////////////////////////////////////////////////////////////////////////////

}

////////////////////////////////////////////////////////////////////////////////

#include "SingleList.inl"