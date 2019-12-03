TEST("Directory::scan()", []
{
	auto entries = Directory::scan(".");
	for (usize i = 0; i < entries.size(); ++i)
		std::cout << entries[i].name() << std::endl;
});