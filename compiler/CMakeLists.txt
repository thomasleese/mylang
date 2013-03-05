CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(Compiler)

SET(SOURCES	src/main.cpp src/compiler.cpp
			src/lex/analyser.cpp src/lex/rule.cpp src/lex/token.cpp
			src/syntax/parser.cpp src/syntax/statements.cpp src/syntax/expressions.cpp
				src/syntax/operators.cpp)

SET(LIBRARIES boost_regex icuuc icudata)

SET(CMAKE_CXX_FLAGS "-std=c++11")

INCLUDE_DIRECTORIES(include)

ADD_EXECUTABLE(Compiler ${SOURCES})
TARGET_LINK_LIBRARIES(Compiler ${LIBRARIES})
