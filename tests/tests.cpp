/*
** Blib tests, 2019
** main.cpp
*/

#include <iostream>
#include <sstream>
#include <typeinfo>
#include <sys/time.h>
// #include <cassert>

#include "B.hpp"
using namespace B;

extern char *__progname;

////////////////////////////////////////////////////////////////////////////////

#define RED(text)     "\x1b[31m" << text << "\x1b[0m"
#define GREEN(text)   "\x1b[32m" << text << "\x1b[0m"
#define YELLOW(text)  "\x1b[33m" << text << "\x1b[0m"
#define BLUE(text)    "\x1b[34m" << text << "\x1b[0m"
#define MAGENTA(text) "\x1b[35m" << text << "\x1b[0m"
#define CYAN(text)    "\x1b[36m" << text << "\x1b[0m"

std::string assertMessage(const char *assertion, const char *file, const char *func, int line)
{
	std::ostringstream oss;
	// oss << __progname << ": ";
	oss << BLUE(file << ":" << std::to_string(line));
	oss << ": ";
	oss << CYAN(func);
	oss << ": Assertion `" << YELLOW(assertion) << "' failed.";
	return oss.str();
}

#ifndef assert
	#define assert(expr) if (!(expr)) {throw std::runtime_error(assertMessage(#expr, __FILE__, __PRETTY_FUNCTION__, __LINE__));}
#endif


void TEST(const char *name, std::function<void()> tester)
{
	std::cout << MAGENTA("TEST") << " " << BLUE(name) << ": ";
	std::cout.flush();

	try {
		tester();
		std::cout << GREEN("OK") << std::endl;
	}
	catch (std::exception &e) {
		std::cout << RED("KO") << std::endl;
		std::cout << e.what() << std::endl;
	}
}

////////////////////////////////////////////////////////////////////////////////

class TYPE
{
public:
	TYPE() : m_n(0) {}
	TYPE(int n) : m_n(n) {}
	~TYPE() { m_n = -1; }

	int n() const { return m_n; }

	bool operator ==(const TYPE &other) const { return m_n == other.m_n; }
	bool operator !=(const TYPE &other) const { return m_n != other.m_n; }

	bool operator <(const TYPE &other) const { return m_n < other.m_n; }
	bool operator >(const TYPE &other) const { return m_n > other.m_n; }
	bool operator <=(const TYPE &other) const { return m_n <= other.m_n; }
	bool operator >=(const TYPE &other) const { return m_n >= other.m_n; }

private:
	int m_n;
};

std::ostream &operator <<(std::ostream &rhs, const TYPE &lhs) {
	return rhs << '{' << lhs.n() << '}';
}

int main(void)
{
	#include "B/Vector/assign.hpp"
	#include "B/Vector/capacity.hpp"
	#include "B/Vector/elAccess.hpp"
	#include "B/Vector/modifiers.hpp"
	#include "B/Vector/operations.hpp"

	return EXIT_SUCCESS;
}