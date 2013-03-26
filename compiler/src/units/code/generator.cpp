#include "units/code.h"

using namespace Code;

Generator::Generator(std::string moduleName) {
	this->moduleName = moduleName;
	this->module = new llvm::Module(moduleName, llvm::getGlobalContext());
}

Generator::~Generator() {
	delete this->module;
}

void Generator::parseAST(std::vector<AST::Statements::Statement *> statements) {
	DeclarationPass pass1(this->module);
	pass1.parseAST(statements);
	
	if (pass1.hasMessages()) {
		for (Message msg : pass1.getMessages()) {
			this->addMessage(msg);
		}
		
		return;
	}
	
	DefinitionPass pass2(this->module);
	pass2.parseAST(statements);
	
	if (pass2.hasMessages()) {
		for (Message msg : pass2.getMessages()) {
			this->addMessage(msg);
		}
		
		return;
	}
}

void Generator::dump() const {
	this->module->dump();
}

/*llvm::Value *Generator::parseDeclarationStatement(int i) {
	if (dynamic_cast<AST::Statements::FunctionDeclaration *>(this->statements[i]) != NULL) {
		return parseFunctionDeclarationStatement(i);
	}
	
	this->addError(i, "Expected declaration statement");
	return NULL;
}

llvm::Function *Generator::parseFunctionDeclarationStatement(int i) {
	AST::Statements::FunctionDeclaration *decl =
		dynamic_cast<AST::Statements::FunctionDeclaration *>(this->statements[i]);
	
	if (decl != NULL) {
		return parseFunctionDeclarationStatement(decl);
	}
	
	this->addError(i, "Expected function declaration statement");
	return NULL;
}

llvm::Function *Generator::parseFunctionDeclarationStatement(AST::Statements::FunctionDeclaration *decl) {
	
	
	llvm::BasicBlock *entry = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", func);
	
	if (decl->getName()->getValue() == "main") {
		this->builder->SetInsertPoint(entry);
	}
	
	return func;
}
*/
