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

#include "B/Sequence.hpp"
#include "B/StringBase.hpp"
#include "B/StringView.hpp"

#include <iostream>
#include <string>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

class String : public Sequence<char>, public StringBase<String>
{
public:
	String();
	String(const String &other);
	String(const StringView &other);
	String(String &&other);
	String(const char *str);
	String(const char *str, usize length);
	String(const std::string &str);
	String(char c, usize n);
	~String();

	inline const char *cStr() const { return this->data(); }
	inline usize length() const { return this->size(); }
	inline bool null() const override { return this->data() == nullptr; }
	inline bool empty() const override { return this->null() || this->size() == 0; }
	inline char operator [](usize i) const { return this->m_data[i]; }
	inline char &operator [](usize i) { return this->m_data[i]; }

	String &assign(const String &s);
	String &assign(const StringView &s);
	String &assign(String &&s);
	String &assign(const char *s);
	String &assign(const char *s, usize l);
	String &fill(char c, usize l, bool a = false);

	String &insert(usize p, const StringView &str);
	String &insert(usize p, char c, usize n = 1);
	// template <typename... Args>
	// String &insertf(usize p, const char *fmt, const Args&... args);
	String &append(const StringView &str) { return insert(size(), str); }
	String &append(char c, usize n = 1) { return insert(size(), c, n); }
	// template <typename... Args>
	// String &appendf(const char *fmt, const Args&... args);
	String &prepend(const StringView &str) { return insert(0, str); }
	String &prepend(char c, usize n = 1) { return insert(0, c, n); }
	// template <typename... Args>
	// String &prependf(const char *fmt, const Args&... args);
	String &erase(usize start = 0, usize length = -1);
	String &replace(usize start, usize length, const StringView &s);
	String &replace(const StringView &search, const StringView &replacement);
	String &repeat(usize n);
	String &reverse();
	String &shuffle();
	String &padRight(usize totalLength, const StringView &p = StringView(" "));
	String &padLeft(usize totalLength, const StringView &p = StringView(" "));
	String &padCenter(usize totalLength, const StringView &p = StringView(" "));
	String &trimRight();
	String &trimLeft();
	String &trim();
	String &toLower();
	String &toUpper();

	String substr(usize start = 0, usize length = -1) const;
	Vector<String> chunk(usize chunksLength = 1) const;
	Vector<String> split(const StringView &delim, bool keepEmpty = true, usize limit = -1) const;
	static String join(const Vector<String> &elements, const StringView &glue);

	template <typename... Args>
	static String format(const char *fmt, const Args&... args) { return format(fmt, args...); }

	bool startsWith(const StringView &s) const;
	bool endsWith(const StringView &s) const;
	usize find(char c, usize p = 0) const;
	usize findLast(char c, usize p = nPos) const;
	usize find(const StringView &s, usize p = 0) const;
	usize findLast(const StringView &s, usize p = nPos) const;
	usize findOf(const StringView &s, usize p = 0) const;
	usize findLastOf(const StringView &s, usize p = nPos) const;

	int compare(const String &s) const;
	int compare(const char *s) const;
	int caseCompare(const String &s) const;
	int caseCompare(const char *s) const;


	String &operator =(const String &s) { assign(s); return *this; }
	String &operator =(const StringView &s) { assign(s.cStr(), s.length()); return *this; }
	String &operator =(String &&s) { assign(std::move(s)); return *this; }
	String &operator =(const char *s) { assign(s); return *this; }
	String &operator +=(char c) { append(c); return *this; }
	String &operator +=(const StringView &s) { append(s); return *this; }
	String &operator *=(usize n) { repeat(n); return *this; }
	bool operator ==(const String &rhs) const;
	bool operator !=(const String &rhs) const;
	bool operator <(const String &rhs) const;
	bool operator <=(const String &rhs) const;
	bool operator >(const String &rhs) const;
	bool operator >=(const String &rhs) const;
	operator StringView() const { return StringView(cStr(), length()); }

	friend String operator +(const String &lhs, const String &rhs);
	friend String operator +(const String &lhs, const char *rhs);
	friend String operator +(const char *lhs, const String &rhs);
	friend String operator +(const String &lhs, char rhs);
	friend String operator +(char lhs, const String &rhs);
	friend String operator *(const String &lhs, usize rhs);
	friend String operator *(usize lhs, const String & rhs);
	friend Vector<String> operator /(const String &lhs, const String &rhs);
	friend std::ostream &operator <<(std::ostream &lhs, const String &rhs);

private:
	void reserve(usize newSize) override;
};

////////////////////////////////////////////////////////////////////////////////

}