/*
** B, 2019
** BufferStream.cpp
*/

#include "BufferStream.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

BufferStream::BufferStream(const byte *data, usize length, OpenMode mode)
: m_originalBuffer(Buffer::wrap(data, length))
, m_offset(0)
{
	// m_buffer.assign(data, length);
	this->open(mode);
}

BufferStream::BufferStream(const Buffer &buffer, OpenMode mode)
: m_originalBuffer(buffer)
, m_offset(0)
{
	// m_buffer.assign(m_originalBuffer.data(), m_originalBuffer.size());
	this->open(mode);
}

BufferStream::BufferStream(Buffer &&buffer, OpenMode mode)
: m_originalBuffer(std::move(buffer))
, m_offset(0)
{
	// m_buffer.assign(m_originalBuffer.data(), m_originalBuffer.size());
	this->open(mode);
}

BufferStream::BufferStream(String &string, OpenMode mode)
: m_originalBuffer(Buffer::copy((byte*)string.cStr(), string.length()))
, m_offset(0)
{
	// m_buffer.assign((byte*)string.cStr(), string.length());
	this->open(mode);
}

////////////////////////////////////////////////////////////////////////////////

bool BufferStream::canRead(usize n)
{
	if (!Stream::canRead(n))
		return false;
	if (m_originalBuffer.size() - m_offset < n)
		return false;
	return true;
}

bool BufferStream::canWrite(usize n)
{
	if (!Stream::canWrite(n))
		return false;
	if (m_originalBuffer.size() - m_offset < n)
		return false;
	return true;
}

usize BufferStream::read(byte *s, usize n)
{
	if (this->eof()) {
		m_state |= State::Fail;
		return 0;
	}
	else if (s == nullptr || n == 0) {
		return 0;
	}

	if (m_offset < m_originalBuffer.size()) {
		usize len = min(n, m_originalBuffer.size() - m_offset);
		std::memcpy(s, &m_originalBuffer[m_offset], len);
		if (len < n)
			m_state |= State::Eof;
		m_offset += len;
		return len;
	}
	return 0;
}

usize BufferStream::write(const byte *s, usize n)
{
	if (this->eof()) {
		m_state |= State::Fail;
		return 0;
	}
	else if (s == nullptr || n == 0) {
		return 0;
	}

	usize i = 0;
	for (; i < n && m_offset < m_originalBuffer.size(); ++i)
		m_originalBuffer[m_offset++] = s[i];
	return i;
}

bool BufferStream::seek(isize offset, SeekMode mode)
{
	switch (mode) {
		case SeekMode::Start:
			if (offset < 0 || (usize)offset >= m_originalBuffer.size())
				return false;
			m_offset = (usize)offset;
			break;
		case SeekMode::Current:
			if ((isize)m_offset + offset < 0 || m_offset + (usize)offset >= m_originalBuffer.size())
				return false;
			m_offset = (isize)m_offset + offset;
			break;
		case SeekMode::End:
			if (offset > 0 || offset < (isize)m_originalBuffer.size())
				return false;
			m_offset = (isize)m_originalBuffer.size() + offset;
			break;
	}

	if (m_offset == m_originalBuffer.size())
		m_state |= State::Eof;
	else
		m_state &= ~State::Eof;

	return true;
}

////////////////////////////////////////////////////////////////////////////////

}