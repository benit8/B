/*
** B, 2020
** Stream.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "B/Common.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

class Stream
{
public:
	virtual ~Stream() = default;

	virtual bool eof() const = 0;
	virtual bool seek(SeekMode, isize pos) = 0;
	virtual usize tell() = 0;

	OpenMode mode() const { return m_mode; }
	bool binary() const { return BitmaskEnum(m_mode).anyOf(OpenMode::Binary); }

protected:
	Stream() = default;

protected:
	OpenMode m_mode = OpenMode::NotOpen;
};

}