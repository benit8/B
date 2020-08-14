/*
** B, 2019
** Vector.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	template <typename T>
	class Vector;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/Function.hpp"
#include "B/Memory.hpp"
#include "B/Containers/Iterators.hpp"
#include "B/Containers/Sequence.hpp"

#include <initializer_list>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

template <typename T>
class Vector : public Sequence<T>
{
public:
	using Iterator = SequenceIterator<Vector, T>;
	using ConstIterator = SequenceIterator<const Vector, const T>;

public:
	Vector();
	Vector(const Vector& other);
	Vector(Vector&& other);
	Vector(const T* buffer, usize size);
	explicit Vector(usize size, const T& val = T());
	Vector(std::initializer_list<T> il);
	~Vector() = default;

	Iterator begin();
	Iterator end();
	ConstIterator begin() const;
	ConstIterator end() const;

	T& front() { return this->at(0); }
	T& back() { return this->at(this->size() - 1); }
	T* data() { return this->m_data; }
	const T& front() const { return this->at(0); }
	const T& back() const { return this->at(this->size() - 1); }
	const T* data() const { return this->m_data; }

	bool contains(const T& val) const;
	void assign(const Vector& other);
	void assign(Vector&& other);
	void assign(const T* buffer, usize size);
	void assign(usize n, const T& val);
	void assign(std::initializer_list<T> il);
	void append(const T& val, usize n = 1);
	void append(T&& val);
	void append(const Vector& other);
	void append(Vector&& other);
	void append(std::initializer_list<T> il);
	void prepend(const T& val, usize n = 1) { insert(0, val, n); }
	void prepend(T&& val) { insert(0, std::move(val)); }
	void prepend(const Vector& other) { insert(0, other); }
	void prepend(Vector&& other) { insert(0, std::move(other)); }
	void prepend(std::initializer_list<T> il) { insert(0, il); }
	void insert(usize pos, const T& val, usize n = 1);
	void insert(usize pos, T&& val);
	void insert(usize pos, const Vector& other);
	void insert(usize pos, Vector&& other);
	void insert(usize pos, std::initializer_list<T> il);
	template <typename... Args>
	void emplace(usize pos, Args&&... args);
	template <typename... Args>
	void emplaceFront(Args&&... args);
	template <typename... Args>
	void emplaceBack(Args&&... args);
	void erase(usize pos, usize l = 1);
	void shift();
	void pop();
	void apply(Function<void(T&)> f);
	void filter(Function<bool(const T&)> f);
	void sort();
	void sort(Function<bool(const T&, const T&)> cmp);
	void swap(Vector& other);
	void shuffle();
	void reverse();

	usize find(const T& val, usize pos = 0) const;
	usize find(Function<bool(const T&)> f, usize pos = 0) const;
	usize findLast(const T& val, usize pos = Sequence<T>::max) const;
	usize findLast(Function<bool(const T&)> f, usize pos = Sequence<T>::max) const;


	Vector& operator =(const Vector& other) { assign(other); return *this; }
	Vector& operator =(Vector&& other) { assign(std::move(other)); return *this; }
	Vector& operator =(std::initializer_list<T> il) { assign(il); return *this; }

	Vector& operator +=(const T& val) { append(val); return *this; }
	Vector& operator +=(T&& val) { append(std::move(val)); return *this; }
	Vector& operator +=(const Vector& other) { append(other); return *this; }
	Vector& operator +=(Vector&& other) { append(std::move(other)); return *this; }
	Vector& operator +=(std::initializer_list<T> il) { append(il); return *this; }

	T& operator [](usize i) { return data()[i]; }
	const T& operator [](usize i) const { return data()[i]; }

	bool operator ==(const Vector& other) const;
	bool operator ==(std::initializer_list<T> il) const;
	bool operator !=(const Vector& other) const;
	bool operator !=(std::initializer_list<T> il) const;

private:
	friend Iterator;
	friend ConstIterator;
};

}

////////////////////////////////////////////////////////////////////////////////

#include "Vector.inl"