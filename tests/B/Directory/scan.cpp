/*
** B tests, 2020
** Directory / scan.cpp
*/

#include "B/FileSystem/Directory.hpp"
#include <criterion/criterion.h>

////////////////////////////////////////////////////////////////////////////////

Test(Directory, Scan)
{
	auto entries = B::Directory::scan("B/Directory");
	cr_expect(entries.size() == 3);
}