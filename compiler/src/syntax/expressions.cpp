#include <sstream>

#include "syntax/expressions.h"
#include "formatting.h"

using namespace Syntax;
using namespace Syntax::Expressions;

void Unary::setOperator(Operators::Unary op) {
	this->op = op;
}

Operators::Unary Unary::getOperator() {
	return this->op;
}

void Unary::setExpression(Expression *expr) {
	this->expr = expr;
}

Expression *Unary::getExpression() {
	return this->expr;
}

std::string Unary::toString() const {
	std::stringstream ss;
	
	ss << FORMAT_BOLD FORMAT_BLUE "Unary" FORMAT_NONE "(";
	ss << FORMAT_YELLOW "op" FORMAT_NONE "=" FORMAT_RED << this->op << FORMAT_NONE " ";
	ss << FORMAT_YELLOW "expr" FORMAT_NONE "=" << this->expr->toString();
	ss << ")";
	
	return ss.str();
}

void Binary::setOperator(Operators::Binary *op) {
	this->op = op;
}

Operators::Binary *Binary::getOperator() {
	return this->op;
}

void Binary::setLeft(Expression *expr) {
	this->lhs = expr;
}

Expression *Binary::getLeft() {
	return this->lhs;
}

void Binary::setRight(Expression *expr) {
	this->rhs = expr;
}

Expression *Binary::getRight() {
	return this->rhs;
}

std::string Binary::toString() const {
	std::stringstream ss;
	
	ss << FORMAT_BOLD FORMAT_BLUE "Binary" FORMAT_NONE "(";
	ss << FORMAT_YELLOW "op" FORMAT_NONE "=" << this->op->toString() << " ";
	ss << FORMAT_YELLOW "lhs" FORMAT_NONE "=" << this->lhs->toString() << " ";
	ss << FORMAT_YELLOW "rhs" FORMAT_NONE "=" << this->rhs->toString();
	ss << ")";
	
	return ss.str();
}

void Expr::setExpression(Expression *expr) {
	this->expr = expr;
}

std::string Expr::toString() const {
	std::stringstream ss;
	
	ss << FORMAT_BOLD FORMAT_BLUE "Expression" FORMAT_NONE "(";
	ss << FORMAT_YELLOW "expr" FORMAT_NONE "=" << this->expr->toString();
	ss << ")";
	
	return ss.str();
}

void Literal::setValue(std::string value) {
	this->value = value;
}

std::string Literal::toString() const {
	std::stringstream ss;
	
	ss << FORMAT_BOLD FORMAT_BLUE "Literal" FORMAT_NONE "(";
	ss << FORMAT_YELLOW "value" FORMAT_NONE "=" FORMAT_RED << this->value << FORMAT_NONE;
	ss << ")";
	
	return ss.str();
}

void Identifier::setValue(std::string value) {
	this->value = value;
}

std::string Identifier::toString() const {
	std::stringstream ss;
	
	ss << FORMAT_BOLD FORMAT_BLUE "Identifier" FORMAT_NONE "(";
	ss << FORMAT_YELLOW "value" FORMAT_NONE "=" FORMAT_RED << this->value << FORMAT_NONE;
	ss << ")";
	
	return ss.str();
}

void Selector::setOperand(Operand *expr) {
	this->operand = expr;
}

void Selector::setIdentifier(Identifier *identifier) {
	this->identifier = identifier;
}

std::string Selector::toString() const {
	std::stringstream ss;
	
	ss << FORMAT_BOLD FORMAT_BLUE "Selector" FORMAT_NONE "(";
	ss << FORMAT_YELLOW "operand" FORMAT_NONE "=" << this->operand->toString() << " ";
	ss << FORMAT_YELLOW "identifier" FORMAT_NONE "=" << this->identifier->toString();
	ss << ")";
	
	return ss.str();
}

void Call::setOperand(Operand *expr) {
	this->operand = expr;
}

void Call::addArgument(Expression *arg) {
	this->arguments.push_back(arg);
}

std::string Call::toString() const {
	std::stringstream ss;
	
	ss << FORMAT_BOLD FORMAT_BLUE "Call" FORMAT_NONE "(";
	ss << FORMAT_YELLOW "operand" FORMAT_NONE "=" << this->operand->toString() << " ";
	ss << FORMAT_YELLOW "arguments" FORMAT_NONE "=[";
	
	for (int i = 0; i < this->arguments.size(); i++) {
		ss << this->arguments[i]->toString();
		
		if (i < this->arguments.size() - 1) {
			ss << ", ";
		}
	}
	
	ss << "])";
	
	return ss.str();
}

void Slice::setOperand(Operand *expr) {
	this->operand = expr;
}

void Slice::setIndex(Expression *expr) {
	this->index = expr;
}

std::string Slice::toString() const {
	std::stringstream ss;
	
	ss << FORMAT_BOLD FORMAT_BLUE "Slice" FORMAT_NONE "(";
	ss << FORMAT_YELLOW "operand" FORMAT_NONE "=" << this->operand->toString() << " ";
	ss << FORMAT_YELLOW "index" FORMAT_NONE "=" << this->index->toString();
	ss << ")";
	
	return ss.str();
}

void Type::setIdentifier(Identifier *identifier) {
	this->identifier = identifier;
}

void Type::addSelector(Selector *selector) {
	this->selectors.push_back(selector);
}

void Type::addSlice(Slice *slice) {
	this->slices.push_back(slice);
}

std::string Type::toString() const {
	std::stringstream ss;
	
	ss << FORMAT_BOLD FORMAT_BLUE "Type" FORMAT_NONE "(";
	ss << FORMAT_YELLOW "identifier" FORMAT_NONE "=" << this->identifier->toString() << " ";
	ss << FORMAT_YELLOW "slice" FORMAT_NONE "=[";
	
	for (int i = 0; i < this->slices.size(); i++) {
		ss << this->slices[i]->toString();
		
		if (i < this->slices.size() - 1) {
			ss << ", ";
		}
	}
	
	ss << "])";
	
	return ss.str();
}

void Parameter::setType(Type *type) {
	this->type = type;
}

void Parameter::setIdentifier(Identifier *identifier) {
	this->identifier = identifier;
}

std::string Parameter::toString() const {
	std::stringstream ss;
	
	ss << FORMAT_BOLD FORMAT_BLUE "Parameter" FORMAT_NONE "(";
	ss << FORMAT_YELLOW "type" FORMAT_NONE "=" << this->type->toString() << " ";
	ss << FORMAT_YELLOW "identifier" FORMAT_NONE "=" << this->identifier->toString();
	ss << ")";
	
	return ss.str();
}
