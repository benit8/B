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

String::String(String &&other)
{
	assign(std::move(other));
}

String::String(const String &other)
{
	assign(other);
}

String::String(const std::string &str)
{
	assign(str.c_str(), str.length());
}

String::String(const char *str)
{
	assign(str, strlen(str));
}

String::String(const char *buffer, usize length)
{
	assign(buffer, length);
}

String::String(char c, usize n)
{
	fill(c, n);
}

String::~String()
{
	reset();
}

////////////////////////////////////////////////////////////////////////////////

char &String::at(usize i)
{
	if (i >= length())
		throw std::out_of_range(format("String::at(): i(%$) >= length(%$)", i, length()));
	return data()[i];
}

const char &String::at(usize i) const
{
	if (i >= length())
		throw std::out_of_range(format("String::at(): i(%$) >= length(%$)", i, length()));
	return data()[i];
}

////////////////////////////////////////////////////////////////////////////////

void String::assign(String &&s)
{
	clear();
	if (m_data)
		delete[] m_data;

	m_data = std::move(s.m_data);
	m_size = std::move(s.length());
	m_capacity = std::move(s.capacity());

	s.m_data = nullptr;
	s.m_size = s.m_capacity = 0;
}

void String::assign(const String &s)
{
	reserve(s.length());
	::memcpy(m_data, s.cStr(), s.length());
	m_data[s.length()] = '\0';
	m_size = s.length();
}

void String::assign(const char *s)
{
	assign(s, strlen(s));
}

void String::assign(const char *buffer, usize length)
{
	reserve(length);
	::memcpy(m_data, buffer, length);
	m_size = length;
	m_data[length] = '\0';
}

void String::fill(char c, usize len, bool append)
{
	reserve(append ? length() + len : len);

	if (!append)
		m_size = 0;

	for (usize i = 0; i < len; ++i)
		m_data[m_size++] = c;
	m_data[length()] = '\0';
}

void String::insert(usize pos, const String &s)
{
	if (s.empty())
		return;

	reserve(length() + s.length());
	if (pos < length())
		::memmove(slot(pos + s.length()), slot(pos), length() - pos);
	::memcpy(slot(pos), s.cStr(), s.length());

	m_size += s.length();
	m_data[length()] = '\0';
}

void String::insert(usize pos, const char *s)
{
	insert(pos, s, strlen(s));
}

void String::insert(usize pos, const char *s, size_t l)
{
	if (l == 0)
		return;

	reserve(length() + l);
	if (pos < l)
		::memmove(slot(pos + l), slot(pos), length() - pos);
	::memcpy(slot(pos), s, l);

	m_size += l;
	m_data[length()] = '\0';
}

void String::insert(usize pos, char c, usize n /*1*/)
{
	if (n == 0)
		return;

	reserve(length() + n);
	if (pos < length())
		::memmove(slot(pos + n), slot(pos), length() - pos);
	::memset(slot(pos), c, n);
	m_size += n;
	m_data[length()] = '\0';
}

void String::erase(usize start /*0*/, usize len /*-1*/)
{
	if (empty() || len == 0 || start >= length())
		return;

	if (start + len > length())
		len = length() - start;
	if (start + len < length())
		::memmove(slot(start), slot(start + len), length() - len);

	m_size -= len;
	m_data[length()] = '\0';
}

void String::replace(usize start, usize len, const char *repl, usize replLen)
{
	if (empty() || len == 0 || start >= length())
		return;

	if (start + len > length())
		len = length() - start;

	usize end = start + len;
	if (replLen != len && end < length())
		::memmove(slot(start + replLen), slot(end), length() - end);
	::memcpy(slot(start), repl, replLen);

	m_size += replLen - len;
	m_data[length()] = '\0';
}

void String::replace(const String &search, const String &replacement)
{
	auto res = join(split(search), replacement);
	assign(std::move(res));
}

void String::repeat(usize times)
{
	assert(times != 0);
	if (times == 1)
		return;

	const usize originalLength = length();
	reserve(originalLength * times);

	for (usize i = 0; i < times; ++i) {
		::memcpy(slot(length()), slot(0), originalLength);
		m_size += originalLength;
	}

	m_data[length()] = '\0';
}

void String::reverse()
{
	for (usize i = 0; i < length() / 2; ++i)
		std::swap(at(i), at(length() - i - 1));
}

void String::shuffle()
{
	for (usize i = length() - 1; i > 0; ++i) {
		usize index = std::rand() % (i + 1);
		std::swap(at(i), at(index));
	}
}

void String::padRight(usize totalLength, const char *pad, usize padLength)
{
	if (totalLength <= length() || padLength == 0)
		return;

	reserve(totalLength);

	usize lengthToPad = totalLength - length();
	for (size_t i = 0; i < lengthToPad; ++i)
		m_data[m_size + i] = pad[i % padLength];

	m_size += lengthToPad;
	m_data[size()] = '\0';
}

void String::padLeft(usize totalLength, const char *pad, usize padLength)
{
	if (totalLength <= length() || padLength == 0)
		return;

	reserve(totalLength);

	usize lengthToPad = totalLength - length();
	::memmove(slot(lengthToPad), slot(0), length());
	for (size_t i = 0; i < lengthToPad; ++i)
		m_data[i] = pad[i % padLength];

	m_size += lengthToPad;
	m_data[size()] = '\0';
}

void String::padCenter(usize totalLength, const char *pad, usize padLength)
{
	if (totalLength <= length() || padLength == 0)
		return;

	// Allocate the total length before hand, so the "sub-pads" don't do it
	reserve(totalLength);

	usize lengthToPad = totalLength - length();
	// Pad left first because it will moves less characters
	padLeft(length() + lengthToPad / 2, pad, padLength);
	padRight(length() + (lengthToPad / 2 + (lengthToPad % 2)), pad, padLength);
}

void String::trimRight()
{
	if (empty())
		return;

	while (isspace(at(length() - 1)))
		m_size--;
	m_data[length()] = '\0';
}

void String::trimLeft()
{
	if (empty())
		return;

	usize i = 0;
	for (; isspace(at(i)); ++i);
	if (i == 0)
		return;

	::memmove(slot(0), slot(i), length() - i);
	m_size -= i;
	m_data[length()] = '\0';
}

void String::trim()
{
	// Start by right-trimming to minimize the number of characters that left-trimming should move
	trimRight();
	trimLeft();
}

void String::toLower()
{
	for (usize i = 0; i < length(); ++i)
		at(i) = tolower(at(i));
}

void String::toUpper()
{
	for (usize i = 0; i < length(); ++i)
		at(i) = toupper(at(i));
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

	// ceil division without math function
	usize chunkCount = length() / chunkLength + (length() % chunkLength);

	Vector<String> chunks;
	chunks.reserve(chunkCount);
	for (usize i = 0; i < length(); i += chunkLength)
		chunks.append(substr(i, chunkLength));

	return chunks;
}

Vector<String> String::split(const char *delimiter, usize delimiterLength, bool keepEmptySpans, usize limit) const
{
	if (empty())
		return {};

	Vector<String> spans;

	usize pos = 0, lastPos = 0;
	while (spans.size() < limit && (pos = find(delimiter, delimiterLength, pos)) != nPos) {
		if (pos - lastPos > 0 || keepEmptySpans)
			spans.append(substr(lastPos, pos - lastPos));
		pos += delimiterLength;
		lastPos = pos;
	}
	if (length() - lastPos > 0 || keepEmptySpans)
		spans.append(substr(lastPos));
	return spans;
}

String String::join(const Vector<String> &spans, const char *glue, usize glueLength)
{
	if (spans.empty())
		return {};

	usize len = 0;
	for (const auto &span : spans)
		len += span.length();
	len += glueLength * (spans.size() - 1);

	String res;
	res.reserve(len);
	res += spans[0];
	for (usize i = 1; i < spans.size(); ++i) {
		res += glue;
		res += spans[i];
	}
	return res;
}

////////////////////////////////////////////////////////////////////////////////

bool String::startsWith(const String &s) const
{
	if (s.empty())
		return true;
	if (empty() || s.length() > length())
		return false;
	return ::strncmp(cStr(), s.cStr(), s.length()) == 0;
}

bool String::startsWith(const char *s) const
{
	usize sl = strlen(s);
	if (s == nullptr || sl == 0)
		return true;
	if (empty() || sl > length())
		return false;
	return ::strncmp(cStr(), s, sl) == 0;
}

bool String::startsWith(char c) const
{
	if (empty())
		return false;
	return at(0) == c;
}

bool String::endsWith(const String &s) const
{
	if (s.empty())
		return true;
	if (empty() || s.length() > length())
		return false;
	return ::strncmp(cStr() + length() - s.length(), s.cStr(), s.length()) == 0;
}

bool String::endsWith(const char *s) const
{
	usize sl = strlen(s);
	if (s == nullptr || sl == 0)
		return true;
	if (empty() || sl > length())
		return false;
	return ::strncmp(cStr() + length() - sl, s, sl) == 0;
}

bool String::endsWith(char c) const
{
	if (empty())
		return false;
	return at(length() - 1) == c;
}

// Boyer-Moore-Horspool
usize String::find(const char *s, usize l, usize start) const
{
	static usize table[256] = {0};

	// Basic optimisation
	if (l == 1)
		return find(s[0], start);

	// Fill the skip table
	for (usize i = 0; i < 256; ++i)
		table[i] = l;
	for (usize i = 0; i < l - 1; ++i)
		table[(u8)s[i]] = l - i - 1;

	// Search the string
	usize pos = start;
	while (pos + l <= length()) {
		for (usize i = l - 1; m_data[pos + i] == s[i]; --i) {
			if (i == 0)
				return pos;
		}
		pos += table[(u8)m_data[pos + l - 1]];
	}

	return nPos;
}

usize String::find(char c, usize start) const
{
	for (usize i = start; i < length(); ++i) {
		if (m_data[i] == c)
			return i;
	}
	return nPos;
}

usize String::findLast(const char *s, usize l, usize end) const
{
	static usize table[256] = {0};

	// Basic optimisation
	if (l == 1)
		return findLast(s[0], end);

	if (end >= length())
		end = length() - 1;

	// Fill the skip table
	for (usize i = 0; i < 256; ++i)
		table[i] = l;
	for (usize i = 1; i < l; ++i)
		table[(u8)s[i]] = i;

	// Search the string
	usize pos = end;
	while (pos > l) {
		for (usize i = 0; m_data[pos - l + 1 + i] == s[i]; ++i) {
			if (i == l - 1)
				return pos - i;
		}
		pos -= table[(u8)m_data[pos - l + 1]];
	}

	return nPos;
}

usize String::findLast(char c, usize end) const
{
	usize i = (end >= length()) ? length() - 1 : end;
	while (m_data[i] != c) {
		if (--i == 0)
			return nPos;
	}
	return i;
}

usize String::findOf(const char *s, usize l, usize start) const
{
	for (usize i = start; i < length(); ++i) {
		for (usize j = 0; j < l; ++j) {
			if (at(i) == s[j])
				return i;
		}
	}
	return nPos;
}

usize String::findLastOf(const char *s, usize l, usize end) const
{
	if (end >= length())
		end = length() - 1;

	for (isize i = end; i >= 0; ++i) {
		for (usize j = 0; j < l; ++j) {
			if (at(i) == s[j])
				return i;
		}
	}
	return nPos;
}

////////////////////////////////////////////////////////////////////////////////

int String::compare(const char *s, usize l) const
{
	if (null())
		return (s == nullptr || l == 0) ? 0 : -1;
	else if (s == nullptr || l == 0)
		return 1;
	return strncmp(m_data, s, l);
}

int String::caseCompare(const char *s, usize l) const
{
	if (null())
		return (s == nullptr || l == 0) ? 0 : -1;
	else if (s == nullptr || l == 0)
		return 1;
	return strncasecmp(m_data, s, l);
}

////////////////////////////////////////////////////////////////////////////////

String String::operator +(const String &rhs)
{
	String s(*this);
	s.append(rhs);
	return s;
}

String String::operator +(const char *rhs)
{
	String s(*this);
	s.append(rhs);
	return s;
}

String String::operator +(char rhs)
{
	String s(*this);
	s.append(rhs);
	return s;
}

String String::operator *(usize rhs)
{
	String s(*this);
	s.repeat(rhs);
	return s;
}

////////////////////////////////////////////////////////////////////////////////

Writer &operator <<(Writer &os, const String &s)
{
	if (s.null())
		return os << "(null)";
	os.write(s.cStr(), s.length());
	return os;
}

std::ostream &operator <<(std::ostream &os, const String &s)
{
	if (s.null())
		return os << "(null)";
	os.write(s.cStr(), s.length());
	return os;
}

////////////////////////////////////////////////////////////////////////////////

}