#pragma once

#include <boost/regex/icu.hpp>

#include "unit.h"

namespace Lexical {
	
	class Rule {
		
	public:
		enum Type {
			Ignore,
			
			Identifier,
			Operator,
			Delimiter,
			Keyword,
			
			IntegerLiteral,
			FloatLiteral,
			ComplexLiteral,
			BooleanLiteral,
			StringLiteral,
			
		};
		
		Rule(Type type, std::string regex);
		Rule(Type type, boost::u32regex regex);
		
		Type getType() const;
		boost::u32regex getRegex() const;
		
	private:
		Type type;
		boost::u32regex regex;
		
	};
	
	class Token {
		
	public:
		Token(Rule *rule, std::string value);
		
		Rule *getRule() const;
		std::string getValue() const;
		
		void setFilename(std::string filename);
		std::string getFilename();
		
		void setLineNumber(int no);
		int getLineNumber();
		
	private:
		Rule *rule;
		std::string value;
		
		std::string filename;
		int lineNumber;
		
	};
	
	class Analyser : public Unit {
		
	public:
		Analyser();
		~Analyser();
		
		void parseFile(std::string filename);
		
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
