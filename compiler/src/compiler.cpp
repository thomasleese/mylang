#include "lex/analyser.h"
#include "compiler.h"

void Compiler::compile(std::string filename) {
	Lex::Analyser analyser(filename);
	for (Lex::Token *token : analyser.getTokens()) {
		std::cout << token->toString() << std::endl;
	}
	
	std::cout << std::endl;
}
