#include "units/code.h"

using namespace Code;

DefinitionPass::DefinitionPass(Generator *gen) :
	Pass(gen) {
	
}

void DefinitionPass::generateCode() {
	for (AST::Statements::TypeDeclaration *decl : this->block->getTypeDeclarationStatements()) {
		parseTypeDeclarationStatement(decl);
	}
	
	for (AST::Statements::ConstantDeclaration *decl : this->block->getConstantDeclarationStatements()) {
		parseConstantDeclarationStatement(decl);
	}
	
	for (AST::Statements::FunctionDeclaration *decl : this->block->getFunctionDeclarationStatements()) {
		parseFunctionDeclarationStatement(decl);
	}
}

void DefinitionPass::parseTypeDeclarationStatement(AST::Statements::TypeDeclaration *decl) {
	AST::Blocks::Type *block = decl->getBlock();
	for (AST::Statements::FunctionDeclaration *func : block->getFunctionDeclarationStatements()) {
		// we don't need to convert it, as this has already happend in the definition stage
		
		if (func != NULL) {
			parseFunctionDeclarationStatement(func);
		}
	}
}

void DefinitionPass::parseConstantDeclarationStatement(AST::Statements::ConstantDeclaration *decl) {
	
}

void DefinitionPass::parseFunctionDeclarationStatement(AST::Statements::FunctionDeclaration *decl) {
	llvm::Function *func = this->module->getFunction(decl->getName()->getValue());
	
	if (func == NULL) { // an error must've occured earlier on
		return;
	}
	
	llvm::BasicBlock *entry = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", func);
	this->irBuilder->SetInsertPoint(entry);
}
