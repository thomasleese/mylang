#include "units/code.h"

using namespace Code;

Generator::Generator(std::string moduleName) {
	this->moduleName = moduleName;
	this->module = new llvm::Module(moduleName, llvm::getGlobalContext());
}

Generator::~Generator() {
	delete this->module;
}

void Generator::addError(Lexical::Token *token, std::string msg) {
	std::string filename = token->getFilename();
	int line = token->getLineNumber();
	int col = token->getColumn();
	std::string tokenValue = token->getValue();
	
	this->addMessage(Message("Error", filename, line, col, tokenValue, msg));
}

void Generator::parseAST(AST::Blocks::Module *block) {
	DeclarationPass pass1(this);
	pass1.parseAST(block);
	
	if (hasMessages()) {
		return;
	}
	
	DefinitionPass pass2(this);
	pass2.parseAST(block);
	
	if (hasMessages()) {
		return;
	}
}

llvm::Module *Generator::getModule() const {
	return this->module;
}

void Generator::dump() const {
	this->module->dump();
}
