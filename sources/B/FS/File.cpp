/*
** B, 2019
** File.cpp
*/

#include "File.hpp"
#include <unistd.h>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

File::File(const StringView &filename)
: m_filename(filename)
{
}

////////////////////////////////////////////////////////////////////////////////

bool File::open(OpenMode mode)
{
	if (m_filename.empty()) {
		m_error = -ENOENT;
		return false;
	}

	m_fd = ::open(m_filename.cStr(), mode, AllRead | UserWrite);
	if (m_fd < 0) {
		m_error = errno;
		return false;
	}

	m_mode = mode;
	return true;
}

////////////////////////////////////////////////////////////////////////////////

}