/*
** B, 2020
** BufferReader.cpp
*/

#include "B/IO/Reader.hpp"

#include <unistd.h>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

BufferReader::BufferReader(const Buffer &buffer)
: m_buffer(buffer)
{}

BufferReader::BufferReader(StringView str)
{
	auto b = Buffer::wrap((const byte *)str.cStr(), str.length());
	m_buffer = std::move(b);
}

////////////////////////////////////////////////////////////////////////////////

bool BufferReader::eof() const
{
	return m_offset >= m_buffer.size();
}

int BufferReader::peek()
{
	if (eof())
		return EOF;
	return m_buffer[m_offset];
}

int BufferReader::get()
{
	if (eof())
		return EOF;
	return m_buffer[m_offset++];
}

usize BufferReader::read(void *data, usize size)
{
	if (eof())
		return 0;

	auto datap = static_cast<byte *>(data);
	usize sizeToRead = min(size, m_buffer.size() - m_offset);

	for (usize i = 0; i < sizeToRead; ++i)
		datap[i] = m_buffer[m_offset++];

	return sizeToRead;
}

bool BufferReader::seek(SeekMode whence, isize pos)
{
	switch (whence) {
		case SeekMode::Start:
			if (pos < 0 || (usize)pos > m_buffer.size())
				return false;
			m_offset = pos;
			break;
		case SeekMode::Current:
			if ((isize)m_offset + pos < 0 || m_offset + pos > m_buffer.size())
				return false;
			m_offset += pos;
			break;
		case SeekMode::End:
			if (pos > 0 || (usize)-pos > m_buffer.size())
				return false;
			m_offset = m_buffer.size() - pos;
			break;
	}
	return true;
}

usize BufferReader::tell() const
{
	return m_offset;
}

////////////////////////////////////////////////////////////////////////////////

}