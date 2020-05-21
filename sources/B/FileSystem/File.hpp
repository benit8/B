/*
** B, 2020
** File.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	class File;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/EnumOperators.hpp"

#include <sys/stat.h>
#include <fcntl.h>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

class File
{
public:
	enum class Type : int
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

	enum class Perms : mode_t
	{
		OtherExec  = S_IXOTH,  // 0001
		OtherWrite = S_IWOTH,  // 0002
		OtherRead  = S_IROTH,  // 0004
		GroupExec  = S_IXGRP,  // 0010
		GroupWrite = S_IWGRP,  // 0020
		GroupRead  = S_IRGRP,  // 0040
		UserExec   = S_IXUSR,  // 0100
		UserWrite  = S_IWUSR,  // 0200
		UserRead   = S_IRUSR,  // 0400
		SetUid     = S_ISUID,  // 1000
		SetGid     = S_ISGID,  // 2000
		StickyBit  = S_ISVTX,  // 4000

		UserFull   = UserRead | UserWrite | UserExec,
		GroupFull  = GroupRead | GroupWrite | GroupExec,
		OtherFull  = OtherRead | OtherWrite | OtherExec,

		AllRead    = UserRead | GroupRead | OtherRead,
		AllWrite   = UserWrite | GroupWrite | OtherWrite,
		AllExec    = UserExec | GroupExec | OtherExec,
		AllFull    = UserFull | GroupFull | OtherFull,

		FileDefault = AllRead | UserWrite | GroupWrite,
		DirDefault = FileDefault | AllExec,
	};
};

ENABLE_ENUM_OPERATORS(File::Perms);

////////////////////////////////////////////////////////////////////////////////

}