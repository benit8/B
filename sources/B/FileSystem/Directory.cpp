/*
** B, 2020
** Directory.cpp
*/

#include "B/FileSystem/Directory.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

Directory::Directory(const String &name)
{
	m_dirp = opendir(name.cStr());
}

Directory::Directory(int fd)
{
	m_dirp = fdopendir(fd);
}

Directory::~Directory()
{
	closedir(m_dirp);
}

////////////////////////////////////////////////////////////////////////////////

bool Directory::readEntry(Directory::Entry &entry)
{
	struct dirent *ent = readdir(m_dirp);
	if (ent == NULL)
		return false;

	entry = ent;
	return true;
}

void Directory::rewind()
{
	rewinddir(m_dirp);
}

void Directory::seek(long offset)
{
	seekdir(m_dirp, offset);
}

long Directory::tell()
{
	return telldir(m_dirp);
}

int Directory::fd()
{
	return dirfd(m_dirp);
}

////////////////////////////////////////////////////////////////////////////////

Vector<Directory::Entry> Directory::scan(const String &dirName, Directory::Filter filter, Directory::Comparator compar)
{
	struct dirent **ents;
	int n = scandir(dirName.cStr(), &ents, NULL, NULL);
	if (n < 0)
		return {};

	Vector<Entry> entries;
	for (int i = 0; i < n; i++) {
		Entry e(ents[i]);
		if (filter(e))
			entries.append(e);
		free(ents[i]);
	}

	entries.sort(compar);

	free(ents);
	return entries;
}

Directory::Filter Directory::defaultScanFilter = [] (const Directory::Entry &e)
{
	return e.name[0] != '.';
};

Directory::Filter Directory::hiddenScanFilter = [] (const Directory::Entry &)
{
	return true;
};

Directory::Comparator Directory::defaultScanCompare = [] (const Directory::Entry &a, const Directory::Entry &b)
{
	return a.name.compare(b.name);
};

////////////////////////////////////////////////////////////////////////////////

}