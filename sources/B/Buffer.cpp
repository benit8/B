/*
** B, 2019
** Buffer.inl
*/

#include "Buffer.hpp"
#include "Memory.hpp"

#include <cassert>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

Buffer::Buffer()
: m_data(nullptr)
, m_size(0)
, m_owner(false)
{
}

Buffer::Buffer(usize n)
: m_data(new byte[n])
, m_size(n)
, m_owner(true)
{
	assert(!null());
	::memset(data(), 0, size());
}

Buffer::Buffer(const Buffer& other)
: m_data(new byte[other.m_size])
, m_size(other.m_size)
, m_owner(true)
{
	::memcpy(m_data, other.m_data, other.m_size);
}

Buffer::Buffer(Buffer&& other)
: m_data(std::move(other.m_data))
, m_size(other.m_size)
, m_owner(other.m_owner)
{
	other.m_data = nullptr;
	other.m_size = 0;
	other.m_owner = false;
}

Buffer::~Buffer()
{
	reset();
}

////////////////////////////////////////////////////////////////////////////////

Buffer Buffer::create(usize n)
{
	auto b = adopt(new byte[n], n);
	::memset(b.data(), 0, b.size());
	return b;
}

Buffer Buffer::adopt(byte* s, usize n)
{
	Buffer b;
	b.m_data = s;
	b.m_size = n;
	b.m_owner = true;
	return b;
}

Buffer Buffer::wrap(const byte* s, usize n)
{
	Buffer b;
	b.m_data = const_cast<byte*>(s);
	b.m_size = n;
	b.m_owner = false;
	return b;
}

Buffer Buffer::copy(const byte* s, usize n)
{
	Buffer b(n);
	::memcpy(b.data(), s, n);
	return b;
}

////////////////////////////////////////////////////////////////////////////////

void Buffer::reset()
{
	if (m_owner && !null()) {
		delete[] m_data;
		m_data = nullptr;
	}
	m_size = 0;
}

void Buffer::clear()
{
	::memset(data(), 0, size());
}

void Buffer::fill(byte c)
{
	::memset(data(), c, size());
}

Buffer Buffer::slice(usize start, usize length) const
{
	if (start >= size())
		return {};
	if (start + length >= size())
		length = size() - start;
	return Buffer::copy(data() + start, length);
}

usize Buffer::find(byte c) const
{
	for (usize i = 0; i < size(); ++i)
		if (data()[i] == c)
			return i;
	return -1;
}

////////////////////////////////////////////////////////////////////////////////

Buffer& Buffer::operator =(const Buffer& rhs)
{
	reset();

	m_data = new byte[rhs.m_size];
	m_size = rhs.m_size;
	m_owner = true;
	::memcpy(m_data, rhs.m_data, rhs.m_size);
	return *this;
}

Buffer& Buffer::operator =(Buffer&& rhs)
{
	reset();

	m_data = std::move(rhs.m_data);
	m_size = rhs.m_size;
	m_owner = rhs.m_owner;
	rhs.m_data = nullptr;
	rhs.m_size = 0;
	rhs.m_owner = false;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

}