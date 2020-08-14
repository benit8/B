/*
** B, 2020
** Writer.cpp
*/

#include "B/IO/Writer.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

FileWriter StdOut = Writer::toFd(1);
FileWriter StdErr = Writer::toFd(2);

////////////////////////////////////////////////////////////////////////////////

FileWriter Writer::toFd(int fd)
{
	return FileWriter(fd);
}

FileWriter Writer::toFile(const String& filename, OpenMode flags, File::Perms mode)
{
	return FileWriter(filename, flags, mode);
}

BufferWriter Writer::toBuffer(Buffer& buffer)
{
	return BufferWriter(buffer);
}

StringWriter Writer::toString(String& string)
{
	return StringWriter(string);
}

////////////////////////////////////////////////////////////////////////////////

bool Writer::write(const Buffer& buffer)
{
	return write(buffer.data(), buffer.size()) == buffer.size();
}

////////////////////////////////////////////////////////////////////////////////

Writer& Writer::operator <<(bool b)
{
	if (binary())
		put(b ? 1 : 0);
	else
		put(b ? '1' : '0');
	return *this;
}

Writer& Writer::operator <<(char c)
{
	put(c);
	return *this;
}

Writer& Writer::operator <<(i8 n)
{
	write(&n, sizeof(n));
	return *this;
}

Writer& Writer::operator <<(i16 n)
{
	write(&n, sizeof(n));
	return *this;
}

Writer& Writer::operator <<(i32 n)
{
	write(&n, sizeof(n));
	return *this;
}

Writer& Writer::operator <<(i64 n)
{
	write(&n, sizeof(n));
	return *this;
}

Writer& Writer::operator <<(u8 n)
{
	write(&n, sizeof(n));
	return *this;
}

Writer& Writer::operator <<(u16 n)
{
	write(&n, sizeof(n));
	return *this;
}

Writer& Writer::operator <<(u32 n)
{
	write(&n, sizeof(n));
	return *this;
}

Writer& Writer::operator <<(u64 n)
{
	write(&n, sizeof(n));
	return *this;
}

Writer& Writer::operator <<(f32 n)
{
	write(&n, sizeof(n));
	return *this;
}

Writer& Writer::operator <<(f64 n)
{
	write(&n, sizeof(n));
	return *this;
}

Writer& Writer::operator <<(f128 n)
{
	write(&n, sizeof(n));
	return *this;
}

Writer& Writer::operator <<(const char* value)
{
	for (usize i = 0; value[i] != '\0'; ++i)
		put(value[i]);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

}