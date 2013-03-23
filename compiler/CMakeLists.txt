CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(Compiler)

SET(SOURCES	src/main.cpp src/compiler.cpp src/unit.cpp
			src/units/lex/analyser.cpp src/units/lex/rule.cpp src/units/lex/token.cpp
			src/units/ast/parser.cpp src/units/ast/statements.cpp src/units/ast/expressions.cpp src/units/ast/operators.cpp
			src/code/generator.cpp)

SET(LIBRARIES boost_regex icuuc icudata)

execute_process(
	COMMAND llvm-config --cxxflags
	OUTPUT_VARIABLE LLVM_CXX_FLAGS
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
	COMMAND llvm-config --ldflags
	OUTPUT_VARIABLE LLVM_LD_FLAGS
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
	COMMAND llvm-config --libs engine
	OUTPUT_VARIABLE LLVM_LIBS
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS "${LLVM_CXX_FLAGS} -std=c++11 -fexceptions")
SET(CMAKE_EXE_LINKER_FLAGS "${LLVM_LD_FLAGS}")

INCLUDE_DIRECTORIES(include)

ADD_EXECUTABLE(Compiler ${SOURCES})
TARGET_LINK_LIBRARIES(Compiler ${LIBRARIES} ${LLVM_LIBS})
