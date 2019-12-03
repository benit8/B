/*
** B, 2019
** StringView.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	class StringView;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/StringBase.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

class StringView : public StringBase<StringView>
{
public:
	StringView();
	StringView(const StringView &other);
	StringView(const char *str);
	StringView(const char *str, usize length);

	inline const char *cStr() const { return m_chars; }
	inline usize length() const { return m_len; }
	inline bool null() const { return m_chars == nullptr; }
	inline bool empty() const { return null() || m_len == 0; }
	inline char operator [](usize i) const { return m_chars[i]; }

	StringView substr(usize start = 0, usize length = nPos) const;
	Vector<StringView> split(const StringView &delimiter, bool keepEmpty = true, usize limit = -1) const;

	int compare(const StringView &s) const;
	bool startsWith(const StringView &s) const;
	bool endsWith(const StringView &s) const;
	usize find(char c, usize start = 0) const;
	usize findLast(char c, usize end = nPos) const;
	usize find(const StringView &s, usize start = 0) const;
	usize findLast(const StringView &s, usize end = nPos) const;
	usize findOf(const StringView &s, usize start = 0) const;
	usize findLastOf(const StringView &s, usize end = nPos) const;

	StringView &operator =(nullptr_t);
	StringView &operator =(const StringView &other);
	bool operator ==(const StringView &rhs) const;
	bool operator !=(const StringView &rhs) const;
	bool operator <(const StringView &rhs) const;
	bool operator <=(const StringView &rhs) const;
	bool operator >(const StringView &rhs) const;
	bool operator >=(const StringView &rhs) const;

	friend std::ostream &operator <<(std::ostream &, const StringView &);

private:
	const char *m_chars = nullptr;
	usize m_len = 0;
};

}