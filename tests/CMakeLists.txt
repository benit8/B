##
## Blib Tests, 2020
## CMakeLists.txt
##

cmake_minimum_required(VERSION 3.15)

project(
	${CMAKE_PROJECT_NAME}Tests
	LANGUAGES CXX
)

add_executable(${PROJECT_NAME})
add_test(
	NAME ${PROJECT_NAME}
	COMMAND ${PROJECT_NAME}
)

target_compile_features(${PROJECT_NAME}
PUBLIC
	cxx_std_20
)

target_compile_options(${PROJECT_NAME}
PUBLIC
	-Wall -Wextra
	-g3
)

target_include_directories(${PROJECT_NAME}
PUBLIC
	../sources
)

target_sources(${PROJECT_NAME}
PUBLIC
	B/Parser.cpp
	B/Directory/scan.cpp
	B/Vector/assign.cpp
	B/Vector/capacity.cpp
	B/Vector/elementAccess.cpp
	B/Vector/modifiers.cpp
	B/Vector/operations.cpp
)

target_link_libraries(${PROJECT_NAME}
PUBLIC
	B
	criterion
)