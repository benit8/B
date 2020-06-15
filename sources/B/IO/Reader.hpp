/*
** B, 2020
** Reader.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	class Reader;
	class FileReader;
	class BufferReader;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/Common.hpp"
#include "B/Buffer.hpp"
#include "B/String.hpp"
#include "B/StringView.hpp"
#include "B/IO/Stream.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

class Reader : public Stream
{
public:
	virtual ~Reader() = default;

	static FileReader fromFd(int fd, OpenMode = OpenMode::ReadOnly);
	static FileReader fromFile(const String &filename, OpenMode = OpenMode::ReadOnly);
	static BufferReader fromBuffer(const Buffer &, bool binary = false);
	static BufferReader fromString(StringView, bool binary = false);

	virtual int peek() = 0;
	virtual int get() = 0;
	virtual usize read(void *data, usize size) = 0;

	void ignore(usize n = 1);
	void ignoreUntil(int c, usize n = -1);
	bool peek(int c);
	bool read(Buffer &buffer);
	bool readLine(String &line);

	Reader &operator >>(bool &b);
	Reader &operator >>(char &c);
	Reader &operator >>(i8  &n);
	Reader &operator >>(i16 &n);
	Reader &operator >>(i32 &n);
	Reader &operator >>(i64 &n);
	Reader &operator >>(u8  &n);
	Reader &operator >>(u16 &n);
	Reader &operator >>(u32 &n);
	Reader &operator >>(u64 &n);
	Reader &operator >>(f32  &n);
	Reader &operator >>(f64  &n);
	Reader &operator >>(f128 &n);
	Reader &operator >>(String &);

protected:
	Reader() = default;
};

////////////////////////////////////////////////////////////////////////////////

class FileReader final : public Reader
{
public:
	FileReader(int fd, OpenMode = OpenMode::ReadOnly);
	FileReader(const String &filename, OpenMode = OpenMode::ReadOnly);
	~FileReader();

	void close();

	// Stream
	bool eof() const override;
	bool seek(SeekMode whence, isize pos) override;
	usize tell() override;

	// Reader
	int peek() override;
	int get() override;
	usize read(void *data, usize length) override;

private:
	int m_fd = -1;
	bool m_eof = false;
	std::optional<byte> m_peeked;
};

////////////////////////////////////////////////////////////////////////////////

class BufferReader final : public Reader
{
public:
	BufferReader(const Buffer &buffer, bool binary = false);
	BufferReader(StringView buffer, bool binary = false);
	~BufferReader() = default;

	// Stream
	bool eof() const override;
	bool seek(SeekMode whence, isize pos) override;
	usize tell() override;

	// Reader
	int peek() override;
	int get() override;
	usize read(void *data, usize length) override;

private:
	Buffer m_buffer;
	usize m_offset = 0;
	bool m_eof = false;
};

////////////////////////////////////////////////////////////////////////////////

}