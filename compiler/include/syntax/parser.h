#pragma once

#include <vector>

namespace Lex {
	class Token;
}

namespace Syntax {
	
	class Parser {
		
	public:
		Parser();
		~Parser();
		
		void parseTokens(std::vector<Lex::Token *> tokens);
		
		void dump();
		
	private:
		void readTokens(std::vector<Lex::Token *> tokens);
		void generateAST();
		
	private:
		std::vector<Lex::Token *> tokens;
		
	};
	
}
