/*
** B, 2019
** Time.cpp
*/

#include "B/Time.hpp"

#include <sys/time.h>

////////////////////////////////////////////////////////////////////////////////

namespace B
{

f64 getTime()
{
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	return (f64)tv.tv_sec + (f64)tv.tv_usec / 1e6;
}

f64 getMilliTime()
{
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	return (f64)tv.tv_sec * 1e3 + (f64)tv.tv_usec / 1e3;
}

f64 getMicroTime()
{
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	return (f64)tv.tv_sec * 1e6 + (f64)tv.tv_usec;
}

}