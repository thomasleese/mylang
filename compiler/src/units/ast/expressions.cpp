#include <sstream>

#include "units/ast.h"

using namespace AST;
using namespace AST::Expressions;

void Unary::setOperator(Operators::Unary *op) {
	this->op = op;
}

Operators::Unary *Unary::getOperator() {
	return this->op;
}

void Unary::setExpression(Expression *expr) {
	this->expr = expr;
}

Expression *Unary::getExpression() {
	return this->expr;
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

void Expr::setExpression(Expression *expr) {
	this->expr = expr;
}

void Literal::setValue(std::string value) {
	this->value = value;
}

void Identifier::setValue(std::string value) {
	this->value = value;
}

std::string Identifier::getValue() const {
	return this->value;
}

void Selector::setOperand(Operand *expr) {
	this->operand = expr;
}

void Selector::setIdentifier(Identifier *identifier) {
	this->identifier = identifier;
}

void Call::setOperand(Operand *expr) {
	this->operand = expr;
}

void Call::addArgument(Expression *arg) {
	this->arguments.push_back(arg);
}

void Slice::setOperand(Operand *expr) {
	this->operand = expr;
}

void Slice::setIndex(Expression *expr) {
	this->index = expr;
}

void Type::addName(Identifier *name) {
	this->names.push_back(name);
}

std::vector<Identifier *> Type::getNames() {
	return this->names;
}

void Type::addSlice(Slice *slice) {
	this->slices.push_back(slice);
}

void Parameter::setType(Type *type) {
	this->type = type;
}

Type *Parameter::getType() const {
	return this->type;
}

void Parameter::setIdentifier(Identifier *identifier) {
	this->identifier = identifier;
}
