#include "units/code.h"

using namespace Code;

DeclarationPass::DeclarationPass(llvm::Module *module) :
	Pass(module) {
	
}

void DeclarationPass::generateCode() {
	for (AST::Statements::Import *import : this->block->getImportStatements()) {
		parseImportStatement(import);
	}
	
	for (AST::Statements::FunctionDeclaration *decl : this->block->getFunctionDeclarationStatements()) {
		parseFunctionDeclarationStatement(decl);
	}
}

void DeclarationPass::parseImportStatement(AST::Statements::Import *import) {
	
}

void DeclarationPass::parseFunctionDeclarationStatement(AST::Statements::FunctionDeclaration *decl) {
	const int paramsLen = decl->getParameters().size();
	llvm::Type *params[paramsLen];
	for (int i = 0; i < paramsLen; i++) {
		llvm::Type *paramType = this->parseTypeExpression(decl->getParameters()[i]->getType());
		if (paramType == NULL) {
			return;
		}
		
		params[i] = paramType;
	}
	
	llvm::Type *returnType = this->parseTypeExpression(decl->getType());
	if (returnType == NULL) {
		return;
	}
	
	llvm::FunctionType *type = llvm::FunctionType::get(returnType,
									llvm::ArrayRef<llvm::Type *>(params, paramsLen), false);
	
	llvm::Function::LinkageTypes linkType = llvm::Function::PrivateLinkage;
	if (decl->getExported()) {
		linkType = llvm::Function::ExternalLinkage;
	}
	
	llvm::Function *func = llvm::Function::Create(type, linkType, decl->getName()->getValue(), this->module);
	
	llvm::Function::arg_iterator args = func->arg_begin();
	for (int i = 0; i < paramsLen; i++) {
		(args++)->setName(decl->getParameters()[i]->getName()->getValue());
	}
}
