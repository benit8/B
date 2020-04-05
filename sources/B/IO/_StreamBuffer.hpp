/*
** B, 2019
** StreamBuffer.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	class StreamBuffer;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/Types.hpp"
#include "B/Buffer.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

class StreamBuffer
{
public:
	StreamBuffer();
	virtual ~StreamBuffer();

	// Input
	int get();  // ret and bump
	int peek(); // ret
	int next(); // bump and ret
	void putback(byte c);
	void unget(); // seek -1 and return curr byte
	virtual usize available() = 0;
	virtual usize get(byte *s, usize n) = 0;
	virtual int underflow() = 0;

	// Output
	/// If no put-pointer, or pp at end: overflow char. Else put and bump
	int put(byte c) { return put(&c, 1) == 1 ? c : EOF; }
	virtual usize put(const byte *s, usize n) = 0;
	virtual int overflow(int c = EOF) = 0;

	virtual void setBuffer(const Buffer &buffer) = 0;
	virtual void seekOffset(isize offset) = 0;
	virtual void seekPosition(usize position) = 0;
	virtual void sync() = 0;

	/// TODO:
	/// Decide what the fuck to do about those pointer things, that needs to be
	/// bumped by member functions.
	/// Investigate std implementation?
	/// http://www.cplusplus.com/reference/streambuf/basic_streambuf/

protected:
	Buffer m_buffer;
	usize m_readPos = 0;
	usize m_writePos = 0;
};

}