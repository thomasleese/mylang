#include "lex/rule.h"
#include "lex/token.h"

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

void Token::setLineNumber(int no) {
	this->lineNumber = no;
}

int Token::getLineNumber() {
	return this->lineNumber;
}

std::string Token::toString() const {
	std::stringstream ss;
	ss << "Token(";

	ss << "rule=" << this->rule->toString() << " ";
	ss << "value=" << this->value << " ";
	ss << "lineno=" << this->lineNumber;

	ss << ")";
	return ss.str();
}
