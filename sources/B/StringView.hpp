/*
** B, 2019
** StringView.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	class StringView;
	class String;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/Containers/Vector.hpp"
#include "B/IO/Writer.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

class StringView
{
public:
	StringView();
	StringView(const String& other);
	StringView(const char* str);
	StringView(const char* str, usize length);

	const char* cStr() const { return m_chars; }
	usize length() const { return m_len; }
	bool null() const { return m_chars == nullptr; }
	bool empty() const { return null() || m_len == 0; }
	char operator [](usize i) const { return m_chars[i]; }

	void trimRight(usize n);
	void trimLeft(usize n);
	StringView substr(usize start = 0, usize length = nPos) const;
	Vector<StringView> split(StringView delimiter, bool keepEmpty = true, usize limit = -1) const;
	String toString() const;

	int compare(StringView s) const;
	bool startsWith(StringView s) const;
	bool endsWith(StringView s) const;
	usize find(char c, usize start = 0) const;
	usize find(StringView s, usize start = 0) const;
	usize findLast(char c, usize end = nPos) const;
	usize findLast(StringView s, usize end = nPos) const;
	usize findOf(StringView s, usize start = 0) const;
	usize findLastOf(StringView s, usize end = nPos) const;

	StringView &operator =(nullptr_t);
	bool operator ==(StringView rhs) const;
	bool operator !=(StringView rhs) const;
	bool operator <(StringView rhs) const;
	bool operator <=(StringView rhs) const;
	bool operator >(StringView rhs) const;
	bool operator >=(StringView rhs) const;

	friend Writer& operator <<(Writer&, StringView);
	friend std::ostream& operator <<(std::ostream&, StringView);

	static const usize nPos = -1;

private:
	const char* m_chars = nullptr;
	usize m_len = 0;
};

}