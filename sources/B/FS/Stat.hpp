/*
** B, 2019
** Stat.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "B/StringView.hpp"
#include "B/FS/File.hpp"

#include <sys/stat.h>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

class Stat
{
public:
	Stat(int fd);
	Stat(const StringView &filename, bool dereference = true);
	~Stat() {}

	bool good() const { return m_good; }
	operator bool() const { return good(); }

	File::Type type() const;
	File::Perms perms() const;
	dev_t device() const { return m_stat.st_dev; }
	ino_t inode() const { return m_stat.st_ino; }
	u64 links() const { return m_stat.st_nlink; }
	uid_t userId() const { return m_stat.st_uid; }
	gid_t groupId() const { return m_stat.st_gid; }
	dev_t deviceId() const { return m_stat.st_rdev; }
	usize size() const { return m_stat.st_size; }
	usize blockSize() const { return m_stat.st_blksize; }
	u64 blocks() const { return m_stat.st_blocks; }

	bool isRegular() const { return good() && S_ISREG(m_stat.st_mode); }
	bool isDirectory() const { return good() && S_ISDIR(m_stat.st_mode); }
	bool isBlockDevice() const { return good() && S_ISBLK(m_stat.st_mode); }
	bool isCharacterDevice() const { return good() && S_ISCHR(m_stat.st_mode); }
	bool isFIFO() const { return good() && S_ISFIFO(m_stat.st_mode); }
	bool isLink() const { return good() && S_ISLNK(m_stat.st_mode); }
	bool isSocket() const { return good() && S_ISSOCK(m_stat.st_mode); }

private:
	struct stat m_stat;
	bool m_good = false;
};

}