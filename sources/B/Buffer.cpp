/*
** B, 2019
** Buffer.inl
*/

#include "Buffer.hpp"

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
	std::memset(data(), 0, size());
}

Buffer::Buffer(const Buffer &other)
: m_data(new byte[other.m_size])
, m_size(other.m_size)
, m_owner(true)
{
	std::memcpy(m_data, other.m_data, other.m_size);
}

Buffer::Buffer(Buffer &&other)
: m_data(std::move(other.m_data))
, m_size(other.m_size)
, m_owner(true)
{
	other.m_data = nullptr;
	other.m_size = 0;
	other.m_owner = false;
}

Buffer::~Buffer()
{
	if (m_owner && !null()) {
		delete[] m_data;
		m_data = nullptr;
	}
	m_size = 0;
}

////////////////////////////////////////////////////////////////////////////////

Buffer Buffer::create(usize n)
{
	auto b = adopt(new byte[n], n);
	std::memset(b.data(), 0, b.size());
	return b;
}

Buffer Buffer::adopt(byte *s, usize n)
{
	Buffer b;
	b.m_data = s;
	b.m_size = n;
	b.m_owner = true;
	return b;
}

Buffer Buffer::wrap(const byte *s, usize n)
{
	Buffer b;
	b.m_data = const_cast<byte*>(s);
	b.m_size = n;
	b.m_owner = false;
	return b;
}

Buffer Buffer::copy(const byte *s, usize n)
{
	Buffer b(n);
	std::memcpy(b.data(), s, n);
	return b;
}

////////////////////////////////////////////////////////////////////////////////

void Buffer::fill(byte c)
{
	std::memset(data(), c, size());
}

void Buffer::clear()
{
	std::memset(data(), 0, size());
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

}