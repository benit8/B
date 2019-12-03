/*
** B, 2019
** IODevice.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "B/Common.hpp"
#include "B/String.hpp"
#include "B/StringView.hpp"
#include "B/Buffer.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

class IODevice
{
public:
	virtual ~IODevice();

	virtual bool open(OpenMode mode) = 0;
	virtual bool close();

	virtual int peek();
	virtual usize read(byte *s, usize n);
	virtual usize write(const byte *s, usize n);
	virtual bool seek(isize offset, SeekMode mode = SeekMode::Start);

	int readOne();
	Buffer read(usize n);
	Buffer readAll();
	bool readLine(String &line);
	bool write(const Buffer &b);
	usize tell() const;

	int fd() const { return m_fd; }
	bool eof() const { return m_eof; }
	OpenMode mode() const { return m_mode; }
	int error() const { return m_error; }
	bool isOpen() const { return m_fd != -1; }
	StringView errorString() const { return strerror(m_error); }
	bool hasError() const { return m_error != 0; }

protected:
	IODevice() {}

	virtual bool refillBuffer();

protected:
	int m_fd = -1;
	int m_error = 0;
	bool m_eof = false;
	OpenMode m_mode = OpenMode::NotOpen;
	Vector<byte> m_buffer;
};

}