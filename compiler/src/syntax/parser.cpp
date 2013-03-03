#include "lex/token.h"
#include "lex/rule.h"
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
	
}
