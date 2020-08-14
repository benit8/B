/*
** B, 2020
** Directory.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	class Directory;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/String.hpp"
#include "B/FileSystem/File.hpp"

#include <dirent.h>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

class Directory
{
public:
	struct Entry
	{
		String name;
		File::Type type = File::Type::Unknown;
		ino_t inode = 0;
		off_t offset = 0;

		Entry() = default;

		explicit Entry(struct dirent *e)
		: name(e->d_name)
		, type(static_cast<File::Type>(e->d_type))
		, inode(e->d_ino)
		, offset(e->d_off)
		{}

		Entry &operator =(struct dirent *e)
		{
			name.assign(e->d_name);
			type = static_cast<File::Type>(e->d_type);
			inode = e->d_ino;
			offset = e->d_off;
			return *this;
		}
	};

	using Filter = Function<bool(const Entry &)>;
	using Comparator = Function<bool(const Entry &, const Entry &)>;

public:
	explicit Directory(const String &name);
	explicit Directory(int fd);
	~Directory();

	bool readEntry(Entry &entry);
	void rewind();
	void seek(long offset);
	long tell();
	int fd();

	static Filter defaultScanFilter;
	static Filter hiddenScanFilter;
	static Comparator defaultScanCompare;
	static Vector<Entry> scan(const String &dir, Filter filter = defaultScanFilter, Comparator compar = defaultScanCompare);

private:
	DIR *m_dirp = NULL;
};

}