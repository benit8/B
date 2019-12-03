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
	${CMAKE_CURRENT_LIST_DIR}/sources/B.hpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/Array.hpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/Associative.hpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/Buffer.hpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/Container.hpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/Common.hpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/Iterators.hpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/Map.hpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/Pair.hpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/Queue.hpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/Sequence.hpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/Stack.hpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/String.hpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/StringView.hpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/Types.hpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/Vector.hpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/FS/Directory.hpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/FS/File.hpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/FS/Path.hpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/FS/Stat.hpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/IO/Print.hpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/IO/IODevice.hpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/OS/Time.hpp
	# ${CMAKE_CURRENT_LIST_DIR}/sources/B/IO/Stream.hpp
	# ${CMAKE_CURRENT_LIST_DIR}/sources/B/IO/FileStream.hpp
	# ${CMAKE_CURRENT_LIST_DIR}/sources/B/IO/BufferStream.hpp

PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/sources/B/Buffer.cpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/Container.inl
	${CMAKE_CURRENT_LIST_DIR}/sources/B/Map.inl
	${CMAKE_CURRENT_LIST_DIR}/sources/B/Queue.inl
	${CMAKE_CURRENT_LIST_DIR}/sources/B/Stack.inl
	${CMAKE_CURRENT_LIST_DIR}/sources/B/String.cpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/StringBase.hpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/StringView.cpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/Time.cpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/Vector.inl
	${CMAKE_CURRENT_LIST_DIR}/sources/B/FS/Directory.cpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/FS/File.cpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/FS/Stat.cpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/IO/IODevice.cpp
	${CMAKE_CURRENT_LIST_DIR}/sources/B/OS/Time.cpp
	# ${CMAKE_CURRENT_LIST_DIR}/sources/B/IO/Stream.cpp
	# ${CMAKE_CURRENT_LIST_DIR}/sources/B/IO/FileStream.cpp
	# ${CMAKE_CURRENT_LIST_DIR}/sources/B/IO/BufferStream.cpp
)