/*
** B, 2019
** StringView.cpp
*/

#include "B/StringView.hpp"
#include "B/String.hpp"
#include "B/Types.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

StringView::StringView()
: m_chars(nullptr)
, m_len(0)
{}

StringView::StringView(const String& string)
: m_chars(string.cStr())
, m_len(string.length())
{}

StringView::StringView(const char* str)
: m_chars(str)
, m_len(0)
{
	if (str)
		for (m_len = 0; str[m_len] != '\0'; m_len++);
}

StringView::StringView(const char* str, usize length)
: m_chars(length > 0 ? str : nullptr)
, m_len(length)
{}

////////////////////////////////////////////////////////////////////////////////

void StringView::trimRight(usize n)
{
	if (n >= m_len) {
		m_len = 0;
		m_chars = nullptr;
	}
	else {
		m_len -= n;
	}
}

void StringView::trimLeft(usize n)
{
	if (n >= m_len) {
		m_len = 0;
		m_chars = nullptr;
	}
	else {
		m_len -= n;
		m_chars += n;
	}
}

StringView StringView::substr(usize start, usize length) const
{
	if (length > m_len)
		length = m_len - start;
	if (start + length > m_len)
		return {};
	return StringView(m_chars + start, length);
}

Vector<StringView> StringView::split(StringView del, bool keepEmpty, usize limit) const
{
	if (empty())
		return {};

	usize pos = 0, lastPos = 0;
	Vector<StringView> v;
	while (v.size() < limit && (pos = find(del, pos)) != nPos) {
		if (pos - lastPos > 0 || keepEmpty)
			v.append(substr(lastPos, pos - lastPos));
		pos += del.length();
		lastPos = pos;
	}
	if (m_len - lastPos > 0 || keepEmpty)
		v.append(substr(lastPos));
	return v;
}

String StringView::toString() const
{
	return String(m_chars, m_len);
}


int StringView::compare(StringView s) const
{
	if (null())
		return s.null() ? 0 : -1;
	else if (s.null())
		return 1;
	return strcmp(m_chars, s.m_chars);
}

bool StringView::startsWith(StringView s) const
{
	if (s.empty())
		return true;
	if (empty())
		return false;
	if (s.length() > length())
		return false;
	return memcmp(m_chars, s.cStr(), s.length()) == 0;
}

bool StringView::endsWith(StringView s) const
{
	if (s.empty())
		return true;
	if (empty())
		return false;
	if (s.length() > length())
		return false;
	return memcmp(m_chars + m_len - s.length(), s.cStr(), s.length()) == 0;
}

usize StringView::find(char c, usize start) const
{
	for (usize i = start; i < m_len; ++i) {
		if (m_chars[i] == c)
			return i;
	}
	return nPos;
}

usize StringView::findLast(char c, usize end) const
{
	usize i = (end >= m_len) ? m_len - 1 : end;
	while (m_chars[i] != c) {
		if (--i == 0)
			return nPos;
	}
	return i;
}

// Boyer-Moore-Horspool
usize StringView::find(StringView s, usize start) const
{
	static usize table[256] = {0};

	// Basic optimisation
	if (s.length() == 1)
		return find(s[0], start);

	// Fill the skip table
	for (usize i = 0; i < 256; ++i)
		table[i] = s.length();
	for (usize i = 0; i < s.length() - 1; ++i)
		table[(u8)s[i]] = s.length() - i - 1;

	// Search the string
	usize pos = start, i;
	while (pos + s.length() <= m_len) {
		for (i = s.length() - 1; m_chars[pos + i] == s[i]; --i) {
			if (i == 0)
				return pos;
		}
		pos += table[(u8)m_chars[pos + s.length() - 1]];
	}

	return nPos;
}

usize StringView::findLast(StringView s, usize end) const
{
	static usize table[256] = {0};

	// Basic optimisation
	if (s.length() == 1)
		return findLast(s[0], end);

	if (end > m_len)
		end = m_len - 1;

	// Fill the skip table
	for (usize i = 0; i < 256; ++i)
		table[i] = s.length();
	for (usize i = 1; i < s.length(); ++i)
		table[(u8)s[i]] = i;

	// Search the string
	usize pos = end, i;
	while (pos > s.length()) {
		for (i = 0; m_chars[pos - s.length() + 1 + i] == s[i]; ++i) {
			if (i == s.length() - 1)
				return pos - i;
		}
		pos -= table[(u8)m_chars[pos - s.length() + 1]];
	}

	return nPos;
}

usize StringView::findOf(StringView s, usize start) const
{
	for (usize i = start; i < m_len; ++i) {
		if (s.find(m_chars[i]) != nPos)
			return i;
	}
	return nPos;
}

usize StringView::findLastOf(StringView s, usize end) const
{
	usize i = (end >= m_len) ? m_len - 1 : end;
	while (s.find(m_chars[i]) == nPos) {
		if (--i == 0)
			return nPos;
	}
	return i;
}


StringView& StringView::operator =(nullptr_t)
{
	m_chars = nullptr;
	m_len = 0;
	return *this;
}

bool StringView::operator ==(StringView rhs) const
{
	return compare(rhs) == 0;
}

bool StringView::operator !=(StringView rhs) const
{
	return compare(rhs) != 0;
}

bool StringView::operator <(StringView rhs) const
{
	return compare(rhs) < 0;
}

bool StringView::operator <=(StringView rhs) const
{
	return compare(rhs) <= 0;
}

bool StringView::operator >(StringView rhs) const
{
	return compare(rhs) > 0;
}

bool StringView::operator >=(StringView rhs) const
{
	return compare(rhs) >= 0;
}


Writer& operator <<(Writer& os, StringView s)
{
	if (s.null())
		return os << "(nullStrVw)";
	os.write(s.m_chars, s.m_len);
	return os;
}

std::ostream& operator <<(std::ostream& os, StringView s)
{
	if (s.null())
		return os << "(nullStrVw)";
	os.write(s.m_chars, s.m_len);
	return os;
}

}