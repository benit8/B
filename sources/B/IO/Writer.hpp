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
	class StringWriter;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/Common.hpp"
#include "B/Buffer.hpp"
#include "B/String.hpp"
#include "B/StringView.hpp"
#include "B/Containers/Array.hpp"
#include "B/FileSystem/File.hpp"
#include "B/IO/Stream.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

class Writer : public Stream
{
public:
	virtual ~Writer() = default;

	static FileWriter toFd(int fd);
	static FileWriter toFile(const String &filename, OpenMode = OpenMode::WriteOnly | OpenMode::Create, File::Perms mode = File::Perms::FileDefault);
	static BufferWriter toBuffer(Buffer &);
	static StringWriter toString(String &);

	virtual bool put(int c) = 0;
	virtual usize write(const void *data, usize size) = 0;

	bool write(const Buffer &buffer);

	Writer &operator <<(bool b);
	Writer &operator <<(char c);
	Writer &operator <<(i8  n);
	Writer &operator <<(i16 n);
	Writer &operator <<(i32 n);
	Writer &operator <<(i64 n);
	Writer &operator <<(u8  n);
	Writer &operator <<(u16 n);
	Writer &operator <<(u32 n);
	Writer &operator <<(u64 n);
	Writer &operator <<(f32  n);
	Writer &operator <<(f64  n);
	Writer &operator <<(f128 n);
	Writer &operator <<(const char *);

protected:
	Writer() = default;
};

////////////////////////////////////////////////////////////////////////////////

class FileWriter : public Writer
{
public:
	FileWriter(int fd);
	FileWriter(const String &filename, OpenMode flags = OpenMode::WriteOnly | OpenMode::Create, File::Perms mode = File::Perms::FileDefault);
	~FileWriter();

	void close();
	void flush();

	bool eof() const override;
	bool put(int c) override;
	usize write(const void *data, usize length) override;
	bool seek(SeekMode whence, isize pos) override;
	usize tell() override;

private:
	int m_fd = -1;
	OpenMode m_flags = OpenMode::NotOpen;
	bool m_eof = false;
	Array<byte, 1024> m_buffer;
	usize m_offset = 0;
};

////////////////////////////////////////////////////////////////////////////////

class BufferWriter : public Writer
{
public:
	BufferWriter(Buffer &buffer);
	~BufferWriter() = default;

	bool eof() const override;
	bool put(int c) override;
	usize write(const void *data, usize length) override;
	bool seek(SeekMode whence, isize pos) override;
	usize tell() override;

private:
	Buffer &m_buffer;
	usize m_offset = 0;
	bool m_eof = false;
};

////////////////////////////////////////////////////////////////////////////////

class StringWriter : public Writer
{
public:
	StringWriter(String &str);
	~StringWriter() = default;

	bool eof() const override;
	bool put(int c) override;
	usize write(const void *data, usize length) override;
	bool seek(SeekMode whence, isize pos) override;
	usize tell() override;

private:
	String &m_string;
	usize m_offset = 0;
	bool m_eof = false;
};

////////////////////////////////////////////////////////////////////////////////

extern FileWriter StdOut;
extern FileWriter StdErr;

}