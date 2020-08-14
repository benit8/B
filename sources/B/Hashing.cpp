/*
** B, 2020
** Hashing.cpp
*/

#include "B/Hashing.hpp"

////////////////////////////////////////////////////////////////////////////////

namespace B
{

u32 hash_int(u32 x)
{
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	x = ((x >> 16) ^ x) * 0x45d9f3b;
	return (x >> 16) ^ x;
}

// u32 hash_int(u64 x)
// {
// 	u32 a = x & 0xFFFFFFFF;
// 	u32 b = x >> 32;
// 	return hash_int((hash_int(a) * 209) ^ hash_int(b * 413));
// }

u32 hash_data(const void* data, usize length)
{
	if (data == nullptr)
		return 0;

	auto p = static_cast<const u8*>(data);

	u32 hash = 0x811c9dc5;
	for (usize i = 0; i < length; ++i) {
		hash ^= p[i];
		hash *= 0x01000193;
	}
	return hash;
}

u32 hash_string(const char* s)
{
	return hash_data(s, __builtin_strlen(s));
}

}