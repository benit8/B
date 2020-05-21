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

#include <type_traits>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

class Reader : public Stream
{
public:
	virtual ~Reader() = default;

	static FileReader fromFd(int fd);
	static FileReader fromFile(const String &filename, OpenMode = OpenMode::ReadOnly);
	static BufferReader fromBuffer(const Buffer &);
	static BufferReader fromString(StringView);

	virtual int peek() = 0;
	virtual int get() = 0;
	virtual usize read(void *data, usize size) = 0;

	bool read(Buffer &buffer)
	{
		buffer.clear();
		return read(buffer.data(), buffer.size()) == buffer.size();
	}

	template <typename T, std::enable_if_t<std::is_fundamental<T>::value>>
	Reader &operator >>(T &value)
	{
		value = T();
		read(&value, sizeof(value));
		return *this;
	}

	Reader &operator >>(bool &);
	Reader &operator >>(String &);

protected:
	Reader() = default;
};

////////////////////////////////////////////////////////////////////////////////

class FileReader : public Reader
{
public:
	FileReader(int fd);
	FileReader(const String &filename, OpenMode flags = OpenMode::ReadOnly);
	~FileReader();

	void close();

	bool eof() const override;
	int peek() override;
	int get() override;
	usize read(void *data, usize length) override;
	bool seek(SeekMode whence, isize pos) override;
	usize tell() const override;

private:
	int m_fd = -1;
	OpenMode m_flags = OpenMode::NotOpen;
	bool m_eof = true;
	std::optional<byte> m_peeked;
};

////////////////////////////////////////////////////////////////////////////////

class BufferReader : public Reader
{
public:
	BufferReader(const Buffer &buffer);
	BufferReader(StringView buffer);
	~BufferReader() = default;

	bool eof() const override;
	int peek() override;
	int get() override;
	usize read(void *data, usize length) override;
	bool seek(SeekMode whence, isize pos) override;
	usize tell() const override;

private:
	Buffer m_buffer;
	usize m_offset = 0;
	bool m_eof = false;
};

////////////////////////////////////////////////////////////////////////////////

}