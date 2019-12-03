/*
** B, 2019
** File.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	class File;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/IO/IODevice.hpp"

#include <sys/stat.h>
#include <fcntl.h>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

class File : public IODevice
{
public:
	enum Type : int
	{
		Unknown   = -1,        // The file type could not be determined
		Block     = S_IFBLK,   // Block device
		Character = S_IFCHR,   // Character device
		Directory = S_IFDIR,   // Directory
		FIFO      = S_IFIFO,   // Named pipe (FIFO)
		SymLink   = S_IFLNK,   // Symbolic link
		Regular   = S_IFREG,   // Regular file
		Socket    = S_IFSOCK,  // UNIX domain socket
	};

	enum Perms : uint
	{
		UserExec   = S_IXUSR,  // 0100
		UserWrite  = S_IWUSR,  // 0200
		UserRead   = S_IRUSR,  // 0400
		GroupExec  = S_IXGRP,  // 0010
		GroupWrite = S_IWGRP,  // 0020
		GroupRead  = S_IRGRP,  // 0040
		OtherExec  = S_IXOTH,  // 0001
		OtherWrite = S_IWOTH,  // 0002
		OtherRead  = S_IROTH,  // 0004

		UserFull   = UserRead | UserWrite | UserExec,
		GroupFull  = GroupRead | GroupWrite | GroupExec,
		OtherFull  = OtherRead | OtherWrite | OtherExec,

		AllRead    = UserRead | GroupRead | OtherRead,
		AllWrite   = UserWrite | GroupWrite | OtherWrite,
		AllExec    = UserExec | GroupExec | OtherExec,
		AllFull    = UserFull | GroupFull | OtherFull,
	};

public:
	File(const StringView &filename);

	bool open(OpenMode mode);

	const String &filename() const { return m_filename; }

private:
	String m_filename;
};

////////////////////////////////////////////////////////////////////////////////

}