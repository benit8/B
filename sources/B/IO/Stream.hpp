/*
** B, 2019
** Stream.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	class Stream;
}

////////////////////////////////////////////////////////////////////////////////

#include "IODevice.hpp"
#include "B/String.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

class Stream : public IODevice
{
public:
	enum State : int
	{
		Good = 0,        // Everything's fine
		Eof  = (1 << 0), // Nothing more to read
		Fail = (1 << 1), // A reading/writing operation failed
		Bad  = (1 << 2), // An unrecoverable problem occured
	};

public:
	virtual ~Stream() {}

	void ignore(int delim = EOF, usize limit = -1);

	Stream &operator >>(bool &val);
	Stream &operator <<(bool val);
	Stream &operator >>(i8 &val);
	Stream &operator <<(i8 val);
	Stream &operator >>(u8 &val);
	Stream &operator <<(u8 val);
	Stream &operator >>(i16 &val);
	Stream &operator <<(i16 val);
	Stream &operator >>(u16 &val);
	Stream &operator <<(u16 val);
	Stream &operator >>(i32 &val);
	Stream &operator <<(i32 val);
	Stream &operator >>(u32 &val);
	Stream &operator <<(u32 val);
	Stream &operator >>(i64 &val);
	Stream &operator <<(i64 val);
	Stream &operator >>(u64 &val);
	Stream &operator <<(u64 val);
	Stream &operator >>(f32 &val);
	Stream &operator <<(f32 val);
	Stream &operator >>(f64 &val);
	Stream &operator <<(f64 val);
	Stream &operator >>(f128 &val);
	Stream &operator <<(f128 val);
	Stream &operator >>(String &val);
	Stream &operator <<(const String &val);
	Stream &operator <<(const char *val);

	bool good() const { return m_state == State::Good; }
	bool eof() const { return m_state & State::Eof; }
	bool fail() const { return m_state & State::Fail; }
	bool bad() const { return m_state & State::Bad; }
	State state() const { return m_state; }
	void clearState() { m_state = State::Good; }

	operator bool() const { return !fail(); }
	bool operator !() const { return fail(); }

protected:
	Stream() {}
	bool open(OpenMode mode); // Pure from IODevice
	virtual bool canRead(usize n = 1);
	virtual bool canWrite(usize n = 1);

	bool parseBoolean();
	i64 parseSigned();
	u64 parseUnsigned();
	f128 parseDecimal();
	void printSigned(i64 value);
	void printUnsigned(u64 value);
	void printDecimal(f128 value);

	State m_state = State::Good;

public:
	static Stream Null, Random;
};

////////////////////////////////////////////////////////////////////////////////

inline Stream::State operator ~(Stream::State rhs)
{
	return Stream::State(~static_cast<int>(rhs));
}

inline Stream::State operator|(Stream::State lhs, Stream::State rhs)
{
	return Stream::State(static_cast<int>(lhs) | static_cast<int>(rhs));
}

inline Stream::State &operator|=(Stream::State &lhs, Stream::State rhs)
{
	lhs = Stream::State(static_cast<int>(lhs) | static_cast<int>(rhs));
	return lhs;
}

inline Stream::State operator&(Stream::State lhs, Stream::State rhs)
{
	return Stream::State(static_cast<int>(lhs) & static_cast<int>(rhs));
}

inline Stream::State &operator&=(Stream::State &lhs, Stream::State rhs)
{
	lhs = Stream::State(static_cast<int>(lhs) & static_cast<int>(rhs));
	return lhs;
}

}