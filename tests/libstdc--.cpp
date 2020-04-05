#include <stdlib.h>

extern "C" void __cxa_pure_virtual()
{
	abort();
}

void *operator new(size_t size)
{
	return malloc(size);
}

void *operator new[](size_t size)
{
	return malloc(size);
}

void operator delete(void *ptr)
{
	if (ptr != nullptr)
		free(ptr);
}

void operator delete[](void *ptr)
{
	if (ptr != nullptr)
		free(ptr);
}

void operator delete(void *ptr, size_t)
{
	if (ptr != nullptr)
		free(ptr);
}