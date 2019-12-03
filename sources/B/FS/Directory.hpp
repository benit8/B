/*
** B, 2019
** Directory.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	class Directory;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/String.hpp"
#include "B/FS/File.hpp"

#include <dirent.h>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

class Directory
{
public:
	struct Entry
	{
		Entry();
		Entry(struct dirent *e);
		Entry &operator =(struct dirent *e);

		String name {};
		File::Type type { File::Unknown };
		ino_t inode { 0 };
		off_t offset { 0 };
	};

	static std::function<bool(const Entry &)> defaultScanFilter;
	static std::function<int(const Entry &, const Entry &)> defaultScanCompare;

public:
	Directory(const StringView &name);
	Directory(int fd);
	~Directory();

	bool readEntry(Entry &entry);
	void rewind();
	void seek(long offset);
	long tell();
	int fd();

	static Vector<Entry> scan(const StringView &dir,
	                          std::function<bool(const Entry &)> filter = defaultScanFilter,
	                          std::function<int(const Entry &, const Entry &)> compar = defaultScanCompare);

private:
	DIR *m_dirp = NULL;
};

}