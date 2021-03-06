#include <sstream>

#include "units/ast.h"

using namespace AST;
using namespace AST::Expressions;

Expression::Expression(Lexical::Token *token) {
	this->token = token;
}

Expression::~Expression() {
	
}

Lexical::Token *Expression::getToken() const {
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

IntegerLiteral::IntegerLiteral(Lexical::Token *token) :
	Literal(token) {
	
}

void IntegerLiteral::setValue(int val) {
	this->value = val;
}

int IntegerLiteral::getValue() const {
	return this->value;
}

StringLiteral::StringLiteral(Lexical::Token *token) :
	Literal(token) {
	
}

void StringLiteral::setValue(std::string val) {
	this->value = val;
}

std::string StringLiteral::getValue() const {
	return this->value;
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

QualifiedIdentifier *Identifier::toQualifiedIdentifier() {
	QualifiedIdentifier *i = new QualifiedIdentifier(this->getToken());
	i->setName(this);
	return i;
}

QualifiedIdentifier::QualifiedIdentifier(Lexical::Token *token) :
	Expression(token) {
	this->name = NULL;
	this->module = NULL;
}

void QualifiedIdentifier::setModule(Identifier *identifier) {
	this->module = identifier;
}

Identifier *QualifiedIdentifier::getModule() {
	return this->module;
}

void QualifiedIdentifier::setName(Identifier *identifier) {
	this->name = identifier;
}

Identifier *QualifiedIdentifier::getName() {
	return this->name;
}

Selector::Selector(Lexical::Token *token) : 
	Operand(token) {
	
}

void Selector::setOperand(Operand *expr) {
	this->operand = expr;
}

Operand *Selector::getOperand() const {
	return this->operand;
}

void Selector::setIdentifier(Identifier *identifier) {
	this->identifier = identifier;
}

Identifier *Selector::getIdentifier() const {
	return this->identifier;
}

Call::Call(Lexical::Token *token) : 
	Operand(token) {
	
}

void Call::setOperand(Operand *expr) {
	this->operand = expr;
}

Operand *Call::getOperand() const {
	return this->operand;
}

void Call::addArgument(Expression *arg) {
	this->arguments.push_back(arg);
}

std::vector<Expression *> Call::getArguments() const {
	return this->arguments;
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
	this->isStruct = false;
}

void Type::setIsStruct(bool s) {
	this->isStruct = s;
}

bool Type::getIsStruct() const {
	return this->isStruct;
}

void Type::setName(QualifiedIdentifier *name) {
	this->name = name;
}

QualifiedIdentifier *Type::getName() {
	return this->name;
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

void Parameter::setName(Identifier *name) {
	this->name = name;
}

Identifier *Parameter::getName() {
	return this->name;
}
