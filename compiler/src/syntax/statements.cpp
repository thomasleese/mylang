#include <sstream>

#include "syntax/expressions.h"
#include "syntax/statements.h"
#include "formatting.h"

using namespace Syntax::Statements;

void Expression::setExpression(Expressions::Expression *expr) {
	this->expression = expr;
}

std::string Expression::toString() const {
	std::stringstream ss;
	
	ss << FORMAT_BOLD FORMAT_BLUE "Expression" FORMAT_NONE "(";
	ss << FORMAT_YELLOW "expr" FORMAT_NONE "=" << this->expression->toString();
	ss << ")";
	
	return ss.str();
}
