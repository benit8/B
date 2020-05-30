/*
** B, 2020
** OptionParser.cpp
*/

#include "B/OptionParser.hpp"
#include "B/Traits.hpp"

#include <cassert>
#include <getopt.h>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

////////////////////////////////////////////////////////////////////////////////

OptionParser::OptionParser()
{
	addOption(m_showHelp, 0, "help", "Display this message");
}

////////////////////////////////////////////////////////////////////////////////

bool OptionParser::parse(int argc, char **argv, bool exitOnFailure)
{
	auto printHelpAndExit = [this, argv, exitOnFailure] {
		printHelp(argv[0]);
		if (exitOnFailure)
			exit(1);
	};

	opterr = 0;
	optind = 0;

	Vector<struct option> longopt;
	String shortopt;
	for (size_t i = 0; i < m_options.size(); i++) {
		auto &opt = m_options[i];
		if (opt.longName)
			longopt.append({opt.longName, opt.requiresArgument ? required_argument : no_argument, nullptr, 0});
		if (opt.shortName) {
			shortopt.append(opt.shortName);
			if (opt.requiresArgument)
				shortopt.append(':');
		}
	}
	longopt.append({ NULL, 0, NULL, 0 });

	while (true) {
		int longIndex = -1;
		int c = getopt_long(argc, argv, shortopt.cStr(), longopt.data(), &longIndex);
		if (c == -1)
			break;
		else if (c == '?') {
			printHelpAndExit();
			return false;
		}

		Option *opt = nullptr;
		if (c == 0) {
			assert(longIndex >= 0);
			opt = &m_options[longIndex];
			longIndex = -1;
		}
		else {
			usize pos = m_options.find([c] (auto &o) { return c == o.shortName; });
			assert(pos != Vector<Option>::max);
			opt = &m_options[pos];
		}
		assert(opt != nullptr);

		const char *arg = opt->requiresArgument ? optarg : nullptr;
		if (!opt->acceptor(arg)) {
			eprint("Invalid value for option %$\n", argv[optind - 1]);
			printHelpAndExit();
			return false;
		}
	}

	int leftValuesCount = argc - optind;
	int argsValuesCount[m_args.size()];
	int totalRequiredValues = 0;
	for (usize i = 0; i < m_args.size(); i++) {
		argsValuesCount[i] = m_args[i].minValues;
		totalRequiredValues += m_args[i].minValues;
	}

	if (totalRequiredValues > leftValuesCount) {
		printHelpAndExit();
		return false;
	}
	int extraValuesToDistribute = leftValuesCount - totalRequiredValues;

	for (usize i = 0; i < m_args.size(); i++) {
		int argValuesCount = min(m_args[i].maxValues - m_args[i].minValues, extraValuesToDistribute);
		argsValuesCount[i] += argValuesCount;
		extraValuesToDistribute -= argValuesCount;
		if (extraValuesToDistribute == 0)
			break;
	}

	if (extraValuesToDistribute > 0) {
		printHelpAndExit();
		return false;
	}

	for (size_t i = 0; i < m_args.size(); i++) {
		for (int j = 0; j < argsValuesCount[i]; j++) {
			const char *value = argv[optind++];
			if (!m_args[i].acceptor(value)) {
				eprint("Invalid value for argument %$\n", m_args[i].name);
				printHelpAndExit();
				return false;
			}
		}
	}

	if (m_showHelp) {
		printHelp(argv[0]);
		if (exitOnFailure)
			exit(0);
		return false;
	}

	return true;
}

void OptionParser::printHelp(const char *programName)
{
	auto prettyOptionName = [] (const Option &opt) {
		String name("\t");
		if (opt.shortName) {
			name.append('-');
			name.append(opt.shortName);
			if (opt.longName)
				name.appendf(", --%$", opt.longName);
		}
		else {
			assert(opt.longName != nullptr);
			name.appendf("    --%$", opt.longName);
		}
		if (opt.valueName)
			name.appendf("=%$", opt.valueName);
		return name;
	};


	print("Usage:\n\t%$", programName);
	for (auto &arg : m_args) {
		bool required = arg.minValues > 0;
		bool repeated = arg.maxValues > 1;

		if (required && repeated)
			print(" %$...", arg.name);
		else if (required && !repeated)
			print(" %$", arg.name);
		else if (!required && repeated)
			print(" [%$...]", arg.name);
		else if (!required && !repeated)
			print(" [%$]", arg.name);
	}

	if (m_args.size())
		print("\n\nArguments:\n");
	for (auto &arg : m_args) {
		String name = arg.name;
		name.padRight(30);
		print("\t%$ %$\n", name, arg.help);
	}

	if (m_options.size())
		print("\nOptions:\n");
	for (auto &opt : m_options) {
		auto name = prettyOptionName(opt);
		name.padRight(30);
		print("%$", name);
		if (name.last() != ' ')
			print("\n\t                             ");
		print("%$\n", opt.help);
	}
}

////////////////////////////////////////////////////////////////////////////////

void OptionParser::addOption(Option &&option)
{
	m_options.append(std::move(option));
}

void OptionParser::addOption(bool &value, char shortName, const char *longName, const char *help)
{
	m_options.append({
		false,
		help,
		longName,
		shortName,
		nullptr,
		[&value] (const char *s) {
			assert(s == nullptr);
			value = true;
			return true;
		}
	});
}

void OptionParser::addOption(int &value, char shortName, const char *longName, const char *help, const char *valueName)
{
	m_options.append({
		true,
		help,
		longName,
		shortName,
		valueName,
		[&value](const char *s) {
			value = atoi(s);
			return true;
		}
	});
}

void OptionParser::addOption(String &value, char shortName, const char *longName, const char *help, const char *valueName)
{
	m_options.append({
		true,
		help,
		longName,
		shortName,
		valueName,
		[&value] (const char *s) {
			value = s;
			return true;
		}
	});
}

////////////////////////////////////////////////////////////////////////////////

void OptionParser::addArgument(Argument &&arg)
{
	m_args.append(std::move(arg));
}

void OptionParser::addArgument(int &value, const char *help, const char *name, bool required)
{
	m_args.append({
		help,
		name,
		required ? 1 : 0,
		1,
		[&value] (const char *s) {
			value = atoi(s);
			return true;
		}
	});
}

void OptionParser::addArgument(String &value, const char *help, const char *name, bool required)
{
	m_args.append({
		help,
		name,
		required ? 1 : 0,
		1,
		[&value] (const char *s) {
			value = s;
			return true;
		}
	});
}

void OptionParser::addArgument(Vector<String> &values, const char *help, const char *name, bool required)
{
	m_args.append({
		help,
		name,
		required ? 1 : 0,
		Traits<int>::max(),
		[&values] (const char *s) {
			values.append(s);
			return true;
		}
	});
}

////////////////////////////////////////////////////////////////////////////////

}