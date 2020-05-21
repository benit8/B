/*
** B, 2020
** FileReader.cpp
*/

#include "B/IO/Reader.hpp"

#include <unistd.h>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

FileReader::FileReader(int fd)
{
	int flags = fcntl(fd, F_GETFL);
	if ((flags & O_RDONLY) == 0 && (flags & O_RDWR) == 0)
		throw std::runtime_error(format("FileReader::FileReader(%d): FD is not readable", fd));
	else {
		m_fd = fd;
		m_flags = (OpenMode)flags;
	}
}

FileReader::FileReader(const String &filename, OpenMode flags)
{
	m_fd = ::open(filename.cStr(), int(flags & OpenMode::StandardFlags));
	if (m_fd == -1)
		throw std::runtime_error(format("FileReader::FileReader(%s, %o): open() failed", filename, (int)flags));
	else {
		m_flags = flags;
	}
}

FileReader::~FileReader()
{
	close();
}

////////////////////////////////////////////////////////////////////////////////

void FileReader::close()
{
	if (m_fd > 2) {
		::close(m_fd);
		m_fd = -1;
	}
}

////////////////////////////////////////////////////////////////////////////////

bool FileReader::eof() const
{
	return m_eof;
}

int FileReader::peek()
{
	if (eof())
		return EOF;

	if (!m_peeked)
		m_peeked = get();
	return m_peeked.value();
}

int FileReader::get()
{
	if (eof())
		return EOF;

	if (m_peeked) {
		byte c = m_peeked.value();
		m_peeked.reset();
		return c;
	}

	byte c = 0;
	usize r = read(&c, 1);
	return r == 1 ? c : EOF;
}

usize FileReader::read(void *data, usize size)
{
	if (eof())
		return 0;

	auto datap = static_cast<byte *>(data);

	if (m_peeked) {
		*datap = m_peeked.value();
		m_peeked.reset();
		if (size == 1)
			return 1;
		--size;
		++datap;
	}

	isize r = ::read(m_fd, datap, size);
	if (r < 0) {
		eprint("FileReader::read(): %m");
		return 0;
	}

	if (r == 0)
		m_eof = true;

	return r;
}

bool FileReader::seek(SeekMode whence, isize pos)
{
	return lseek(m_fd, pos, (int)whence) != -1;
}

usize FileReader::tell() const
{
	return lseek(m_fd, 0, SEEK_CUR);
}

////////////////////////////////////////////////////////////////////////////////

}