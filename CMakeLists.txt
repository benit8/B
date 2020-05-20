##
## B, 2020
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
	-fPIC
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
	sources/B/Buffer.hpp
	sources/B/Common.hpp
	sources/B/EnumOperators.hpp
	sources/B/String.hpp
	sources/B/StringView.hpp
	sources/B/Traits.hpp
	sources/B/Types.hpp
	sources/B/Containers/Array.hpp
	sources/B/Containers/Associative.hpp
	sources/B/Containers/Container.hpp
	sources/B/Containers/Iterators.hpp
	sources/B/Containers/Map.hpp
	sources/B/Containers/Pair.hpp
	sources/B/Containers/Queue.hpp
	sources/B/Containers/Sequence.hpp
	sources/B/Containers/SingleList.hpp
	sources/B/Containers/Stack.hpp
	sources/B/Containers/Vector.hpp
	sources/B/FileSystem/Directory.hpp
	sources/B/FileSystem/File.hpp
	sources/B/FileSystem/Stat.hpp
	sources/B/IO/Print.hpp
	sources/B/Platform/Pipe.hpp
	sources/B/Platform/Process.hpp
	sources/B/Platform/Time.hpp
	sources/B/Platform/ThreadPool.hpp

PRIVATE
	sources/B/Buffer.cpp
	sources/B/String.cpp
	sources/B/StringView.cpp
	sources/B/Containers/Map.inl
	sources/B/Containers/SingleList.inl
	sources/B/Containers/Vector.inl
	sources/B/FileSystem/Directory.cpp
	sources/B/FileSystem/Stat.cpp
	sources/B/Platform/Pipe.cpp
	sources/B/Platform/Process.cpp
	sources/B/Platform/Time.cpp
)