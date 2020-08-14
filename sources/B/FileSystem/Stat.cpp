/*
** B, 2020
** Stat.cpp
*/

#include "B/FileSystem/Stat.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

Stat::Stat(int fd)
{
	m_good = fstat(fd, &m_stat) == 0;
}

Stat::Stat(const String& filename, bool dereference)
{
	m_good = (dereference ? ::lstat : ::stat)(filename.cStr(), &m_stat) == 0;
}

////////////////////////////////////////////////////////////////////////////////

File::Type Stat::type() const
{
	switch (m_stat.st_mode & S_IFMT) {
		case S_IFBLK: return File::Type::Block;
		case S_IFCHR: return File::Type::Character;
		case S_IFDIR: return File::Type::Directory;
		case S_IFIFO: return File::Type::FIFO;
		case S_IFLNK: return File::Type::SymLink;
		case S_IFREG: return File::Type::Regular;
		case S_IFSOCK: return File::Type::Socket;
		default: break;
	}
	return File::Type::Unknown;
}

File::Perms Stat::perms() const
{
	return (File::Perms)m_stat.st_mode & File::Perms::AllFull;
}

////////////////////////////////////////////////////////////////////////////////

}