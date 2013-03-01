#include <sstream>

#include "lex/rule.h"

using namespace Lex;

Rule::Rule(Type type, std::string regex) {
	this->type = type;
	this->regex = boost::make_u32regex(regex);
}

Rule::Rule(Type type, boost::u32regex regex) {
	this->type = type;
	this->regex = regex;
}

Rule::Type Rule::getType() const {
	return this->type;
}

boost::u32regex Rule::getRegex() const {
	return this->regex;
}

std::string Rule::toString() const {
	std::stringstream ss;
	
	ss << "Rule(";
	
	switch (this->type) {
		case Identifier:			ss << "Identifier"; break;
		case Operator:				ss << "Operator"; break;
		case Delimiter:				ss << "Delimiter"; break;
		case Keyword:				ss << "Keyword"; break;
		case Comment:				ss << "Comment"; break;
		
		case IntegerLiteral:		ss << "IntegerLiteral"; break;
		case FloatLiteral:			ss << "FloatLiteral"; break;
		case ImaginaryLiteral:		ss << "ImaginaryLiteral"; break;
		case BooleanLiteral:		ss << "BooleanLiteral"; break;
		case StringLiteral:			ss << "StringLiteral"; break;
	}
	
	ss << ")";
	
	return ss.str();
}
