#include "units/code.h"

using namespace Code;

DeclarationPass::DeclarationPass(Generator *gen) :
	Pass(gen) {
	
}

void DeclarationPass::generateCode() {
	for (AST::Statements::Import *import : this->block->getImportStatements()) {
		parseImportStatement(import);
	}
	
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

void DeclarationPass::parseImportStatement(AST::Statements::Import *import) {
	
}

void DeclarationPass::parseTypeDeclarationStatement(AST::Statements::TypeDeclaration *decl) {
	// Create function which defines the type
	llvm::Type *baseType = this->parseTypeExpression(decl->getType());
	if (baseType == NULL) {
		return;
	}
	
	std::string name = decl->getName()->getValue();
	
	llvm::FunctionType *type = llvm::FunctionType::get(baseType, false);
	
	llvm::Function::LinkageTypes linkType = llvm::Function::PrivateLinkage;
	if (decl->getExported()) {
		linkType = llvm::Function::ExternalLinkage;
	}
	
	llvm::Function::Create(type, linkType, name, this->module);
	
	AST::Blocks::Type *block = decl->getBlock();
	for (AST::Statements::FunctionDeclaration *func : block->getFunctionDeclarationStatements()) {
		func->getName()->setValue(name + "_method_" + func->getName()->getValue());
		
		AST::Expressions::Parameter *param = new AST::Expressions::Parameter(NULL);
		
		param->setType(decl->getType());
		
		AST::Expressions::Identifier *thisIdentifier = new AST::Expressions::Identifier(NULL);
		thisIdentifier->setValue("this");
		param->setName(thisIdentifier);
		
		func->insertParameter(0, param);
		
		parseFunctionDeclarationStatement(func);
	}
}

void DeclarationPass::parseConstantDeclarationStatement(AST::Statements::ConstantDeclaration *decl) {
	llvm::Type *type = this->parseTypeExpression(decl->getType());
	if (type == NULL) {
		return;
	}
	
	llvm::Function::LinkageTypes linkType = llvm::Function::PrivateLinkage;
	if (decl->getExported()) {
		linkType = llvm::Function::ExternalLinkage;
	}
	
	llvm::Constant *constant = llvm::Constant::getNullValue(type);
	
	std::string name = decl->getName()->getValue();
	
	llvm::GlobalVariable *var = new llvm::GlobalVariable(*this->module, type,
											true, linkType, constant, name);
}

void DeclarationPass::parseFunctionDeclarationStatement(AST::Statements::FunctionDeclaration *decl) {
	const int paramsLen = decl->getParameters().size();
	
	std::vector<llvm::Type *> paramTypes;
	for (int i = 0; i < paramsLen; i++) {
		llvm::Type *paramType = this->parseTypeExpression(decl->getParameters()[i]->getType());
		if (paramType == NULL) {
			return;
		}
		
		paramTypes.push_back(paramType);
	}
	
	llvm::Type *returnType = this->parseTypeExpression(decl->getType());
	if (returnType == NULL) {
		return;
	}
	
	llvm::FunctionType *type = llvm::FunctionType::get(returnType,
									llvm::ArrayRef<llvm::Type *>(paramTypes), false);
	
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
