#include "units/code.h"

using namespace Code;

DeclarationPass::DeclarationPass(Generator *gen) :
	Pass(gen) {
	
}

void DeclarationPass::generateCode() {
	for (AST::Statements::Import *import : this->block->getImportStatements()) {
		parseGlobalImportStatement(import);
	}
	
	for (AST::Statements::TypeDeclaration *decl : this->block->getTypeDeclarationStatements()) {
		parseGlobalTypeDeclarationStatement(decl);
	}
	
	for (AST::Statements::VariableDeclaration *decl : this->block->getVariableDeclarationStatements()) {
		parseGlobalVariableDeclarationStatement(decl);
	}
	
	for (AST::Statements::FunctionDeclaration *decl : this->block->getFunctionDeclarationStatements()) {
		parseGlobalFunctionDeclarationStatement(decl);
	}
}

void DeclarationPass::parseGlobalImportStatement(AST::Statements::Import *import) {
	
}

void DeclarationPass::parseGlobalTypeDeclarationStatement(AST::Statements::TypeDeclaration *decl) {
	std::string name = decl->getName()->getValue();
	
#ifdef DEBUG
	std::cout << "Parsing global type declaration statement: " << name << std::endl;
#endif
	
	// Create function which defines the type
	llvm::Type *baseType = this->parseTypeExpression(decl->getType(), decl);
	if (baseType == NULL) {
		return;
	}
	
	llvm::FunctionType *type = llvm::FunctionType::get(baseType, false);
	
	llvm::Function::LinkageTypes linkType = llvm::Function::PrivateLinkage;
	if (decl->getExported()) {
		linkType = llvm::Function::ExternalLinkage;
	}
	
	llvm::Function::Create(type, linkType, name, this->module);
	
	// create the methods of the type
	AST::Blocks::Type *block = decl->getBlock();
	for (AST::Statements::FunctionDeclaration *func : block->getFunctionDeclarationStatements()) {
		// we need to convert the "normal" function into a method
		func = this->convertTypeFunctionToFunction(func, decl);
		
		if (func != NULL) {
			parseGlobalFunctionDeclarationStatement(func);
		}
	}
}

void DeclarationPass::parseGlobalVariableDeclarationStatement(AST::Statements::VariableDeclaration *decl) {
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
	
	new llvm::GlobalVariable(*this->module, type, decl->getIsConstant(), linkType, constant, name);
}

void DeclarationPass::parseGlobalFunctionDeclarationStatement(AST::Statements::FunctionDeclaration *decl) {
	llvm::LLVMContext &ctx = llvm::getGlobalContext();
	
	std::string funcName = decl->getName()->getValue();
	
	const int paramsLen = decl->getParameters().size();

#ifdef DEBUG
	std::cout << "Parsing global function declaration statement: " << funcName << " (" << paramsLen << " params)" << std::endl;
#endif
	
	std::vector<llvm::Type *> paramTypes;
	for (int i = 0; i < paramsLen; i++) {
		AST::Expressions::Parameter *param = decl->getParameters()[i];
		
		llvm::Type *paramType = this->parseTypeExpression(param->getType());
		if (paramType == NULL) {
			return;
		}
		
		paramTypes.push_back(paramType);
	}
	
	llvm::Type *returnType = this->parseTypeExpression(decl->getType());
	if (returnType == NULL) {
		return;
	}
	
	llvm::FunctionType *type = llvm::FunctionType::get(returnType, paramTypes, false);
	
	llvm::Function::LinkageTypes linkType = llvm::Function::PrivateLinkage;
	if (decl->getExported()) {
		linkType = llvm::Function::ExternalLinkage;
	}
	
	llvm::Function *func = llvm::Function::Create(type, linkType, funcName, this->module);
	func->setCallingConv(llvm::CallingConv::C);
	
	llvm::Function::arg_iterator args = func->arg_begin();
	for (int i = 0; i < paramsLen; i++) {
		(args++)->setName(decl->getParameters()[i]->getName()->getValue());
	}
}
