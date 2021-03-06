/*
** B, 2019
** String.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	class String;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/StringView.hpp"
#include "B/Containers/Sequence.hpp"
#include "B/IO/Writer.hpp"
#include "tinyformat.hpp"

#include <iostream>
#include <string>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

class String : public Sequence<char>
{
public:
	String();
	String(String&&);
	String(const String&);
	String(const std::string&);
	String(const char*);
	String(const char*, usize);
	String(char c, usize n);
	~String() = default;

	const char* cStr() const { return data(); }
	usize length() const { return size(); }

	char& at(usize i);
	char& first() { return at(0); }
	char& last() { return at(length() - 1); }
	char& operator [](usize i) { return data()[i]; }
	const char& at(usize i) const;
	const char& first() const { return at(0); }
	const char& last() const { return at(length() - 1); }
	const char& operator [](usize i) const { return data()[i]; }

	const char* begin() const { return data(); }
	const char* end() const { return data() + length(); }

	void assign(String&&);
	void assign(const String&);
	void assign(const char*);
	void assign(const char*, usize);

	void fill(char character, usize length, bool append = false);

	void insert(usize pos, const String&);
	void insert(usize pos, const char*);
	void insert(usize pos, const char*, usize);
	void insert(usize pos, char c, usize n = 1);
	template <typename... Args>
	void insertf(usize pos, const char* fmt, const Args&... args)
	{
		insert(pos, tfm::format(fmt, args...));
	}

	void append(const String& s)        { return insert(length(), s); }
	void append(const char* s)          { return insert(length(), s); }
	void append(const char* s, usize l) { return insert(length(), s, l); }
	void append(char c, usize n = 1)    { return insert(length(), c, n); }
	template <typename... Args>
	void appendf(const char* fmt, const Args&... args)
	{
		insert(length(), tfm::format(fmt, args...));
	}

	void prepend(const String& s)        { return insert(0, s); }
	void prepend(const char* s)          { return insert(0, s); }
	void prepend(const char* s, usize l) { return insert(0, s, l); }
	void prepend(char c, usize n = 1)    { return insert(0, c, n); }
	template <typename... Args>
	void prependf(const char* fmt, const Args&... args)
	{
		insert(0, tfm::format(fmt, args...));
	}

	void erase(usize start = 0, usize length = -1);

	void replace(usize start, usize length, const String& s) { replace(start, length, s.cStr(), s.length()); }
	void replace(usize start, usize length, const char* s)   { replace(start, length, s, strlen(s)); }
	void replace(usize start, usize length, const char* s, usize l);
	void replace(const String& search, const String& replacement);

	void repeat(usize n);

	void reverse();

	void shuffle();

	void padRight(usize totalLength, const String& pad)     { padRight(totalLength, pad.cStr(), pad.length()); }
	void padRight(usize totalLength, const char* pad = " ") { padRight(totalLength, pad, strlen(pad)); }
	void padRight(usize totalLength, const char* pad, usize l);

	void padLeft(usize totalLength, const String& pad)     { padLeft(totalLength, pad.cStr(), pad.length()); }
	void padLeft(usize totalLength, const char* pad = " ") { padLeft(totalLength, pad, strlen(pad)); }
	void padLeft(usize totalLength, const char* pad, usize l);

	void padCenter(usize totalLength, const String& pad)     { padCenter(totalLength, pad.cStr(), pad.length()); }
	void padCenter(usize totalLength, const char* pad = " ") { padCenter(totalLength, pad, strlen(pad)); }
	void padCenter(usize totalLength, const char* pad, usize l);

	void trimRight();
	void trimLeft();
	void trim();

	void toLowercase();
	void toUppercase();

	String substr(usize start = 0, usize length = -1) const;

	Vector<String> chunk(usize chunksLength) const;

	Vector<String> split(const String& delim, bool keepEmptySpans = true, usize limit = -1) const;
	Vector<String> split(const char* delim,   bool keepEmptySpans = true, usize limit = -1) const;
	Vector<String> split(const char* delim, usize delimLength, bool keepEmptySpans = true, usize limit = -1) const;

	static String join(const Vector<String>& spans, const String& glue);
	static String join(const Vector<String>& spans, const char* glue);
	static String join(const Vector<String>& spans, const char* glue, usize glueLength);

	bool startsWith(const String& s) const;
	bool startsWith(const char* s) const;
	bool startsWith(char c) const;

	bool endsWith(const String& s) const;
	bool endsWith(const char* s) const;
	bool endsWith(char c) const;

	usize find(const String& s, usize p = 0) const { return find(s.cStr(), s.length(), p); }
	usize find(const char* s, usize p = 0) const   { return find(s, strlen(s), p); }
	usize find(const char* s, usize l, usize p = 0) const;
	usize find(char c, usize p = 0) const;

	usize findLast(const String& s, usize p = nPos) const { return findLast(s.cStr(), s.length(), p); }
	usize findLast(const char* s, usize p = nPos) const   { return findLast(s, strlen(s), p); }
	usize findLast(const char* s, usize l, usize p = nPos) const;
	usize findLast(char c, usize p = nPos) const;

	usize findOf(const String& s, usize p = 0) const { return findOf(s.cStr(), s.length(), p); }
	usize findOf(const char* s, usize p = 0) const   { return findOf(s, strlen(s), p); }
	usize findOf(const char* s, usize l, usize p = 0) const;

	usize findLastOf(const String& s, usize p = nPos) const { return findOf(s.cStr(), s.length(), p); }
	usize findLastOf(const char* s, usize p = nPos) const   { return findOf(s, strlen(s), p); }
	usize findLastOf(const char* s, usize l, usize p = nPos) const;

	int compare(const String& s) const { return compare(s.cStr(), s.length()); }
	int compare(const char* s) const   { return compare(s, strlen(s)); }
	int compare(const char* s, usize l) const;
	// int operator <=>(const String& rhs) const { return compare(rhs); }
	// int operator <=>(const char* rhs) const   { return compare(rhs); }

	int caseCompare(const String& s) const { return compare(s.cStr(), s.length()); }
	int caseCompare(const char* s) const   { return compare(s, strlen(s)); }
	int caseCompare(const char* s, usize l) const;


	String& operator =(String&& s)      { assign(std::move(s)); return *this; }
	String& operator =(const String& s) { assign(s); return *this; }
	String& operator =(const char* s)   { assign(s); return *this; }

	String& operator +=(const String& s) { append(s); return *this; }
	String& operator +=(const char* s)   { append(s); return *this; }
	String& operator +=(char c)          { append(c); return *this; }

	String& operator *=(usize n) { repeat(n); return *this; }

	bool operator ==(const String& rhs) const { return compare(rhs) == 0; }
	bool operator ==(const char* rhs) const   { return compare(rhs) == 0; }
	bool operator !=(const String& rhs) const { return compare(rhs) != 0; }
	bool operator !=(const char* rhs) const   { return compare(rhs) != 0; }
	bool operator  <(const String& rhs) const { return compare(rhs)  < 0; }
	bool operator  <(const char* rhs) const   { return compare(rhs)  < 0; }
	bool operator <=(const String& rhs) const { return compare(rhs) <= 0; }
	bool operator <=(const char* rhs) const   { return compare(rhs) <= 0; }
	bool operator  >(const String& rhs) const { return compare(rhs)  > 0; }
	bool operator  >(const char* rhs) const   { return compare(rhs)  > 0; }
	bool operator >=(const String& rhs) const { return compare(rhs) >= 0; }
	bool operator >=(const char* rhs) const   { return compare(rhs) >= 0; }

	String operator +(const String& rhs);
	String operator +(const char* rhs);
	String operator +(char rhs);

	String operator *(usize rhs);

	Vector<String> operator /(const String& rhs) const;
	Vector<String> operator /(const char* rhs)   const;

	friend Writer& operator <<(Writer& lhs, const String& rhs);
	friend std::ostream& operator <<(std::ostream& lhs, const String& rhs);

	static const usize nPos = -1;

private:
	// Allocate for the null terminator too
	void reserve(usize newSize) override { return Sequence<char>::reserve(newSize + 1); }
};

////////////////////////////////////////////////////////////////////////////////

template <>
struct Traits<String> : public GenericTraits<String>
{
	static u32 hash(const String& s) { return hash_data(s.cStr(), s.length()); };
};

struct CaseInsensitiveStringTraits : public Traits<String>
{
	static bool equals(const String& a, const String& b)
	{
		return a.caseCompare(b) == 0;
	}

	static unsigned hash(const String& s)
	{
		if (s.empty())
			return 0;
		u32 hash = 0x811c9dc5;
		for (auto c : s)
			hash = (hash ^ tolower(c)) * 0x01000193;
		return hash;
	}
};

////////////////////////////////////////////////////////////////////////////////

}