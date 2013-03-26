#include "units/code.h"

using namespace Code;

Pass::Pass(llvm::Module *module) {
	this->module = module;
	this->builder = new llvm::IRBuilder<>(llvm::getGlobalContext());
}

Pass::~Pass() {
	delete this->builder;
}

void Pass::addError(int i, std::string msg) {
	this->addError(this->statements[i]->getToken(), msg);
}

void Pass::addError(Lexical::Token *token, std::string msg) {
	std::string filename = token->getFilename();
	int line = token->getLineNumber();
	int col = token->getColumn();
	std::string tokenValue = token->getValue();
	
	this->addMessage(Message("Error", filename, line, col, tokenValue, msg));
}

void Pass::parseAST(std::vector<AST::Statements::Statement *> statements) {
	this->readAST(statements);
	this->tokeniseAST();
}

void Pass::readAST(std::vector<AST::Statements::Statement *> statements) {
	this->statements = statements;
}

llvm::Type *Pass::parseTypeExpression(AST::Expressions::Type *expr) {
	std::vector<AST::Expressions::Identifier *> names = expr->getNames();
	std::string name = names[0]->getValue();
	
	if (name == "Integer") {
		return llvm::Type::getInt64Ty(llvm::getGlobalContext());
	} else if (name == "Void") {
		return llvm::Type::getVoidTy(llvm::getGlobalContext());
	} else {
		this->addError(expr->getToken(), "Unknown type");
		return NULL;
	}
}
