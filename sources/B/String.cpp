/*
** B, 2019
** String.cpp
*/

#include "B/String.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

String::String()
{}

String::String(const String &other)
{
	assign(other);
}

String::String(const StringView &other)
{
	assign(other.cStr(), other.length());
}

String::String(String &&other)
{
	assign(std::move(other));
}

String::String(const char *str)
{
	assign(str, strlen(str));
}

String::String(const char *buffer, usize length)
{
	assign(buffer, length);
}

String::String(const std::string &str)
{
	assign(str.c_str(), str.length());
}

String::String(char c, usize n)
{
	fill(c, n);
}

String::~String()
{
	clear();
}

////////////////////////////////////////////////////////////////////////////////

String &String::assign(const String &s)
{
	reserve(s.length());
	std::memcpy(m_data, s.cStr(), s.length());
	m_data[s.length()] = '\0';
	m_size = s.length();
	return *this;
}

String &String::assign(const StringView &s)
{
	return assign(s.cStr(), s.length());
}

String &String::assign(String &&s)
{
	clear();
	if (m_data)
		delete[] m_data;

	m_data = std::move(s.m_data);
	m_size = std::move(s.length());
	m_capacity = std::move(s.capacity());

	s.m_data = nullptr;
	s.m_size = s.m_capacity = 0;

	return *this;
}

String &String::assign(const char *str)
{
	return assign(str, strlen(str));
}

String &String::assign(const char *buffer, usize length)
{
	reserve(length);
	std::memcpy(m_data, buffer, length);
	m_size = length;
	m_data[length] = '\0';
	return *this;
}

String &String::fill(char c, usize len, bool append)
{
	reserve(append ? length() + len : len);

	if (!append)
		m_size = 0;

	for (usize i = 0; i < len; ++i)
		m_data[m_size++] = c;
	m_data[length()] = '\0';

	return *this;
}

////////////////////////////////////////////////////////////////////////////////

String &String::insert(usize pos, char c, usize n /*1*/)
{
	if (n == 0)
		return *this;

	reserve(length() + n);
	if (pos < length())
		std::memmove(this->slot(pos + n), this->slot(pos), length() - pos);
	std::memset(this->slot(pos), c, n);
	m_size += n;
	m_data[length()] = '\0';
	return *this;
}

String &String::insert(usize pos, const StringView &s)
{
	if (s.empty())
		return *this;

	reserve(length() + s.length());
	if (pos < length())
		std::memmove(this->slot(pos + s.length()), this->slot(pos), length() - pos);
	std::memcpy(this->slot(pos), s.cStr(), s.length());

	m_size += s.length();
	m_data[length()] = '\0';
	return *this;
}

String &String::erase(usize start /*0*/, usize len /*-1*/)
{
	if (empty() || len == 0 || start >= length())
		return *this;

	if (start + len > length())
		len = length() - start;
	if (start + len < length())
		std::memmove(slot(start), slot(start + len), length() - len);

	this->m_size -= len;
	m_data[length()] = '\0';
	return *this;
}

String &String::replace(usize start, usize len, const StringView &replacement)
{
	if (empty() || len == 0 || start >= length())
		return *this;

	if (start + len > length())
		len = length() - start;

	if (replacement.length() != len && start + len < length())
		std::memmove(slot(start + replacement.length()), slot(start + len), length() - len);
	std::memcpy(slot(start), replacement.cStr(), replacement.length());

	m_size += replacement.length() - len;
	m_data[length()] = '\0';
	return *this;
}

String &String::replace(const StringView &search, const StringView &replacement)
{
	return *this = String::join(split(search), replacement);
}

String &String::repeat(usize times)
{
	const usize origSize = length();
	reserve(origSize * times);

	for (usize i = 0; i < times; ++i) {
		std::memcpy(slot(length()), slot(0), origSize);
		m_size += origSize;
	}

	m_data[length()] = '\0';
	return *this;
}

String &String::reverse()
{
	for (usize i = 0; i < length() / 2; ++i)
		std::swap(at(i), at(length() - i - 1));
	return *this;
}

String &String::shuffle()
{
	for (usize i = length() - 1; i > 0; ++i) {
		usize index = rand() % (i + 1);
		std::swap(at(i), at(index));
	}
	return *this;
}

String &String::padRight(usize totalLength, const StringView &pad)
{
	if (totalLength <= length() || pad.length() == 0)
		return *this;

	reserve(totalLength);

	usize padLength = totalLength - length();
	for (size_t i = 0; i < padLength; ++i)
		m_data[m_size + i] = pad[i % pad.length()];

	m_size += padLength;
	m_data[size()] = '\0';
	return *this;
}

String &String::padLeft(usize totalLength, const StringView &pad)
{
	if (totalLength <= length() || pad.length() == 0)
		return *this;

	reserve(totalLength);

	usize padLength = totalLength - length();
	std::memmove(slot(padLength), slot(0), length());
	for (size_t i = 0; i < padLength; ++i)
		m_data[i] = pad[i % pad.length()];

	m_size += padLength;
	m_data[size()] = '\0';
	return *this;
}

String &String::padCenter(usize totalLength, const StringView &pad)
{
	if (totalLength <= length() || pad.length() == 0)
		return *this;

	// Allocate the total length before hand, so the "sub-pads" don't do it
	reserve(totalLength);

	usize padLength = totalLength - length();
	// Pad left first because it will moves less characters
	padLeft(length() + padLength / 2, pad);
	padRight(length() + (padLength / 2 + (padLength % 2)), pad);

	return *this;
}

String &String::trimRight()
{
	if (empty())
		return *this;

	while (isspace(at(length() - 1)))
		m_size--;
	m_data[length()] = '\0';
	return *this;
}

String &String::trimLeft()
{
	if (empty())
		return *this;

	usize i = 0;
	for (; isspace(at(i)); ++i);
	if (i == 0)
		return *this;
	std::memmove(slot(0), slot(i), length() - i);
	m_size -= i;
	m_data[length()] = '\0';
	return *this;
}

String &String::trim()
{
	/// Start by right-trimming to minimize the number of characters that
	/// left-trimming should move
	trimRight();
	trimLeft();
	return *this;
}

String &String::toLower()
{
	for (usize i = 0; i < length(); ++i)
		at(i) = tolower(at(i));
	return *this;
}

String &String::toUpper()
{
	for (usize i = 0; i < length(); ++i)
		at(i) = toupper(at(i));
	return *this;
}

////////////////////////////////////////////////////////////////////////////////

String String::substr(usize start, usize len) const
{
	if (start >= length())
		return {};
	if (start + len > length())
		len = length() - start;
	return String(m_data + start, len);
}

Vector<String> String::chunk(usize chunkLength) const
{
	if (empty() || chunkLength == 0)
		return {};

	// ceil div without math function
	usize chunkCount = length() / chunkLength + (length() % chunkLength);

	Vector<String> chunks;
	chunks.reserve(chunkCount);
	for (usize i = 0; i < length(); i += chunkLength)
		chunks.append(substr(i, chunkLength));

	return chunks;
}

Vector<String> String::split(const StringView &del, bool keepEmpty, usize limit) const
{
	if (empty())
		return {};

	usize pos = 0, lastPos = 0;
	Vector<String> v;
	while (v.size() < limit && (pos = find(del, pos)) != StringBase::nPos) {
		if (pos - lastPos > 0 || keepEmpty)
			v.append(substr(lastPos, pos - lastPos));
		pos += del.length();
		lastPos = pos;
	}
	if (length() - lastPos > 0 || keepEmpty)
		v.append(substr(lastPos));
	return v;
}

String String::join(const Vector<String> &elements, const StringView &glue)
{
	if (elements.empty())
		return {};

	usize len = 0;
	for (usize i = 0; i < elements.size(); ++i)
		len += elements[i].length();
	len += glue.length() * elements.size() - 1;

	String res;
	res.reserve(len + 1);
	res += elements[0];
	for (usize i = 1; i < elements.size(); ++i) {
		res += glue;
		res += elements[i];
	}

	return res;
}

////////////////////////////////////////////////////////////////////////////////

bool String::startsWith(const StringView &s) const
{
	if (s.empty())
		return true;
	if (empty())
		return false;
	if (s.length() > length())
		return false;
	return std::memcmp(m_data, s.cStr(), s.length()) == 0;
}

bool String::endsWith(const StringView &s) const
{
	if (s.empty())
		return true;
	if (empty())
		return false;
	if (s.length() > length())
		return false;
	return std::memcmp(m_data + length() - s.length(), s.cStr(), s.length()) == 0;
}

usize String::find(char c, usize start) const
{
	for (usize i = start; i < length(); ++i) {
		if (m_data[i] == c)
			return i;
	}
	return StringBase::nPos;
}

usize String::findLast(char c, usize end) const
{
	usize i = (end >= length()) ? length() - 1 : end;
	while (m_data[i] != c) {
		if (--i == 0)
			return StringBase::nPos;
	}
	return i;
}


// Boyer-Moore-Horspool
usize String::find(const StringView &s, usize start) const
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
	while (pos + s.length() <= length()) {
		for (i = s.length() - 1; m_data[pos + i] == s[i]; --i) {
			if (i == 0)
				return pos;
		}
		pos += table[(u8)m_data[pos + s.length() - 1]];
	}

	return StringBase::nPos;
}

usize String::findLast(const StringView &s, usize end) const
{
	static usize table[256] = {0};

	// Basic optimisation
	if (s.length() == 1)
		return findLast(s[0], end);

	if (end > length())
		end = length() - 1;

	// Fill the skip table
	for (usize i = 0; i < 256; ++i)
		table[i] = s.length();
	for (usize i = 1; i < s.length(); ++i)
		table[(u8)s[i]] = i;

	// Search the string
	usize pos = end, i;
	while (pos > s.length()) {
		for (i = 0; m_data[pos - s.length() + 1 + i] == s[i]; ++i) {
			if (i == s.length() - 1)
				return pos - i;
		}
		pos -= table[(u8)m_data[pos - s.length() + 1]];
	}

	return StringBase::nPos;
}

usize String::findOf(const StringView &s, usize start) const
{
	for (usize i = start; i < length(); ++i) {
		if (s.find(m_data[i]) != StringBase::nPos)
			return i;
	}
	return StringBase::nPos;
}

usize String::findLastOf(const StringView &s, usize end) const
{
	usize i = (end >= length()) ? length() - 1 : end;
	while (s.find(m_data[i]) == StringBase::nPos) {
		if (--i == 0)
			return StringBase::nPos;
	}
	return i;
}


int String::compare(const String &s) const
{
	return compare(s.cStr());
}

int String::compare(const char *s) const
{
	if (null())
		return s == NULL ? 0 : -1;
	else if (s == NULL)
		return 1;
	return strcmp(m_data, s);
}

int String::caseCompare(const String &s) const
{
	return caseCompare(s.cStr());
}

int String::caseCompare(const char *s) const
{
	if (null())
		return s == NULL ? 0 : -1;
	else if (s == NULL)
		return 1;
	return strcasecmp(m_data, s);
}


////////////////////////////////////////////////////////////////////////////////

bool String::operator ==(const String &rhs) const
{
	if (null())
		return rhs.null();
	else if (rhs.null() || length() != rhs.length())
		return false;
	return std::memcmp(m_data, rhs.cStr(), length()) == 0;
}

bool String::operator !=(const String &rhs) const
{
	return !(*this == rhs);
}

bool String::operator <(const String &rhs) const
{
	return compare(rhs) < 0;
}

bool String::operator <=(const String &rhs) const
{
	return compare(rhs) <= 0;
}

bool String::operator >(const String &rhs) const
{
	return compare(rhs) > 0;
}

bool String::operator >=(const String &rhs) const
{
	return compare(rhs) >= 0;
}

////////////////////////////////////////////////////////////////////////////////

String operator +(const String &lhs, const String &rhs)
{
	return String(lhs).append(rhs);
}

String operator +(const String &lhs, const char *rhs)
{
	return String(lhs).append(rhs);
}

String operator +(const char *lhs, const String &rhs)
{
	return String(rhs).append(lhs);
}

String operator +(const String &lhs, char rhs)
{
	return String(lhs).append(rhs);
}

String operator +(char lhs, const String &rhs)
{
	return String(rhs).append(lhs);
}


String operator *(const String &lhs, usize rhs)
{
	return String(lhs).repeat(rhs);
}

String operator *(usize lhs, const String & rhs)
{
	return String(rhs).repeat(lhs);
}


Vector<String> operator /(const String &lhs, const String &rhs)
{
	return lhs.split(rhs);
}

std::ostream &operator <<(std::ostream &lhs, const String &rhs)
{
	if (rhs.null())
		return lhs << "(nullStr)";
	lhs.write(rhs.cStr(), rhs.length());
	return lhs;
}

////////////////////////////////////////////////////////////////////////////////

void String::reserve(usize newSize)
{
	// Allocate for the null terminator too
	return Sequence<char>::reserve(newSize + 1);
}

////////////////////////////////////////////////////////////////////////////////

}