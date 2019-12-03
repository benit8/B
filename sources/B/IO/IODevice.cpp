/*
** B, 2019
** IODevice.cpp
*/

#include "IODevice.hpp"
#include <unistd.h>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

IODevice::~IODevice()
{
	this->close();
}

////////////////////////////////////////////////////////////////////////////////

bool IODevice::close()
{
	if (this->fd() < 0 || this->mode() == OpenMode::NotOpen)
		return false;
	if (::close(this->fd()) < 0) {
		m_error = errno;
		return false;
	}
	m_fd = -1;
	m_mode = OpenMode::NotOpen;
	return true;
}

int IODevice::peek()
{
	if (m_buffer.empty())
		this->refillBuffer();
	return !m_buffer.empty() ? m_buffer.front() : EOF;
}

usize IODevice::read(byte *s, usize n)
{
	if (this->eof() || (s == nullptr || n == 0))
		return 0;

	if (!m_buffer.empty()) {
		usize toRead = min(n, m_buffer.size());
		std::memmove(s, m_buffer.data(), toRead);
		m_buffer.erase(0, toRead);

		// If there was enough in the buffer, return here.
		if (toRead == n)
			return n;

		n -= toRead;
		s += toRead;
	}
	isize r = ::read(fd(), s, n);
	if (r < 0) {
		m_error = errno;
		return 0;
	}
	else if (r == 0) {
		m_eof = true;
	}

	return r;
}

usize IODevice::write(const byte *s, usize n)
{
	isize r = ::write(m_fd, s, n);
	if (r < 0) {
		m_error = errno;
		return 0;
	}
	return r;
}

bool IODevice::seek(isize offset, SeekMode mode)
{
	int r = ::lseek(m_fd, offset, mode);
	if (r < 0) {
		m_error = errno;
		return false;
	}
	m_buffer.clear();
	m_eof = false;
	return true;
}

////////////////////////////////////////////////////////////////////////////////

int IODevice::readOne()
{
	byte c = 0;
	return this->read(&c, 1) == 1 ? c : EOF;
}

Buffer IODevice::read(usize n)
{
	if (this->eof() || n == 0)
		return {};

	auto data = new byte[n];
	usize r = this->read(data, n);
	if (r == 0)
		return {};

	return Buffer::adopt(data, r);
}

Buffer IODevice::readAll()
{
	Vector<byte> contents;

	while (!(m_buffer.empty() && !this->refillBuffer()))
		contents.append(std::move(m_buffer));

	return Buffer::copy(contents.data(), contents.size());
}

bool IODevice::readLine(String &line)
{
	line.erase();

	for (;;) {
		if (m_buffer.empty() && !this->refillBuffer())
			return !line.empty();

		usize pos = m_buffer.find('\n');
		if (pos == (usize)-1) {
			line.append(StringView((char*)m_buffer.data(), m_buffer.size()));
			m_buffer.clear();
			continue;
		}
		else {
			line.append(StringView((char*)m_buffer.data(), pos));
			m_buffer.erase(0, pos + 1);
			break;
		}
	}

	return true;
}

bool IODevice::write(const Buffer &b)
{
	return this->write(b.data(), b.size()) == b.size();
}

usize IODevice::tell() const
{
	return lseek(m_fd, 0, SEEK_CUR);
}

////////////////////////////////////////////////////////////////////////////////

bool IODevice::refillBuffer()
{
	if (!this->isOpen())
		return false;

	byte buff[1024];
	isize r = ::read(m_fd, buff, sizeof(buff));
	if (r < 0) {
		m_error = errno;
		return false;
	}
	else if (r == 0) {
		m_eof = true;
		return false;
	}
	m_buffer.assign(buff, r);
	return true;
}

////////////////////////////////////////////////////////////////////////////////

}