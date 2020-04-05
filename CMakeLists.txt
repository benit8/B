##
## Benito's lib, 2019
## CMakeLists.txt
##

cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)
cmake_policy(SET CMP0076 NEW)

project(B VERSION 0.1.0 LANGUAGES CXX)
add_library(B SHARED)

target_compile_features(B
PRIVATE
	cxx_std_17
)

target_compile_options(B
PRIVATE
	-nodefaultlibs
	-W -Wall -Wextra
	-g3
)

target_include_directories(B
PUBLIC
	./sources
)

target_sources(B
PUBLIC
	sources/B.hpp
	sources/B/Array.hpp
	sources/B/Associative.hpp
	sources/B/Buffer.hpp
	sources/B/Container.hpp
	sources/B/Common.hpp
	sources/B/Iterators.hpp
	sources/B/Map.hpp
	sources/B/Pair.hpp
	sources/B/Queue.hpp
	sources/B/Sequence.hpp
	sources/B/SingleList.hpp
	sources/B/Stack.hpp
	sources/B/String.hpp
	sources/B/StringView.hpp
	sources/B/Traits.hpp
	sources/B/Types.hpp
	sources/B/Vector.hpp
	sources/B/FS/Directory.hpp
	sources/B/FS/File.hpp
	sources/B/FS/Stat.hpp
	sources/B/IO/Print.hpp
	sources/B/IO/IODevice.hpp
	sources/B/OS/Pipe.hpp
	sources/B/OS/Process.hpp
	sources/B/OS/Time.hpp
	sources/B/OS/ThreadPool.hpp

PRIVATE
	sources/B/Buffer.cpp
	sources/B/Map.inl
	sources/B/Queue.inl
	sources/B/Sequence.inl
	sources/B/SingleList.inl
	sources/B/Stack.inl
	sources/B/String.cpp
	sources/B/StringView.cpp
	sources/B/Vector.inl
	sources/B/FS/Directory.cpp
	sources/B/FS/File.cpp
	sources/B/FS/Stat.cpp
	sources/B/IO/IODevice.cpp
	sources/B/OS/Pipe.cpp
	sources/B/OS/Process.cpp
	sources/B/OS/Time.cpp
)