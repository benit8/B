/*
** B tests, 2020
** Directory.cpp
*/

#include "B/FileSystem/Directory.hpp"
#include <criterion/criterion.h>

using B::Directory;

////////////////////////////////////////////////////////////////////////////////

Test(Directory, Scan)
{
	auto entries = Directory::scan(".");

	cr_expect_eq(-1, entries.find([] (auto &e) { return e.name == "."; }));
	cr_expect_eq(-1, entries.find([] (auto &e) { return e.name == ".."; }));
}

Test(Directory, ScanHidden)
{
	auto entries = Directory::scan(".", Directory::hiddenScanFilter);

	cr_assert_geq(entries.size(), 3, "entries.size() == %zu", entries.size());

	cr_expect_neq(-1, entries.find([] (auto &e) { return e.name == "."; }));
	cr_expect_neq(-1, entries.find([] (auto &e) { return e.name == ".."; }));
}