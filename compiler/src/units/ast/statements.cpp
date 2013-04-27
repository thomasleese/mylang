#include <sstream>

#include "units/ast.h"

using namespace AST;
using namespace AST::Statements;

Statement::Statement(Lexical::Token *token) {
	this->token = token;
}

Statement::~Statement() {
	
}

Lexical::Token *Statement::getToken() {
	return this->token;
}

Generic::Generic(Lexical::Token *token) :
	Statement(token) {
	
}

Expression::Expression(Lexical::Token *token) :
	Generic(token) {
	
}

void Expression::setExpression(Expressions::Expression *expr) {
	this->expression = expr;
}

Import::Import(Lexical::Token *token) :
	Statement(token) {
	
}

void Import::setIdentifier(Expressions::Identifier *identifier) {
	this->identifier = identifier;
}

Return::Return(Lexical::Token *token) :
	Generic(token) {
	
}

void Return::setExpression(Expressions::Expression *expr) {
	this->expression = expr;
}

Control::Control(Lexical::Token *token) :
	Generic(token) {
	
}

If::If(Lexical::Token *token) :
	Control(token) {
	this->falseStatement = NULL;
}

void If::setExpression(Expressions::Expression *expr) {
	this->expression = expr;
}

Expressions::Expression *If::getExpression() const {
	return this->expression;
}

void If::setTrueBlock(Blocks::Generic *block) {
	this->trueBlock = block;
}

void If::setFalseBlock(Blocks::Generic *block) {
	this->falseBlock = block;
}

void If::setFalseStatement(If *statement) {
	this->falseStatement = statement;
}

Case::Case(Lexical::Token *token) :
	Control(token) {
	this->expression = NULL;
}

void Case::setExpression(Expressions::Expression *expr) {
	this->expression = expr;
}

void Case::setBlock(Blocks::Generic *block) {
	this->block = block;
}

Switch::Switch(Lexical::Token *token) :
	Control(token) {
	
}

void Switch::setExpression(Expressions::Expression *expr) {
	this->expression = expr;
}

void Switch::addCase(Case *statement) {
	this->cases.push_back(statement);
}

Declaration::Declaration(Lexical::Token *token) :
	Statement(token) {
	this->exported = false;
}

void Declaration::setExported(bool exported) {
	this->exported = exported;
}

bool Declaration::getExported() {
	return this->exported;
}

void Declaration::setName(Expressions::Identifier *name) {
	this->name = name;
}

AST::Expressions::Identifier *Declaration::getName() {
	return this->name;
}

VariableDeclaration::VariableDeclaration(Lexical::Token *token) :
	Declaration(token),
	Generic(token) {
	this->assignment = NULL;
	this->isConstant = false;
}

void VariableDeclaration::setIsConstant(bool constant) {
	this->isConstant = constant;
}

bool VariableDeclaration::getIsConstant() {
	return this->isConstant;
}

void VariableDeclaration::setType(Expressions::Type *type) {
	this->type = type;
}

Expressions::Type *VariableDeclaration::getType() const {
	return this->type;
}

void VariableDeclaration::setAssignment(Expressions::Expression *expr) {
	this->assignment = expr;
}

TypeDeclaration::TypeDeclaration(Lexical::Token *token) :
	Declaration(token) {
	
}

void TypeDeclaration::setType(Expressions::Type *type) {
	this->type = type;
}

Expressions::Type *TypeDeclaration::getType() {
	return this->type;
}

void TypeDeclaration::setBlock(Blocks::Type *block) {
	this->block = block;
}

Blocks::Type *TypeDeclaration::getBlock() const {
	return this->block;
}

FunctionDeclaration::FunctionDeclaration(Lexical::Token *token) :
	Declaration(token) {
	
}

void FunctionDeclaration::setType(Expressions::Type *type) {
	this->type = type;
}

AST::Expressions::Type *FunctionDeclaration::getType() const {
	return this->type;
}

void FunctionDeclaration::insertParameter(int index, Expressions::Parameter *param) {
	this->parameters.insert(this->parameters.begin() + index, param);
}

void FunctionDeclaration::addParameter(Expressions::Parameter *param) {
	this->parameters.push_back(param);
}

std::vector<AST::Expressions::Parameter *> FunctionDeclaration::getParameters() {
	return this->parameters;
}

void FunctionDeclaration::setBlock(Blocks::Generic *block) {
	this->block = block;
}

Blocks::Generic *FunctionDeclaration::getBlock() const {
	return this->block;
}
