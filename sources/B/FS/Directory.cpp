/*
** B, 2019
** Directory.cpp
*/

#include "Directory.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

Directory::Entry::Entry()
{}

Directory::Entry::Entry(struct dirent *e)
{
	*this = e;
}

Directory::Entry &Directory::Entry::operator =(struct dirent *e)
{
	this->name = String(&e->d_name[0]);
	this->type = static_cast<File::Type>(e->d_type);
	this->inode = e->d_ino;
	this->offset = e->d_off;
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

Directory::Directory(const StringView &name)
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

Vector<Directory::Entry> Directory::scan(const StringView &dir, std::function<bool(const Entry &)> filter, std::function<int(const Entry &, const Entry &)> compar)
{
	struct dirent **ents;
	int n = scandir(dir.cStr(), &ents, NULL, NULL);
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

std::function<bool(const Directory::Entry &)> Directory::defaultScanFilter = [] (const Directory::Entry &)
{
	return true;
};

std::function<int(const Directory::Entry &, const Directory::Entry &)> Directory::defaultScanCompare = [] (const Directory::Entry &a, const Directory::Entry &b)
{
	return a.name.compare(b.name);
};

////////////////////////////////////////////////////////////////////////////////

}