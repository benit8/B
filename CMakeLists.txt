##
## B, 2020
## CMakeLists.txt
##

cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.13")
	cmake_policy(SET CMP0076 NEW)
endif()

project(
	"B"
	VERSION 0.1.0
	LANGUAGES CXX
)

add_library(${PROJECT_NAME})

target_compile_features(${PROJECT_NAME}
PUBLIC
	cxx_std_20
)

target_compile_options(${PROJECT_NAME}
PUBLIC
	-fPIC
	-Wall -Wextra
PRIVATE
	-g3
)

target_include_directories(${PROJECT_NAME}
PUBLIC
	./sources
	./extern
)

target_sources(${PROJECT_NAME}
PUBLIC
	sources/B.hpp
	sources/B/Buffer.hpp
	sources/B/Common.hpp
	sources/B/EnumOperators.hpp
	sources/B/Memory.hpp
	sources/B/OptionParser.hpp
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
	sources/B/IO/Parser.hpp
	sources/B/IO/Print.hpp
	sources/B/IO/Reader.hpp
	sources/B/IO/Stream.hpp
	sources/B/IO/Writer.hpp
	sources/B/Platform/Pipe.hpp
	sources/B/Platform/Process.hpp
	sources/B/Platform/Time.hpp
	sources/B/Platform/ThreadPool.hpp
	extern/tinyformat.hpp

PRIVATE
	sources/B/Buffer.cpp
	sources/B/OptionParser.cpp
	sources/B/String.cpp
	sources/B/StringView.cpp
	sources/B/Containers/Map.inl
	sources/B/Containers/SingleList.inl
	sources/B/Containers/Vector.inl
	sources/B/FileSystem/Directory.cpp
	sources/B/FileSystem/Stat.cpp
	sources/B/IO/BufferReader.cpp
	sources/B/IO/BufferWriter.cpp
	sources/B/IO/FileReader.cpp
	sources/B/IO/FileWriter.cpp
	sources/B/IO/Reader.cpp
	sources/B/IO/StringWriter.cpp
	sources/B/IO/Writer.cpp
	sources/B/Platform/Pipe.cpp
	sources/B/Platform/Process.cpp
	sources/B/Platform/Time.cpp
)

add_subdirectory(tests)