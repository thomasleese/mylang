#include "units/ast.h"

using namespace AST;
using namespace AST::Blocks;

Block::Block(Lexical::Token *token) {
	this->token = token;
}

Lexical::Token *Block::getToken() const {
	return this->token;
}

Generic::Generic(Lexical::Token *token) :
	Block(token) {
	
}

void Generic::addGenericStatement(Statements::Generic *stat) {
	this->generics.push_back(stat);
}

std::vector<Statements::Generic *> Generic::getGenericStatements() {
	return this->generics;
}

Type::Type(Lexical::Token *token) :
	Block(token) {
	
}

void Type::addVariableDeclarationStatement(Statements::VariableDeclaration *decl) {
	this->variables.push_back(decl);
}

std::vector<Statements::VariableDeclaration *> Type::getVariableDeclarationStatements() {
	return this->variables;
}

void Type::addFunctionDeclarationStatement(Statements::FunctionDeclaration *decl) {
	this->functions.push_back(decl);
}

std::vector<Statements::FunctionDeclaration *> Type::getFunctionDeclarationStatements() {
	return this->functions;
}

Module::Module(Lexical::Token *token) :
	Type(token) {
	
}

void Module::addImportStatement(Statements::Import *import) {
	this->imports.push_back(import);
}

std::vector<Statements::Import *> Module::getImportStatements() {
	return this->imports;
}

void Module::addConstantDeclarationStatement(Statements::ConstantDeclaration *decl) {
	this->constants.push_back(decl);
}

std::vector<Statements::ConstantDeclaration *> Module::getConstantDeclarationStatements() {
	return this->constants;
}

void Module::addTypeDeclarationStatement(Statements::TypeDeclaration *decl) {
	this->types.push_back(decl);
}

std::vector<Statements::TypeDeclaration *> Module::getTypeDeclarationStatements() {
	return this->types;
}
