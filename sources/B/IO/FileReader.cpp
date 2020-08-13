/*
** B, 2020
** FileReader.cpp
*/

#include "B/IO/Reader.hpp"
#include "tinyformat.hpp"

#include <unistd.h>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

FileReader::FileReader(int fd, OpenMode mode)
{
	int fdMode = fcntl(fd, F_GETFL);
	if ((fdMode & O_RDONLY) == 0 && (fdMode & O_RDWR) == 0)
		throw std::runtime_error(tfm::format("FileReader::FileReader(%d): FD is not readable", fd));
	else {
		m_fd = fd;
		m_mode = (OpenMode)fdMode | mode;
	}
}

FileReader::FileReader(const String &filename, OpenMode mode)
{
	m_fd = ::open(filename.cStr(), int(mode & OpenMode::StandardFlags));
	if (m_fd == -1)
		throw std::runtime_error(tfm::format("FileReader::FileReader(%s, %o): open() failed", filename, (int)mode));
	else {
		m_mode = mode;
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
		tfm::format(std::cerr, "FileReader::read(): %s", strerror(errno));
		return 0;
	}

	if (r == 0)
		m_eof = true;

	return r;
}

bool FileReader::seek(SeekMode whence, isize pos)
{
	if (::lseek(m_fd, pos, (int)whence) < 0)
		return false;
	m_eof = false;
	m_peeked.reset();
	return true;
}

usize FileReader::tell()
{
	return ::lseek(m_fd, 0, SEEK_CUR);
}

////////////////////////////////////////////////////////////////////////////////

}