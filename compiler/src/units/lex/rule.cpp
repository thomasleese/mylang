#include <sstream>

#include "units/lex.h"

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
