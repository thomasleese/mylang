#pragma once

#include <iostream>

#include "lex/token.h"
#include "lex/rule.h"

namespace Lex {

	class Analyser {
		
	public:
		Analyser(std::string filename);
		~Analyser();
		
		std::vector<Token *> &getTokens();
		
	private:
		void loadRules();
		void readFile(std::string filename);
		void tokeniseBuffer();
		int getLineFromIndex(int index);
		
	private:
		std::string buffer;
		std::vector<Token *> tokens;
		std::vector<Rule *> rules;
		
	};

}
