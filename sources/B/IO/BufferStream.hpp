/*
** B, 2019
** BufferStream.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	class BufferStream;
}

////////////////////////////////////////////////////////////////////////////////

#include "Stream.hpp"
#include "B/Buffer.hpp"
#include "B/String.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

class BufferStream final : public Stream
{
public:
	BufferStream(const byte *data, usize length, OpenMode mode = OpenMode::ReadOnly);
	BufferStream(const Buffer &buffer, OpenMode mode = OpenMode::ReadOnly);
	BufferStream(Buffer &&buffer, OpenMode mode = OpenMode::ReadOnly);
	BufferStream(String &string, OpenMode mode = OpenMode::ReadOnly);
	~BufferStream() {}

	bool canRead(usize n = 1) override;
	bool canWrite(usize n = 1) override;
	usize read(byte *s, usize n) override;
	usize write(const byte *, usize) override;
	bool seek(isize offset, SeekMode mode = SeekMode::Start) override;

private:
	bool refillBuffer() { return false; }

private:
	Buffer m_originalBuffer;
	usize m_offset = 0;
};

}