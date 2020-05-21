/*
** B, 2020
** BufferWriter.cpp
*/

#include "B/IO/Writer.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

BufferWriter::BufferWriter(Buffer &buffer)
: m_buffer(buffer)
{}

////////////////////////////////////////////////////////////////////////////////

bool BufferWriter::eof() const
{
	return m_offset >= m_buffer.size();
}

bool BufferWriter::put(int c)
{
	if (eof())
		return false;

	m_buffer[m_offset++] = c;
	return true;
}

usize BufferWriter::write(const void *data, usize size)
{
	auto datap = static_cast<const byte *>(data);
	usize written = 0;

	while (put(datap[written++]) && written < size);

	return written;
}

bool BufferWriter::seek(SeekMode whence, isize pos)
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

usize BufferWriter::tell()
{
	return m_offset;
}

////////////////////////////////////////////////////////////////////////////////

}