#include <sstream>

#include "units/ast.h"

using namespace AST::Statements;

Statement::~Statement() {
	
}

void Block::addStatement(Statement *stat) {
	this->statements.push_back(stat);
}

void Expression::setExpression(Expressions::Expression *expr) {
	this->expression = expr;
}

void Import::setIdentifier(Expressions::Identifier *identifier) {
	this->identifier = identifier;
}

void Return::setExpression(Expressions::Expression *expr) {
	this->expression = expr;
}

If::If() {
	this->falseStatement = NULL;
}

void If::setExpression(Expressions::Expression *expr) {
	this->expression = expr;
}

void If::setTrueStatement(Statement *statement) {
	this->trueStatement = statement;
}

void If::setFalseStatement(Statement *statement) {
	this->falseStatement = statement;
}

Case::Case() {
	this->expression = NULL;
}

void Case::setExpression(Expressions::Expression *expr) {
	this->expression = expr;
}

void Case::setBlock(Block *block) {
	this->block = block;
}

void Switch::setExpression(Expressions::Expression *expr) {
	this->expression = expr;
}

void Switch::addCase(Case *statement) {
	this->cases.push_back(statement);
}

Declaration::Declaration() {
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

VariableDeclaration::VariableDeclaration() {
	this->assignment = NULL;
}

void VariableDeclaration::setType(Expressions::Type *type) {
	this->type = type;
}

void VariableDeclaration::setAssignment(Expressions::Expression *expr) {
	this->assignment = expr;
}

ConstantDeclaration::ConstantDeclaration() {
	this->assignment = NULL;
}

void ConstantDeclaration::setType(Expressions::Type *type) {
	this->type = type;
}

void ConstantDeclaration::setAssignment(Expressions::Expression *expr) {
	this->assignment = expr;
}

void TypeDeclaration::setType(Expressions::Type *type) {
	this->type = type;
}

void TypeDeclaration::setBlock(Block *block) {
	this->block = block;
}

void FunctionDeclaration::setType(Expressions::Type *type) {
	this->type = type;
}

AST::Expressions::Type *FunctionDeclaration::getType() const {
	return this->type;
}

void FunctionDeclaration::addParameter(Expressions::Parameter *param) {
	this->parameters.push_back(param);
}

std::vector<AST::Expressions::Parameter *> FunctionDeclaration::getParameters() {
	return this->parameters;
}

void FunctionDeclaration::setBlock(Block *block) {
	this->block = block;
}
