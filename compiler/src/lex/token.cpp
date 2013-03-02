#include "lex/rule.h"
#include "lex/token.h"
#include "formatting.h"

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
	
	ss << FORMAT_BOLD FORMAT_BLUE "Token" FORMAT_NONE "(";
	ss << FORMAT_YELLOW "rule" FORMAT_NONE "=" << this->rule->toString() << "\t";
	ss << FORMAT_YELLOW "lineno" FORMAT_NONE "=" << FORMAT_RED << this->lineNumber << FORMAT_NONE "\t";
	ss << FORMAT_YELLOW "value" FORMAT_NONE "=" << FORMAT_RED "'" << this->value << "'" FORMAT_NONE;
	ss << ")";
	
	return ss.str();
}
