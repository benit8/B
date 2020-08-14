/*
** B, 2020
** SingleList.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "B/Function.hpp"
#include "B/Containers/Container.hpp"
#include "B/Containers/Pair.hpp"
#include "B/IO/Writer.hpp"

#include <initializer_list>
#include <functional>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

template <typename L, typename T>
class SingleListIterator
{
private:
	explicit SingleListIterator(typename L::Node* node, typename L::Node* prev = nullptr)
	: m_node(node)
	, m_prev(prev)
	{
		if (prev != nullptr)
			assert(prev->next == node);
	}

public:
	bool isEnd() const { return m_node == nullptr; }

	T& operator *() { return m_node->value; }
	const T& operator *() const { return m_node->value; }

	T* operator ->() { return &m_node->value; }
	const T* operator ->() const { return &m_node->value; }

	SingleListIterator& operator ++()
	{
		m_prev = m_node;
		m_node = m_node->next;
		return *this;
	}

	SingleListIterator& operator ++(int)
	{
		m_prev = m_node;
		m_node = m_node->next;
		return *this;
	}

	bool operator ==(const SingleListIterator& other) const { return m_node == other.m_node; }
	bool operator !=(const SingleListIterator& other) const { return m_node != other.m_node; }

private:
	friend L;

	typename L::Node* m_node = nullptr;
	typename L::Node* m_prev = nullptr;
};

////////////////////////////////////////////////////////////////////////////////

template <typename T>
class SingleList : public Container<T>
{
private:
	struct Node
	{
		T value;
		Node* next = nullptr;

		explicit Node(const T& v)
		: value(v)
		{}

		explicit Node(T&& v)
		: value(std::move(v))
		{}

		~Node()
		{
			value.~T();
		}
	};

public:
	using Iterator = SingleListIterator<SingleList, T>;
	using ConstIterator = SingleListIterator<const SingleList, const T>;

public:
	SingleList() = default;
	SingleList(const SingleList& other);
	SingleList(SingleList&& other);
	template <typename It>
	SingleList(It first, It last);
	explicit SingleList(std::initializer_list<T> il);
	~SingleList();

	/// Capacity
	size_t size() const override;
	bool empty() const override;
	void clear() override;

	/// Element Access
	T& first() { assert(m_head); return m_head->value; }
	const T& first() const { assert(m_head); return m_head->value; }

	Iterator begin() { return Iterator(m_head); }
	Iterator end() { return Iterator(nullptr); }
	ConstIterator begin() const { return ConstIterator(m_head); }
	ConstIterator end() const { return ConstIterator(nullptr); }

	/// Operations
	template <typename It>
	void assign(It first, It last);
	void assign(std::initializer_list<T> il);

	void push(const T& value);
	void push(T&& value);

	template <typename... Args>
	void emplace(Args&&... args)
	{
		insert(T(std::forward<Args>(args)...));
	}

	void pop();

	Iterator insertAfter(Iterator it, const T& value);
	Iterator insertAfter(Iterator it, T&& value);

	template <typename... Args>
	Iterator emplaceAfter(const Iterator it, Args&&... args)
	{
		return insertAfter(it, T(std::forward<Args>(args)...));
	}

	void erase(Iterator it);
	void erase(Iterator first, Iterator last);

	size_t remove(const T& value);

	template <typename F /* bool(const T&) */>
	size_t removeIf(F filter);

	size_t unique();

	SingleList splice(Iterator it);

	void reverse();

	void sort();
	void sort(Function<bool(const T&, const T&)> comparator);

	bool contains(const T& value) const;

	Iterator find(const T& value);
	Iterator find(Function<bool(const T&)> finder);

	ConstIterator find(const T& value) const;
	ConstIterator find(Function<bool(const T&)> finder) const;

	/// Operators
	SingleList& operator =(const SingleList& other);
	SingleList& operator =(SingleList&& other);
	SingleList& operator =(std::initializer_list<T> il);

	bool operator ==(const SingleList& other);
	bool operator ==(std::initializer_list<T> il);
	bool operator !=(const SingleList& other);
	bool operator !=(std::initializer_list<T> il);

	friend Writer& operator <<<>(Writer& lhs, const SingleList& rhs);

private:
	Node* m_head = nullptr;

	friend Iterator;
	friend ConstIterator;
};

////////////////////////////////////////////////////////////////////////////////

}

////////////////////////////////////////////////////////////////////////////////

#include "SingleList.inl"