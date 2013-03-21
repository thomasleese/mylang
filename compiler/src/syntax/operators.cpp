#include <sstream>

#include "syntax/operators.h"
#include "unit.h"

using namespace Syntax::Operators;

void Binary::setType(Type t) {
	this->type = t;
	
	switch (this->type) {
		case Assignment:
			this->setPrecedence(6);
			break;
			
		case Multiply:
		case Divide:
		case Modulus:
		case LeftShift:
		case RightShift:
		case BitwiseAnd:
		case BitwiseAndOr:
			this->setPrecedence(5);
			break;
			
		case Add:
		case Subtract:
		case BitwiseOr:
		case BitwiseNot:
			this->setPrecedence(4);
			break;
			
		case GreaterThan:
		case LessThan:
		case Equals:
		case NotEquals:
		case LessThanEquals:
		case GreaterThanEquals:
			this->setPrecedence(3);
			break;
			
		case And:
			this->setPrecedence(2);
			break;
			
		case Or:
			this->setPrecedence(1);
			break;
			
		default:
			this->setPrecedence(0);
			break;
	}
}

Binary::Type Binary::getType() {
	return this->type;
}

void Binary::setPrecedence(int p) {
	this->precedence = p;
}

int Binary::getPrecedence() {
	return this->precedence;
}

std::string Binary::toString() const {
	std::stringstream ss;
	
	ss << FORMAT_BOLD FORMAT_BLUE "Binary" FORMAT_NONE "(";
	ss << FORMAT_YELLOW "type" FORMAT_NONE "=" FORMAT_RED << this->type << FORMAT_NONE " ";
	ss << FORMAT_YELLOW "precedence" FORMAT_NONE "=" FORMAT_RED << this->precedence << FORMAT_NONE;
	ss << ")";
	
	return ss.str();
}
