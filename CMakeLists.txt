cmake_minimum_required(VERSION 3.25)
project(grammar_analysis CXX C)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER "g++")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/common)
add_subdirectory (${PROJECT_SOURCE_DIR}/src/lexical_analysis)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/regex)
#add_subdirectory(${PROJECT_SOURCE_DIR}/src/automata)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/LL1)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/compiler)

add_executable(main ${PROJECT_SOURCE_DIR}/src/test/main.cpp)
add_executable(test2 ${PROJECT_SOURCE_DIR}/src/test/test2.cpp)
add_executable(class ${PROJECT_SOURCE_DIR}/src/test/class.cpp)

#file
target_link_libraries(main compiler LL1 lexical_analysis regex  common)
#folder
include_directories(
	${PROJECT_SOURCE_DIR}/src/regex
	${PROJECT_SOURCE_DIR}/src/lexical_analysis
	${PROJECT_SOURCE_DIR}/src/common
	${PROJECT_SOURCE_DIR}/src/LL1
	${PROJECT_SOURCE_DIR}/src/compiler
	
)
