/*
** B, 2020
** OptionParser.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "B/Function.hpp"
#include "B/String.hpp"
#include "B/Containers/Vector.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

class OptionParser
{
public:
	struct Option
	{
		bool requiresArgument = true;
		const char *help = nullptr;
		const char *longName = nullptr;
		char shortName = 0;
		const char *valueName = nullptr;
		Function<bool(const char*)> acceptor;
	};

	struct Argument
	{
		const char *help = nullptr;
		const char *name = nullptr;
		int minValues = 0;
		int maxValues = 1;
		Function<bool(const char*)> acceptor;
	};

public:
	OptionParser();

	bool parse(int argc, char **argv, bool exitOnFailure = true);
	void printHelp(const char *programName);

	void addOption(Option &&);
	void addOption(bool   &value, char shortName, const char *longName, const char *help);
	void addOption(int    &value, char shortName, const char *longName, const char *help, const char *valueName);
	void addOption(String &value, char shortName, const char *longName, const char *help, const char *valueName);

	void addArgument(Argument &&);
	void addArgument(int            &value, const char *help, const char *name, bool required = true);
	void addArgument(String         &value, const char *help, const char *name, bool required = true);
	void addArgument(Vector<String> &value, const char *help, const char *name, bool required = true);

private:
	Vector<Option> m_options;
	Vector<Argument> m_args;

	bool m_showHelp = false;
};

}
