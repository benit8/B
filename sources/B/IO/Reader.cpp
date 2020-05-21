/*
** B, 2020
** Reader.cpp
*/

#include "B/IO/Reader.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

FileReader Reader::fromFd(int fd)
{
	return FileReader(fd);
}

FileReader Reader::fromFile(const String &filename, OpenMode flags)
{
	return FileReader(filename, flags);
}

BufferReader Reader::fromBuffer(const Buffer &buffer)
{
	return BufferReader(buffer);
}

BufferReader Reader::fromString(StringView buffer)
{
	return BufferReader(buffer);
}

////////////////////////////////////////////////////////////////////////////////

Reader &Reader::operator >>(String &value)
{
	value.erase();

	while (iscntrl(peek()))
		get();

	for (int c = get(); c >= 0 && !isspace(c); c = get())
		value.append(c);

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

}