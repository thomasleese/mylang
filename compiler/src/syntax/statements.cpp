#include <sstream>

#include "syntax/expressions.h"
#include "syntax/statements.h"
#include "formatting.h"

using namespace Syntax::Statements;

void Block::addStatement(Statement *stat) {
	this->statements.push_back(stat);
}

std::string Block::toString() const {
	std::stringstream ss;
	
	ss << FORMAT_BOLD FORMAT_BLUE "Block" FORMAT_NONE "(";
	ss << FORMAT_YELLOW "statements" FORMAT_NONE "=[";
	
	for (int i = 0; i < this->statements.size(); i++) {
		ss << this->statements[i]->toString();
		
		if (i < this->statements.size() - 1) {
			ss << ", ";
		}
	}
	
	ss << "])";
	
	return ss.str();
}

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

void Import::setIdentifier(Expressions::Identifier *identifier) {
	this->identifier = identifier;
}

std::string Import::toString() const {
	std::stringstream ss;
	
	ss << FORMAT_BOLD FORMAT_BLUE "Import" FORMAT_NONE "(";
	ss << FORMAT_YELLOW "identifier" FORMAT_NONE "=" << this->identifier->toString();
	ss << ")";
	
	return ss.str();
}

void Return::setExpression(Expressions::Expression *expr) {
	this->expression = expr;
}

std::string Return::toString() const {
	std::stringstream ss;
	
	ss << FORMAT_BOLD FORMAT_BLUE "Return" FORMAT_NONE "(";
	ss << FORMAT_YELLOW "expression" FORMAT_NONE "=" << this->expression->toString();
	ss << ")";
	
	return ss.str();
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

std::string If::toString() const {
	std::stringstream ss;
	
	ss << FORMAT_BOLD FORMAT_BLUE "If" FORMAT_NONE "(";
	ss << FORMAT_YELLOW "expression" FORMAT_NONE "=" << this->expression->toString() << " ";
	ss << FORMAT_YELLOW "true" FORMAT_NONE "=" << this->trueStatement->toString();
	
	if (falseStatement != NULL) {
		ss << " ";
		ss << FORMAT_YELLOW "false" FORMAT_NONE "=" << this->falseStatement->toString();
	}
	
	ss << ")";
	
	return ss.str();
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

std::string Case::toString() const {
	std::stringstream ss;
	
	ss << FORMAT_BOLD FORMAT_BLUE "Case" FORMAT_NONE "(";
	
	if (this->expression != NULL) {
		ss << FORMAT_YELLOW "expression" FORMAT_NONE "=" << this->expression->toString() << " ";
	}
	
	ss << FORMAT_YELLOW "block" FORMAT_NONE "=" << this->block->toString();
	ss << ")";
	
	return ss.str();
}

void Switch::setExpression(Expressions::Expression *expr) {
	this->expression = expr;
}

void Switch::addCase(Case *statement) {
	this->cases.push_back(statement);
}

std::string Switch::toString() const {
	std::stringstream ss;
	
	ss << FORMAT_BOLD FORMAT_BLUE "Switch" FORMAT_NONE "(";
	ss << FORMAT_YELLOW "expression" FORMAT_NONE "=" << this->expression->toString() << " ";
	
	ss << FORMAT_YELLOW "cases" FORMAT_NONE "=[";
	for (int i = 0; i < this->cases.size(); i++) {
		ss << this->cases[i]->toString();
		
		if (i < this->cases.size() - 1) {
			ss << ", ";
		}
	}
	ss << "])";
	
	return ss.str();
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

Syntax::Expressions::Identifier *Declaration::getName() {
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

std::string VariableDeclaration::toString() const {
	std::stringstream ss;
	
	ss << FORMAT_BOLD FORMAT_BLUE "Variable" FORMAT_NONE "(";
	ss << FORMAT_YELLOW "type" FORMAT_NONE "=" << this->type->toString() << " ";
	ss << FORMAT_YELLOW "name" FORMAT_NONE "=" << this->name->toString() << " ";
	
	if (this->assignment != NULL) {
		ss << FORMAT_YELLOW "assignment" FORMAT_NONE "=" << this->assignment->toString();
	}
	
	ss << ")";
	
	return ss.str();
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

std::string ConstantDeclaration::toString() const {
	std::stringstream ss;
	
	ss << FORMAT_BOLD FORMAT_BLUE "Constant" FORMAT_NONE "(";
	ss << FORMAT_YELLOW "type" FORMAT_NONE "=" << this->type->toString() << " ";
	ss << FORMAT_YELLOW "name" FORMAT_NONE "=" << this->name->toString() << " ";
	
	if (this->assignment != NULL) {
		ss << FORMAT_YELLOW "assignment" FORMAT_NONE "=" << this->assignment->toString();
	}
	
	ss << ")";
	
	return ss.str();
}

void TypeDeclaration::setType(Expressions::Type *type) {
	this->type = type;
}

void TypeDeclaration::setBlock(Block *block) {
	this->block = block;
}

std::string TypeDeclaration::toString() const {
	std::stringstream ss;
	
	ss << FORMAT_BOLD FORMAT_BLUE "TypeDeclaration" FORMAT_NONE "(";
	ss << FORMAT_YELLOW "type" FORMAT_NONE "=" << this->type->toString() << " ";
	ss << FORMAT_YELLOW "name" FORMAT_NONE "=" << this->name->toString() << " ";
	ss << FORMAT_YELLOW "block" FORMAT_NONE "=" << this->block->toString();
	ss << ")";
	
	return ss.str();
}

void FunctionDeclaration::setType(Expressions::Type *type) {
	this->type = type;
}

Syntax::Expressions::Type *FunctionDeclaration::getType() const {
	return this->type;
}

void FunctionDeclaration::addParameter(Expressions::Parameter *param) {
	this->parameters.push_back(param);
}

std::vector<Syntax::Expressions::Parameter *> &FunctionDeclaration::getParameters() {
	return this->parameters;
}

void FunctionDeclaration::setBlock(Block *block) {
	this->block = block;
}

std::string FunctionDeclaration::toString() const {
	std::stringstream ss;
	
	ss << FORMAT_BOLD FORMAT_BLUE "FunctionDeclaration" FORMAT_NONE "(";
	ss << FORMAT_YELLOW "type" FORMAT_NONE "=" << this->type->toString() << " ";
	ss << FORMAT_YELLOW "name" FORMAT_NONE "=" << this->name->toString() << " ";
	
	ss << FORMAT_YELLOW "parameters" FORMAT_NONE "=[";
	for (int i = 0; i < this->parameters.size(); i++) {
		ss << this->parameters[i]->toString();
		
		if (i < this->parameters.size() - 1) {
			ss << ", ";
		}
	}
	ss << "] ";
	
	ss << FORMAT_YELLOW "block" FORMAT_NONE "=" << this->block->toString();
	ss << ")";
	
	return ss.str();
}
