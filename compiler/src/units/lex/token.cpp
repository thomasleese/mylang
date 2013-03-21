#include "units/lex.h"

using namespace Lex;

Token::Token(Rule *rule, std::string value) {
	this->rule = rule;
	this->value = value;
}

Rule *Token::getRule() const {
	return this->rule;
}

std::string Token::getValue() const {
	return this->value;
}

void Token::setFilename(std::string filename) {
	this->filename = filename;
}

std::string Token::getFilename() {
	return this->filename;
}

void Token::setLineNumber(int no) {
	this->lineNumber = no;
}

int Token::getLineNumber() {
	return this->lineNumber;
}
