#include <sstream>

#include "units/ast.h"

using namespace AST;
using namespace AST::Expressions;

Expression::Expression(Lexical::Token *token) {
	this->token = token;
}

Expression::~Expression() {
	
}

Lexical::Token *Expression::getToken() {
	return this->token;
}

Unary::Unary(Lexical::Token *token) : 
	Expression(token) {
	
}

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

Binary::Binary(Lexical::Token *token) : 
	Expression(token) {
	
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

Operand::Operand(Lexical::Token *token) :
	Expression(token) {
	
}

Expr::Expr(Lexical::Token *token) : 
	Operand(token) {
	
}

void Expr::setExpression(Expression *expr) {
	this->expr = expr;
}

Literal::Literal(Lexical::Token *token) : 
	Operand(token) {
	
}

void Literal::setValue(std::string value) {
	this->value = value;
}

Identifier::Identifier(Lexical::Token *token) : 
	Operand(token) {
	
}

void Identifier::setValue(std::string value) {
	this->value = value;
}

std::string Identifier::getValue() const {
	return this->value;
}

Selector::Selector(Lexical::Token *token) : 
	Operand(token) {
	
}

void Selector::setOperand(Operand *expr) {
	this->operand = expr;
}

void Selector::setIdentifier(Identifier *identifier) {
	this->identifier = identifier;
}

Call::Call(Lexical::Token *token) : 
	Operand(token) {
	
}

void Call::setOperand(Operand *expr) {
	this->operand = expr;
}

void Call::addArgument(Expression *arg) {
	this->arguments.push_back(arg);
}

Slice::Slice(Lexical::Token *token) : 
	Operand(token) {
	
}

void Slice::setOperand(Operand *expr) {
	this->operand = expr;
}

void Slice::setIndex(Expression *expr) {
	this->index = expr;
}

Type::Type(Lexical::Token *token) : 
	Expression(token) {
	
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

Parameter::Parameter(Lexical::Token *token) : 
	Expression(token) {
	
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
