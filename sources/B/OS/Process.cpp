/*
** B, 2019
** Process.cpp
*/

#include "Process.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

Process::Process(const Vector<String> &args, Process::Flags flags)
{
	create(args, flags);
}

Process::~Process()
{
}

////////////////////////////////////////////////////////////////////////////////

bool Process::create(const Vector<String> &args, Process::Flags flags)
{
	if (!m_inPipe || !m_outPipe || !m_errPipe)
		return false;

	m_pid = ::fork();
	if (m_pid < 0)
		return false;

	if (m_pid == 0) {
		printf("child(%d) will run '%s'\n", ::getpid(), args[0].cStr());
		// Close the stdin write end
		::close(m_inPipe.in());
		// Map the read end to stdin
		::dup2(m_inPipe.out(), STDIN_FILENO);

		// Close the stdout read end
		::close(m_outPipe.out());
		// Map the write end to stdout
		::dup2(m_outPipe.in(), STDOUT_FILENO);

		// Close the stderr read end
		::close(m_errPipe.in());
		// Map the write end to stdout
		::dup2(m_errPipe.out(), STDERR_FILENO);


		char **cArgs = buildCStringArray(args);
		if (flags & EmptyEnv)
			exit(::execve(cArgs[0], cArgs, environ));
		else
			exit(::execvp(cArgs[0], cArgs));
		freeCStringArray(cArgs);
	}
	else {
		// Close the stdin read end
		::close(m_inPipe.out());
		// Store the stdin write end
		// m_inPipe.open(m_inPipe.in(), OpenMode::WriteOnly);

		// Close the stdout write end
		::close(m_outPipe.in());
		// Store the stdout read end
		// m_outPipe.open(m_outPipe.out(), OpenMode::ReadOnly);

		// Close the stderr write end
		::close(m_errPipe.in());
		// Store the stderr read end
		// m_errPipe.open(m_errPipe.out(), OpenMode::ReadOnly);

		printf("dad: creation OK\n");

		return true;
	}
}

bool Process::join()
{
	::close(m_inPipe.out());

	int status = 0;
	if (::waitpid(m_pid, &status, WUNTRACED | WCONTINUED) == -1)
		return false;

	m_exitCode = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
	return true;
}

void Process::stop()
{
	printf("process stop\n");
	m_inPipe.close();
	m_outPipe.close();
	m_errPipe.close();
	::kill(m_pid, SIGTERM);
}

void Process::kill()
{
	printf("process kill\n");
	m_inPipe.close();
	m_outPipe.close();
	m_errPipe.close();
	::kill(m_pid, SIGKILL);
}


bool Process::running() const
{
	if (m_pid == -1)
		return false;

	printf("checking for run\n");

	int status = 0;
	if (waitpid(m_pid, &status, WNOHANG) == -1) {
		printf("waitpid failed\n");
		return false;
	}

	// FIXME: Eventually stops working

	return !WIFEXITED(status);
}

////////////////////////////////////////////////////////////////////////////////

char **Process::buildCStringArray(const Vector<String> &vector)
{
	char **array = (char**)::calloc(vector.size() + 1, sizeof(char *));
	if (array == NULL)
		return NULL;

	usize i = 0;
	for (; i < vector.size(); ++i) {
		array[i] = ::strdup(vector[i].cStr());
		if (array[i] == NULL) {
			freeCStringArray(array);
			return NULL;
		}
	}

	return array;
}

void Process::freeCStringArray(char **array)
{
	for (usize i = 0; array[i] != NULL; ++i)
		::free(array[i]);
	::free(array);
}

////////////////////////////////////////////////////////////////////////////////

}