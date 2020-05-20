/*
** B, 2019
** Pipe.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	class Pipe;
}

////////////////////////////////////////////////////////////////////////////////

#include <fcntl.h>
#include <unistd.h>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

class Pipe
{
public:
	enum Flags
	{
		CloseOnExec = O_CLOEXEC,
		Direct      = O_DIRECT,
		NonBlock    = O_NONBLOCK,
	};

public:
	Pipe();
	explicit Pipe(Flags flags);
	~Pipe();

	void close();

	// End to which you write
	int in()  const { return m_fds[1]; }
	// End from which you read
	int out() const { return m_fds[0]; }

	bool ok() const { return m_ok; }
	operator bool() const { return ok(); }

private:
	int m_fds[2] = {-1, -1};
	bool m_ok = false;
};

}

////////////////////////////////////////////////////////////////////////////////

/*
** Example:
** ```
** B::Pipe pipe;
** write(pipe.in(), "Hello World!", 12);
**
** char s[32] = {0};
** read(pipe.out(), s, 32);
** B::print("'%$'\n", s);
** ```
*/