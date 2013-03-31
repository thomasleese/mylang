#include "units/code.h"

using namespace Code;

Pass::Pass(llvm::Module *module) {
	this->module = module;
	this->builder = new llvm::IRBuilder<>(llvm::getGlobalContext());
}

Pass::~Pass() {
	delete this->builder;
}

void Pass::addError(Lexical::Token *token, std::string msg) {
	std::string filename = token->getFilename();
	int line = token->getLineNumber();
	int col = token->getColumn();
	std::string tokenValue = token->getValue();
	
	this->addMessage(Message("Error", filename, line, col, tokenValue, msg));
}

void Pass::parseAST(AST::Blocks::Module *block) {
	this->readAST(block);
	this->generateCode();
}

void Pass::readAST(AST::Blocks::Module *block) {
	this->block = block;
}

llvm::Type *Pass::parseTypeExpression(AST::Expressions::Type *expr) {
	std::string name = expr->getName()->getName()->getValue();
	
	if (name == "Integer") {
		return llvm::Type::getInt64Ty(llvm::getGlobalContext());
	} else if (name == "Void") {
		return llvm::Type::getVoidTy(llvm::getGlobalContext());
	} else {
		this->addError(expr->getToken(), "Unknown type");
		return NULL;
	}
}
