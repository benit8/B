/*
** B, 2019
** FileStream.cpp
*/

#include "FileStream.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

// Stream not opened
FileStream::FileStream()
{}

FileStream::FileStream(const int fd, OpenMode mode)
{
	m_fd = fd;
	open(mode);
}

FileStream::FileStream(const StringView &filename, OpenMode mode)
{
	m_fd = ::open(filename.cStr(), OpenMode::ReadOnly);
	open(mode);
}

////////////////////////////////////////////////////////////////////////////////

bool FileStream::canRead(usize n)
{
	if (!Stream::canRead(n))
		return false;
	if (m_buffer.size() < n && !this->refillBuffer())
		return false;
	return true;
}

bool FileStream::canWrite(usize n)
{
	if (!Stream::canWrite(n))
		return false;
	return true;
}

// usize FileStream::read(byte *s, usize n)
// {
// }

// usize FileStream::write(const byte *s, usize n)
// {
// }

////////////////////////////////////////////////////////////////////////////////

}