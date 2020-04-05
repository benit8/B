/*
** B, 2019
** StringBase.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	template <typename T>
	class StringBase;

	class StringView;
	class String;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/Vector.hpp"
#include <cstdlib>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

template <typename S>
class StringBase
{
public:
	virtual ~StringBase() {}

	virtual bool empty() const = 0;
	virtual const char *cStr() const = 0;
	virtual usize length() const = 0;
	virtual char operator [](usize i) const = 0;

	virtual S substr(usize start = 0, usize length = nPos) const = 0;
	virtual Vector<S> split(const StringView &delimiter, bool keepEmpty = true, usize limit = -1) const = 0;

	virtual bool startsWith(const StringView &s) const = 0;
	virtual bool endsWith(const StringView &s) const = 0;
	virtual usize find(char c, usize start = 0) const = 0;
	virtual usize findLast(char c, usize start = nPos) const = 0;
	virtual usize find(const StringView &s, usize start = 0) const = 0;
	virtual usize findLast(const StringView &s, usize start = nPos) const = 0;
	virtual usize findOf(const StringView &s, usize start = 0) const = 0;
	virtual usize findLastOf(const StringView &s, usize end = nPos) const = 0;

	virtual bool operator ==(const S &rhs) const = 0;
	virtual bool operator !=(const S &rhs) const = 0;
	virtual bool operator <(const S &rhs) const = 0;
	virtual bool operator <=(const S &rhs) const = 0;
	virtual bool operator >(const S &rhs) const = 0;
	virtual bool operator >=(const S &rhs) const = 0;

	static const usize nPos = -1;
};

}