/*
** B, 2019
** Process.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

namespace B {
	class Process;
}

////////////////////////////////////////////////////////////////////////////////

#include "B/String.hpp"
#include "B/Containers/Vector.hpp"
#include "B/Platform/Pipe.hpp"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

class Process
{
public:
	enum Flags
	{
		None     = 0,
		EmptyEnv = 0x1,
	};

public:
	explicit Process(const Vector<String> &args, Process::Flags flags = None);
	~Process();

	bool create(const Vector<String> &args, Process::Flags flags = None);
	bool join();
	void stop();
	void kill();

	bool running() const;
	int exitCode() const { return m_exitCode; }
	operator bool() const { return running(); }

	int in() const { return m_inPipe.out(); }
	int out() const { return m_outPipe.in(); }
	int err() const { return m_errPipe.in(); }

protected:
	char **buildCStringArray(const Vector<String> &vector);
	void freeCStringArray(char **array);

private:
	pid_t m_pid = -1;
	Pipe m_inPipe, m_outPipe, m_errPipe;
	int m_exitCode = -1;
};

}

////////////////////////////////////////////////////////////////////////////////

/*
** Example: Let a child process run for up to 10 seconds, then kill it
** ```
** B::Process child({"echo", "Hello", "World!"});
** for (usize i = 0; child.running() && i < 10; ++i) {
** 	B::print("Child process is running...\n");
** 	sleep(1);
** }
**
** if (child.running()) {
** 	child.stop();
** 	B::print("Child process exited with code: %$\n", child.exitCode());
** }
** ```
*/