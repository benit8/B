/*
** Blib, 2019
** tests / raw.cpp
*/

#include "B.hpp"
using namespace B;

#include <cstdio>
#include <sys/stat.h>

////////////////////////////////////////////////////////////////////////////////

class ParsedArgs
{
public:
	bool isPresent(const String &name) const { return m_args.contains(name); }
	const String &get(const String &name) const { return m_args[name]; }
	const Vector<String> &getSingleValues() const { return m_singleValues; }

	bool failed() const { return m_failed; }
	operator bool() const { return !failed(); }

private:
	ParsedArgs() {}

	ParsedArgs(bool /**/)
	: m_failed(false)
	{
	}

private:
	bool m_failed = true;
	Map<String, String> m_args;
	Vector<String> m_singleValues;

	friend class ArgParser;
};



class ArgParser
{
public:
	struct Arg
	{
		String name;
		char shortcut = 0;
		String defaultValue;
		String description;
		bool required = false;
		bool single = false;

		// Arg() {}

		// Arg(const String &n)
		// : name(n)
		// , single(true)
		// {}

		// Arg(const String &n, char s, const String &d = "", const String &h = "")
		// : name(n)
		// , shortcut(s)
		// , defaultValue(d)
		// , description(h)
		// , single(false)
		// {}
	};

public:
	ArgParser(const String &programName)
	: m_programName(programName)
	{}

	void add(const String &n, char s, const String &d = "", const String &h = "")
	{
		m_args.insert(n, { n, s, d, h, false, false });
	}

	void addRequired(const String &n, char s, const String &d = "", const String &h = "")
	{
		m_args.insert(n, { n, s, d, h, true, false });
	}

	void addSingle(const String &name)
	{
		m_singleValues.append({ name, 0, "", "", false, true });
	}

	void addSingleRequired(const String &name)
	{
		if (!m_singleValues.empty())
			assert(m_singleValues.back().required == true);
		m_singleValues.append({ name, 0, "", "", true, true });
	}

	ParsedArgs parse(int ac, char **av)
	{
		return {};
	}

private:
	String m_programName;
	Map<String, Arg> m_args;
	Vector<Arg> m_singleValues;
};



int main(int ac, char **av)
{
	// ArgParser args(av[0]);
	// args.add("archive", 'a', "", "same as -dR --preserve=all");
	// args.add("attributes-only", 0, "", "don't copy the file data, just the attributes");
	// args.addSingleRequired("source");
	// args.addSingleRequired("destination");
	// auto res = args.parse(ac, av);
	// if (!res)
	// 	fprintf(stderr, "Args parsing failed\n");


	// File f(ac > 1 ? av[1] : "run.sh");
	// if (!f.open(OpenMode::ReadOnly)) {
	// 	eprint("%$: %m / %$\n", f.filename(), f.errorString());
	// 	return 1;
	// }

	// const char *data = "  String m_programName; Map<String, Arg> m_args;	Vector<Arg> m_singleValues;";
	// auto ibs = BufferStream((const byte*)data, 76);
	// String word;
	// while (ibs >> word)
	// 	print("b: '%$'\n", word);


	// print("\n");


	std::istringstream iss("true  String m_programName; Map<String, Arg> m_args;	Vector<Arg> m_singleValues;");
	bool b = 0;
	while (iss >> b)
		print("b: '%$'\n", b);

	print("good:%$ oef:%$ fail:%$ bad:%$\n", iss.good(), iss.eof(), iss.fail(), iss.bad());

	return 0;
}