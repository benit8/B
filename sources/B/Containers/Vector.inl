/*
** B, 2019
** Vector.inl
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

template <typename T>
Vector<T>::Vector()
{
	this->reserve(Sequence<T>::minimumSize);
}

template <typename T>
Vector<T>::Vector(const Vector<T> &other)
{
	this->assign(other);
}

template <typename T>
Vector<T>::Vector(Vector<T> &&other)
{
	this->assign(std::move(other));
}

template <typename T>
Vector<T>::Vector(const T *buffer, usize size)
{
	this->assign(buffer, size);
}

template <typename T>
Vector<T>::Vector(usize size, const T &val)
{
	this->assign(size, val);
}

template <typename T>
Vector<T>::Vector(std::initializer_list<T> il)
{
	this->assign(il);
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
typename Vector<T>::Iterator Vector<T>::begin()
{
	return Iterator(*this, 0);
}

template <typename T>
typename Vector<T>::Iterator Vector<T>::end()
{
	return Iterator(*this, this->size());
}

template <typename T>
typename Vector<T>::ConstIterator Vector<T>::begin() const
{
	return ConstIterator(*this, 0);
}

template <typename T>
typename Vector<T>::ConstIterator Vector<T>::end() const
{
	return ConstIterator(*this, this->size());
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool Vector<T>::contains(const T &val) const
{
	for (usize i = 0; i < this->size(); ++i) {
		if (this->at(i) == val)
			return true;
	}
	return false;
}

template <typename T>
void Vector<T>::assign(const Vector<T> &other)
{
	this->reserve(other.size());
	Type<T>::copy(this->slot(0), other.data(), other.size());
	this->m_size = other.size();
}

template <typename T>
void Vector<T>::assign(Vector<T> &&other)
{
	this->reset();

	this->m_data = std::move(other.data());
	this->m_capacity = std::move(other.capacity());
	this->m_size = std::move(other.size());

	other.m_data = nullptr;
	other.m_capacity = 0;
	other.m_size = 0;
}

template <typename T>
void Vector<T>::assign(usize size, const T &val)
{
	this->reserve(size);
	for (usize i = 0; i < size; ++i)
		new (this->slot(i)) T(val);
	this->m_size = size;
}

template <typename T>
void Vector<T>::assign(const T *buffer, usize size)
{
	this->reserve(size);
	Type<T>::copy(this->slot(0), buffer, size);
	this->m_size = size;
}

template <typename T>
void Vector<T>::assign(std::initializer_list<T> il)
{
	this->reserve(il.size());
	this->m_size = 0;
	for (auto i : il)
		Type<T>::copy(this->slot(this->m_size++), &i, 1);
}

template <typename T>
void Vector<T>::append(const T &val, usize n)
{
	if (this->size() + n > this->capacity())
		this->reserve(this->size() + n);

	for (usize i = 0; i < n; ++i)
		new (this->slot(this->size() + i)) T(val);

	this->m_size += n;
}

template <typename T>
void Vector<T>::append(T &&val)
{
	if (this->size() == this->capacity())
		this->reserve(this->size() + 1);

	new (this->slot(this->m_size++)) T(std::move(val));
}

template <typename T>
void Vector<T>::append(const Vector<T> &other)
{
	const usize n = other.size();
	if (this->size() + n > this->capacity())
		this->reserve(this->size() + n);

	Type<T>::copy(this->slot(this->size()), other.data(), n);
	this->m_size += n;
}

template <typename T>
void Vector<T>::append(Vector<T> &&other)
{
	const usize n = other.size();
	if (this->size() + n > this->capacity())
		this->reserve(this->size() + n);

	Type<T>::move(this->slot(this->size()), other.data(), n);
	this->m_size += n;

	other.clear();
}

template <typename T>
void Vector<T>::append(std::initializer_list<T> il)
{
	const usize n = il.size();
	if (this->size() + n > this->capacity())
		this->reserve(this->size() + n);

	for (auto i : il)
		Type<T>::copy(this->slot(this->m_size++), &i, 1);
}

template <typename T>
void Vector<T>::insert(usize pos, const T &val, usize n)
{
	if (pos > this->size())
		return;
	else if (pos == this->size())
		return this->append(val, n);

	if (this->size() + n > this->capacity())
		this->reserve(this->size() + n);

	Type<T>::move(this->slot(pos + n), this->slot(pos), this->size() - pos);
	for (usize i = 0; i < n; ++i)
		new (this->slot(pos + i)) T(val);

	this->m_size += n;
}

template <typename T>
void Vector<T>::insert(usize pos, T &&val)
{
	if (pos > this->size())
		return;
	else if (pos == this->size())
		return this->append(std::move(val));

	if (this->size() + 1 > this->capacity())
		this->reserve(this->size() + 1);

	Type<T>::move(this->slot(pos + 1), this->slot(pos), this->size() - pos);
	new (this->slot(pos)) T(std::move(val));

	this->m_size++;
}

template <typename T>
void Vector<T>::insert(usize pos, const Vector &other)
{
	if (pos > this->size())
		return;
	else if (pos == this->size())
		return this->append(other);

	const usize n = other.size();
	if (this->size() + n > this->capacity())
		this->reserve(this->size() + n);

	Type<T>::move(this->slot(pos + n), this->slot(pos), this->size() - pos);
	Type<T>::copy(this->slot(pos), other.data(), n);

	this->m_size += n;
}

template <typename T>
void Vector<T>::insert(usize pos, Vector &&other)
{
	if (pos > this->size())
		return;
	else if (pos == this->size())
		return this->append(std::move(other));

	const usize n = other.size();
	if (this->size() + n > this->capacity())
		this->reserve(this->size() + n);

	Type<T>::move(this->slot(pos + n), this->slot(pos), this->size() - pos);
	Type<T>::move(this->slot(pos), other.data(), n);

	this->m_size += n;
	other.clear();
}

template <typename T>
void Vector<T>::insert(usize pos, std::initializer_list<T> il)
{
	if (pos > this->size())
		return;
	else if (pos == this->size())
		return this->append(il);

	const usize n = il.size();
	if (this->size() + n > this->capacity())
		this->reserve(this->size() + n);

	Type<T>::move(this->slot(pos + n), this->slot(pos), this->size() - pos);
	for (usize i = 0; i < n; ++i)
		new (this->slot(i + pos)) T(*(il.begin() + i));

	this->m_size += n;
}

template <typename T>
template <typename... Args>
void Vector<T>::emplace(usize pos, Args&&... args)
{
	this->insert(pos, T(std::forward<Args>(args)...));
}

template <typename T>
template <typename... Args>
void Vector<T>::emplaceFront(Args&&... args)
{
	this->prepend(T(std::forward<Args>(args)...));
}

template <typename T>
template <typename... Args>
void Vector<T>::emplaceBack(Args&&... args)
{
	this->append(T(std::forward<Args>(args)...));
}

template <typename T>
void Vector<T>::erase(usize pos, usize len)
{
	if (len == 0 || pos >= this->size())
		return;

	/// If range exceeds size, truncate it
	if (pos + len > this->size())
		len = this->size() - pos;

	/// Destruct elements
	for (usize i = 0; i < len; ++i)
		this->slot(pos + i)->~T();

	/// Move right part of mem, if range is not at the end
	if (pos + len < this->size())
		Type<T>::move(this->slot(pos), this->slot(pos + len), this->size() - len);

	this->m_size -= len;
}

template <typename T>
void Vector<T>::shift()
{
	this->erase(0, 1);
}

template <typename T>
void Vector<T>::pop()
{
	this->erase(this->size() - 1, 1);
}

template <typename T>
void Vector<T>::apply(std::function<void(T &)> callback)
{
	for (usize i = 0; i < this->size(); ++i)
		callback(this->at(i));
}

template <typename T>
void Vector<T>::filter(std::function<bool(const T &val)> callback)
{
	Vector<T> r;
	r.reserve(this->size());

	for (usize i = 0; i < this->size(); ++i) {
		if (callback(this->at(i)))
			r.append(std::move(this->at(i)));
		this->slot(i)->~T();
	}

	this->assign(std::move(r));
}

template <typename T>
void Vector<T>::sort()
{
	this->sort([] (const T &a, const T &b) {
		return a < b;
	});
}

template <typename T>
void Vector<T>::sort(std::function<bool(const T&, const T&)> cmp)
{
	std::function<void(usize, usize)> partition = [&] (usize start, usize end)
	{
		if (start >= end)
			return;

		usize size = end - start + 1;
		if (size <= 1)
			return;
		else if (size == 2) {
			if (cmp(this->m_data[end], this->m_data[start]))
				std::swap(this->m_data[start], this->m_data[end]);
			return;
		}

		usize i, j, p = start + size / 2;
		auto pivot = this->m_data[p];
		for (i = start, j = end; ; ++i, --j) {
			while (cmp(this->m_data[i], pivot)) ++i;
			while (cmp(pivot, this->m_data[j])) --j;

			if (i >= j)
				break;

			std::swap(this->m_data[i], this->m_data[j]);
		}

		partition(start, i - 1);
		partition(i, end);
	};

	partition(0, this->size() - 1);
}

template <typename T>
void Vector<T>::swap(Vector<T> &other)
{
	Vector<T> c(std::move(other));
	other = std::move(*this);
	*this = std::move(c);
}

template <typename T>
void Vector<T>::shuffle()
{
	for (usize i = this->size() - 1; i > 0; --i) {
		const usize j = rand() % (this->size() - 1);
		std::swap(this->m_data[i], this->m_data[j]);
	}
}

template <typename T>
void Vector<T>::reverse()
{
	for (usize i = 0, j = this->size() - 1; i < j; ++i, --j) {
		std::swap(this->m_data[i], this->m_data[j]);
	}
}

template <typename T>
usize Vector<T>::find(const T &val, usize pos) const
{
	if (pos >= this->size())
		return Sequence<T>::max;

	for (usize i = pos; i < this->size(); ++i) {
		if (this->at(i) == val)
			return i;
	}
	return Sequence<T>::max;
}

template <typename T>
usize Vector<T>::find(std::function<bool(const T &)> callback, usize pos) const
{
	if (pos >= this->size())
		return Sequence<T>::max;

	for (usize i = pos; i < this->size(); ++i) {
		if (callback(this->at(i)))
			return i;
	}
	return Sequence<T>::max;
}

template <typename T>
usize Vector<T>::findLast(const T &val, usize pos) const
{
	if (pos >= this->size())
		pos = this->size() - 1;

	usize i = pos;
	for (;; --i) {
		if (this->at(i) == val)
			return i;

		if (i == 0)
			break;
	}
	return Sequence<T>::max;
}

template <typename T>
usize Vector<T>::findLast(std::function<bool(const T &)> callback, usize pos) const
{
	if (pos >= this->size())
		pos = this->size() - 1;

	usize i = pos;
	for (;; --i) {
		if (callback(this->at(i)))
			return i;

		if (i == 0)
			break;
	}
	return Sequence<T>::max;
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool Vector<T>::operator ==(const Vector<T> &other) const
{
	if (this->size() != other.size())
		return false;
	for (usize i = 0; i < this->size(); ++i) {
		if (this->at(i) != other[i])
			return false;
	}
	return true;
}

template <typename T>
bool Vector<T>::operator ==(std::initializer_list<T> il) const
{
	if (this->size() != il.size())
		return false;
	for (usize i = 0; i < this->size(); ++i) {
		if (this->at(i) != *(il.begin() + i))
			return false;
	}
	return true;
}

template <typename T>
bool Vector<T>::operator !=(const Vector<T> &other) const
{
	return !(*this == other);
}

template <typename T>
bool Vector<T>::operator !=(std::initializer_list<T> il) const
{
	return !(*this == il);
}

////////////////////////////////////////////////////////////////////////////////

template <typename T>
std::ostream &operator <<(std::ostream &os, const Vector<T> &v)
{
	os << "Vector<" << typeid(T).name() << ">(" << v.size() << "/" << v.capacity() << ") ";
	if (v.null())
		return os << "null";

	os << '{';
	for (usize i = 0; i < v.size(); ++i) {
		if (i > 0)
			os << ", ";
		os << v[i];
	}
	return os << '}';
}

////////////////////////////////////////////////////////////////////////////////

}