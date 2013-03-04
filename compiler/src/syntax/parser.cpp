#include "lex/token.h"
#include "lex/rule.h"
#include "syntax/statements.h"
#include "syntax/parser.h"

using namespace Syntax;

Parser::Parser() {
	
}

Parser::~Parser() {
	
}

void Parser::parseTokens(std::vector<Lex::Token *> tokens) {
	readTokens(tokens);
	generateAST();
}

void Parser::dump() {
	for (Statements::Statement *s : this->statements) {
		std::cout << s->toString() << std::endl;
	}
}

void Parser::readTokens(std::vector<Lex::Token *> tokens) {
	this->tokens.clear();
	
	for (Lex::Token *token : tokens) {
		if (token->getRule()->getType() != Lex::Rule::Comment) {
			this->tokens.push_back(token);
		}
	}
}

void Parser::generateAST() {
	int index = 0;
	for (; index < this->tokens.size(); ) {
		//this->statements.push_back(readStatement(&index));
		readDelimiterToken(&index, ";");
	}
}

// Tokens
bool Parser::isToken(int *index, Lex::Rule::Type type, std::string value) {
	Lex::Token *token = this->tokens[*index];
	if (token->getRule()->getType() == type) {
		if (value == "") {
			return true;
		}
		
		return token->getValue() == value;
	}
	
	return false;
}

Lex::Token *Parser::readToken(int *index, Lex::Rule::Type type, std::string value) {
	Lex::Token *token = this->tokens[*index];
	if (isToken(index, type, value)) {
		(*index)++;
		return token;
	}
	
	throw "ARGH!";
}

bool Parser::isIdentifierToken(int *index) {
	return isToken(index, Lex::Rule::Identifier, "");
}

Lex::Token *Parser::readIdentifierToken(int *index) {
	return readToken(index, Lex::Rule::Identifier, "");
}

bool Parser::isOperatorToken(int *index, std::string op) {
	return isToken(index, Lex::Rule::Operator, op);
}

Lex::Token *Parser::readOperatorToken(int *index, std::string op) {
	return readToken(index, Lex::Rule::Operator, op);
}

bool Parser::isDelimiterToken(int *index, std::string delimiter) {
	return isToken(index, Lex::Rule::Delimiter, delimiter);
}

Lex::Token *Parser::readDelimiterToken(int *index, std::string delimiter) {
	return readToken(index, Lex::Rule::Delimiter, delimiter);
}

bool Parser::isKeywordToken(int *index, std::string keyword) {
	return isToken(index, Lex::Rule::Keyword, keyword);
}

Lex::Token *Parser::readKeywordToken(int *index, std::string keyword) {
	return readToken(index, Lex::Rule::Keyword, keyword);
}

bool Parser::isIntegerLiteralToken(int *index) {
	return isToken(index, Lex::Rule::IntegerLiteral, "");
}

Lex::Token *Parser::readIntegerLiteralToken(int *index) {
	return readToken(index, Lex::Rule::IntegerLiteral, "");
}

bool Parser::isFloatLiteralToken(int *index) {
	return isToken(index, Lex::Rule::FloatLiteral, "");
}

Lex::Token *Parser::readFloatLiteralToken(int *index) {
	return readToken(index, Lex::Rule::FloatLiteral, "");
}

bool Parser::isComplexLiteralToken(int *index) {
	return isToken(index, Lex::Rule::ComplexLiteral, "");
}

Lex::Token *Parser::readComplexLiteralToken(int *index) {
	return readToken(index, Lex::Rule::ComplexLiteral, "");
}

bool Parser::isBooleanLiteralToken(int *index) {
	return isToken(index, Lex::Rule::BooleanLiteral, "");
}

Lex::Token *Parser::readBooleanLiteralToken(int *index) {
	return readToken(index, Lex::Rule::BooleanLiteral, "");
}

bool Parser::isStringLiteralToken(int *index) {
	return isToken(index, Lex::Rule::StringLiteral, "");
}

Lex::Token *Parser::readStringLiteralToken(int *index) {
	return readToken(index, Lex::Rule::StringLiteral, "");
}
