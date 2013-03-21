#pragma once

#include <iostream>

#include "lex/token.h"
#include "lex/rule.h"

namespace Lex {

	class Analyser {
		
	public:
		Analyser();
		~Analyser();
		
		void parseFile(std::string filename);
		void dump() const;
		
		std::vector<Token *> getTokens();
		
	private:
		void loadRules();
		
		void readFile(std::string filename);
		void tokeniseBuffer(std::string filename);
		
		int getLineFromIndex(int index);
		
	private:
		std::string buffer;
		std::vector<Token *> tokens;
		std::vector<Rule *> rules;
		
	};

}
