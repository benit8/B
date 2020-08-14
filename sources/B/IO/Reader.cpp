/*
** B, 2020
** Reader.cpp
*/

#include "B/IO/Reader.hpp"
#include "B/IO/Parser.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

FileReader Reader::fromFd(int fd, OpenMode mode)
{
	return FileReader(fd, mode);
}

FileReader Reader::fromFile(const String& filename, OpenMode mode)
{
	return FileReader(filename, mode);
}

BufferReader Reader::fromBuffer(const Buffer& buffer, bool binary)
{
	return BufferReader(buffer, binary);
}

BufferReader Reader::fromString(StringView buffer, bool binary)
{
	return BufferReader(buffer, binary);
}

////////////////////////////////////////////////////////////////////////////////

void Reader::ignore(usize n)
{
	for (usize i = 0; !eof() && i < n; ++i)
		get();
}

void Reader::ignoreUntil(int c, usize n)
{
	for (usize i = 0; !eof() && c != get() && i < n; ++i);
}

bool Reader::peek(int c)
{
	return peek() == c;
}

bool Reader::read(Buffer& buffer)
{
	buffer.clear();
	return read(buffer.data(), buffer.size()) == buffer.size();
}

bool Reader::readLine(String& line)
{
	if (eof())
		return false;

	static StringView discarded = "\r";

	line.clear();
	for (int c = get(); c != EOF && c != '\n'; c = get()) {
		if (discarded.find(c) != StringView::nPos)
			continue;
		line.append(c);
	}
	return true;
}

////////////////////////////////////////////////////////////////////////////////

Reader& Reader::operator >>(bool& b)
{
	if (binary())
		b = get() > 0;
	else {
		auto c = get();
		b = (c >= '1' && c <= '9');
	}
	return *this;
}

Reader& Reader::operator >>(char& c)
{
	c = get();
	return *this;
}

Reader& Reader::operator >>(i8& n)
{
	if (binary())
		read(&n, sizeof(n));
	else
		n = parse<i8>(*this, 0);
	return *this;
}

Reader& Reader::operator >>(i16& n)
{
	if (binary())
		read(&n, sizeof(n));
	else
		n = parse<i16>(*this, 0);
	return *this;
}

Reader& Reader::operator >>(i32& n)
{
	if (binary())
		read(&n, sizeof(n));
	else
		n = parse<i32>(*this, 0);
	return *this;
}

Reader& Reader::operator >>(i64& n)
{
	if (binary())
		read(&n, sizeof(n));
	else
		n = parse<i64>(*this, 0);
	return *this;
}

Reader& Reader::operator >>(u8& n)
{
	if (binary())
		read(&n, sizeof(n));
	else
		n = parse<u8>(*this, 0);
	return *this;
}

Reader& Reader::operator >>(u16& n)
{
	if (binary())
		read(&n, sizeof(n));
	else
		n = parse<u16>(*this, 0);
	return *this;
}

Reader& Reader::operator >>(u32& n)
{
	if (binary())
		read(&n, sizeof(n));
	else
		n = parse<u32>(*this, 0);
	return *this;
}

Reader& Reader::operator >>(u64& n)
{
	if (binary())
		read(&n, sizeof(n));
	else
		n = parse<u64>(*this, 0);
	return *this;
}

Reader& Reader::operator >>(f32& n)
{
	if (binary())
		read(&n, sizeof(n));
	else
		n = parse<f32>(*this);
	return *this;
}

Reader& Reader::operator >>(f64& n)
{
	if (binary())
		read(&n, sizeof(n));
	else
		n = parse<f64>(*this);
	return *this;
}

Reader& Reader::operator >>(f128& n)
{
	if (binary())
		read(&n, sizeof(n));
	else
		n = parse<f128>(*this);
	return *this;
}

Reader& Reader::operator >>(String& value)
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