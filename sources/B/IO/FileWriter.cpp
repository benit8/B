/*
** B, 2020
** FileWriter.cpp
*/

#include "B/IO/Writer.hpp"
#include "tinyformat.hpp"

#include <unistd.h>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

FileWriter::FileWriter(int fd)
{
	int flags = fcntl(fd, F_GETFL);
	if ((flags & O_WRONLY) == 0 && (flags & O_RDWR) == 0)
		throw std::runtime_error(tfm::format("FileWriter::FileWriter(%d): FD is not writable", fd));
	else {
		m_fd = fd;
		m_flags = (OpenMode)flags;
	}
}

FileWriter::FileWriter(const String &filename, OpenMode flags, File::Perms mode)
{
	m_fd = ::open(filename.cStr(), int(flags & OpenMode::StandardFlags), (mode_t)mode);
	if (m_fd == -1)
		throw std::runtime_error(tfm::format("FileWriter::FileWriter(%s, %o, %o): open() failed", filename, (int)flags, (mode_t)mode));
	else {
		m_flags = flags;
	}
}

FileWriter::~FileWriter()
{
	close();
}

////////////////////////////////////////////////////////////////////////////////

void FileWriter::close()
{
	flush();
	if (m_fd > 2)
		::close(m_fd);
}

void FileWriter::flush()
{
	if (m_offset == 0)
		return;

	isize written = ::write(m_fd, m_buffer.data(), m_offset);
	if (written < 0) {
		tfm::format(std::cerr, "FileWriter::flush(): write() failed: %s\n", strerror(errno));
		return;
	}
	if (written == 0)
		m_eof = true;
	else {
		m_offset -= written;
		if (m_offset != 0)
			::memmove(m_buffer.data() + (m_offset - written), m_buffer.data(), m_offset - written);
	}
}

////////////////////////////////////////////////////////////////////////////////

bool FileWriter::eof() const
{
	return m_eof;
}

bool FileWriter::put(int c)
{
	if (eof())
		return false;

	m_buffer[m_offset++] = c;
	if (m_offset >= m_buffer.size() || c == '\n')
		flush();
	return !eof();
}

usize FileWriter::write(const void *data, usize size)
{
	if (eof())
		return 0;

	auto datap = static_cast<const byte *>(data);
	for (usize i = 0; i < size; ++i) {
		if (!put(datap[i]))
			return i;
	}
	return size;
}

bool FileWriter::seek(SeekMode whence, isize pos)
{
	flush();
	if (::lseek(m_fd, pos, (int)whence) < 0)
		return false;
	m_eof = false;
	return true;
}

usize FileWriter::tell()
{
	flush();
	return ::lseek(m_fd, 0, SEEK_CUR);
}

////////////////////////////////////////////////////////////////////////////////

}