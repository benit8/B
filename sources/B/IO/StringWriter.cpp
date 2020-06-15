/*
** B, 2020
** StringWriter.cpp
*/

#include "B/IO/Writer.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

StringWriter::StringWriter(String &string)
: m_string(string)
{}

////////////////////////////////////////////////////////////////////////////////

bool StringWriter::eof() const
{
	return m_offset >= String::maxSize;
}

bool StringWriter::put(int c)
{
	if (eof())
		return false;

	m_string.insert(m_offset, c);
	return true;
}

usize StringWriter::write(const void *data, usize size)
{
	auto datap = static_cast<const char *>(data);
	m_string.insert(m_offset, datap, size);
	return size;
}

bool StringWriter::seek(SeekMode whence, isize pos)
{
	switch (whence) {
	case SeekMode::Start:
		if (pos < 0 || (usize)pos > m_string.length())
			return false;
		m_offset = pos;
		break;
	case SeekMode::Current:
		if ((isize)m_offset + pos < 0 || m_offset + pos > m_string.length())
			return false;
		m_offset += pos;
		break;
	case SeekMode::End:
		if (pos > 0 || (usize)-pos > m_string.length())
			return false;
		m_offset = m_string.length() - pos;
		break;
	}
	return true;
}

usize StringWriter::tell()
{
	return m_offset;
}

////////////////////////////////////////////////////////////////////////////////

}