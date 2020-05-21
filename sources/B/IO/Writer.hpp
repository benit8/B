/*
** B, 2020
** Writer.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	class Writer;
	class FileWriter;
	class BufferWriter;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/Common.hpp"
#include "B/Buffer.hpp"
#include "B/String.hpp"
#include "B/StringView.hpp"
#include "B/FileSystem/File.hpp"
#include "B/IO/Stream.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

class Writer : public Stream
{
public:
	virtual ~Writer();

	static FileWriter toFd(int fd);
	static FileWriter toFile(const String &filename, OpenMode = OpenMode::WriteOnly, File::Perms mode = File::Perms::FileDefault);
	static BufferWriter toBuffer(Buffer &);

	virtual bool put(int c) = 0;
	virtual usize write(const void *data, usize size) = 0;

	bool write(const Buffer &buffer)
	{
		return write(buffer.data(), buffer.size()) == buffer.size();
	}

	Writer &operator <<(bool);
	Writer &operator <<(char);
	Writer &operator <<(i8);
	Writer &operator <<(i16);
	Writer &operator <<(i32);
	Writer &operator <<(i64);
	Writer &operator <<(u8);
	Writer &operator <<(u16);
	Writer &operator <<(u32);
	Writer &operator <<(u64);
	Writer &operator <<(f32);
	Writer &operator <<(f64);
	Writer &operator <<(f128);
	Writer &operator <<(const char *);

protected:
	Writer();
};

////////////////////////////////////////////////////////////////////////////////

class FileWriter : public Writer
{
public:
	FileWriter(int fd);
	FileWriter(const String &filename, OpenMode flags = OpenMode::WriteOnly, File::Perms mode = File::Perms::FileDefault);
	~FileWriter();

	void close();
	void flush();

	bool eof() const override;
	bool put(int c) override;
	usize write(const void *data, usize length) override;
	bool seek(SeekMode whence, isize pos) override;
	usize tell() const override;

private:
	int m_fd = -1;
	OpenMode m_mode = OpenMode::NotOpen;
	Buffer m_buffer;
};

////////////////////////////////////////////////////////////////////////////////

class BufferWriter : public Writer
{
public:
	BufferWriter(Buffer &buffer);
	~BufferWriter();

	bool eof() const override;
	bool put(int c) override;
	usize write(const void *data, usize length) override;
	bool seek(SeekMode whence, isize pos) override;
	usize tell() const override;

private:
	Buffer m_buffer;
	usize m_offset = 0;
	bool m_eof = false;
};

////////////////////////////////////////////////////////////////////////////////

}