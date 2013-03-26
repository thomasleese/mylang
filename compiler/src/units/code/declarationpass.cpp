#include "units/code.h"

using namespace Code;

DeclarationPass::DeclarationPass(llvm::Module *module) :
	Pass(module) {
	
}

void DeclarationPass::tokeniseAST() {
	for (int i = 0; i < this->statements.size(); i++) {
		if (dynamic_cast<AST::Statements::Import *>(this->statements[i]) != NULL) {
			parseImportStatement(i);
		} else if (dynamic_cast<AST::Statements::FunctionDeclaration *>(this->statements[i]) != NULL) {
			parseFunctionDeclarationStatement(i);
		} else {
			this->addError(i, "Expected import or declaration");
		}
	}
}

llvm::Value *DeclarationPass::parseImportStatement(int i) {
	AST::Statements::Import *import = dynamic_cast<AST::Statements::Import *>(this->statements[i]);
	if (import != NULL) {
		return parseImportStatement(import);
	}
	
	this->addError(import->getToken(), "Expected import");
	return NULL;
}

llvm::Value *DeclarationPass::parseImportStatement(AST::Statements::Import *import) {
	return NULL;
}

llvm::Function *DeclarationPass::parseFunctionDeclarationStatement(int i) {
	AST::Statements::FunctionDeclaration *func = dynamic_cast<AST::Statements::FunctionDeclaration *>(this->statements[i]);
	if (func != NULL) {
		return parseFunctionDeclarationStatement(func);
	}
	
	this->addError(func->getToken(), "Expected 'func'");
	return NULL;
}

llvm::Function *DeclarationPass::parseFunctionDeclarationStatement(AST::Statements::FunctionDeclaration *decl) {
	const int paramsLen = decl->getParameters().size();
	llvm::Type *params[paramsLen];
	for (int i = 0; i < paramsLen; i++) {
		llvm::Type *paramType = this->parseTypeExpression(decl->getParameters()[i]->getType());
		if (paramType == NULL) {
			return NULL;
		}
		
		params[i] = paramType;
	}
	
	llvm::Type *returnType = this->parseTypeExpression(decl->getType());
	if (returnType == NULL) {
		return NULL;
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
	
	return func;
}
