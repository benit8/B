/*
** B, 2019
** Stat.cpp
*/

#include "Stat.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

Stat::Stat(int fd)
{
	m_good = fstat(fd, &m_stat) == 0;
}

Stat::Stat(const StringView &filename, bool dereference)
{
	m_good = (dereference ? ::lstat : ::stat)(filename.cStr(), &m_stat) == 0;
}

////////////////////////////////////////////////////////////////////////////////

File::Type Stat::type() const
{
	switch (m_stat.st_mode & S_IFMT) {
		case S_IFBLK: return File::Block;
		case S_IFCHR: return File::Character;
		case S_IFDIR: return File::Directory;
		case S_IFIFO: return File::FIFO;
		case S_IFLNK: return File::SymLink;
		case S_IFREG: return File::Regular;
		case S_IFSOCK: return File::Socket;
		default: break;
	}
	return File::Unknown;
}

File::Perms Stat::perms() const
{
	mode_t mode = m_stat.st_mode & File::AllFull;
	return (File::Perms)mode;
}

////////////////////////////////////////////////////////////////////////////////

}