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
	virtual usize tell() const = 0;

protected:
	Stream() = default;
};

}