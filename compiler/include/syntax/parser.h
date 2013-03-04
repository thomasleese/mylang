#pragma once

#include <vector>

namespace Lex {
	class Token;
}

namespace Syntax {
	
	namespace Statements {
		class Statement;
	}
	
	class Parser {
		
	public:
		Parser();
		~Parser();
		
		void parseTokens(std::vector<Lex::Token *> tokens);
		
		void dump();
		
	private:
		void readTokens(std::vector<Lex::Token *> tokens);
		void generateAST();
		
		// Tokens
		bool isToken(int *index, Lex::Rule::Type type, std::string value);
		Lex::Token *readToken(int *index, Lex::Rule::Type type, std::string value);
		
		bool isIdentifierToken(int *index);
		Lex::Token *readIdentifierToken(int *index);
		
		bool isOperatorToken(int *index, std::string op);
		Lex::Token *readOperatorToken(int *index, std::string op);
		
		bool isDelimiterToken(int *index, std::string delimiter);
		Lex::Token *readDelimiterToken(int *index, std::string delimiter);
		
		bool isKeywordToken(int *index, std::string keyword);
		Lex::Token *readKeywordToken(int *index, std::string keyword);
		
		bool isIntegerLiteralToken(int *index);
		Lex::Token *readIntegerLiteralToken(int *index);
		
		bool isFloatLiteralToken(int *index);
		Lex::Token *readFloatLiteralToken(int *index);
		
		bool isComplexLiteralToken(int *index);
		Lex::Token *readComplexLiteralToken(int *index);
		
		bool isBooleanLiteralToken(int *index);
		Lex::Token *readBooleanLiteralToken(int *index);
		
		bool isStringLiteralToken(int *index);
		Lex::Token *readStringLiteralToken(int *index);
		
	private:
		std::vector<Lex::Token *> tokens;
		std::vector<Statements::Statement *> statements;
		
	};
	
}
