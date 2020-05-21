/*
** B, 2020
** Writer.cpp
*/

#include "B/IO/Writer.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

FileWriter Writer::toFd(int fd)
{
	return FileWriter(fd);
}

FileWriter Writer::toFile(const String &filename, OpenMode flags, File::Perms mode)
{
	return FileWriter(filename, flags, mode);
}

BufferWriter Writer::toBuffer(Buffer &buffer)
{
	return BufferWriter(buffer);
}

////////////////////////////////////////////////////////////////////////////////

Writer &Writer::operator <<(const char *value)
{
	for (usize i = 0; value[i] != '\0'; ++i)
		put(value[i]);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

}