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
	
	for (AST::Statements::ConstantDeclaration *decl : this->block->getConstantDeclarationStatements()) {
		parseGlobalConstantDeclarationStatement(decl);
	}
	
	for (AST::Statements::FunctionDeclaration *decl : this->block->getFunctionDeclarationStatements()) {
		parseGlobalFunctionDeclarationStatement(decl);
	}
}

void DeclarationPass::parseGlobalImportStatement(AST::Statements::Import *import) {
	
}

void DeclarationPass::parseGlobalTypeDeclarationStatement(AST::Statements::TypeDeclaration *decl) {
	// Create function which defines the type
	llvm::NamedMDNode *metadata = this->module->getOrInsertNamedMetadata("type_" + decl->getName()->getValue());
	
	llvm::MDNode *typeMetadataNode;
	llvm::Type *baseType = this->parseTypeExpression(decl->getType(), &typeMetadataNode, decl);
	if (baseType == NULL) {
		return;
	}
	
	metadata->addOperand(typeMetadataNode);
	
	llvm::FunctionType *type = llvm::FunctionType::get(baseType, false);
	
	llvm::Function::LinkageTypes linkType = llvm::Function::PrivateLinkage;
	if (decl->getExported()) {
		linkType = llvm::Function::ExternalLinkage;
	}
	
	llvm::Function::Create(type, linkType, decl->getName()->getValue(), this->module);
	
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
	llvm::NamedMDNode *metadata = this->module->getOrInsertNamedMetadata("var_" + decl->getName()->getValue());
	
	llvm::MDNode *typeMetadataNode;
	llvm::Type *type = this->parseTypeExpression(decl->getType(), &typeMetadataNode);
	if (type == NULL) {
		return;
	}
	
	metadata->addOperand(typeMetadataNode);
	
	llvm::Function::LinkageTypes linkType = llvm::Function::PrivateLinkage;
	if (decl->getExported()) {
		linkType = llvm::Function::ExternalLinkage;
	}
	
	llvm::Constant *constant = llvm::Constant::getNullValue(type);
	
	std::string name = decl->getName()->getValue();
	
	new llvm::GlobalVariable(*this->module, type, false, linkType, constant, name);
}

void DeclarationPass::parseGlobalConstantDeclarationStatement(AST::Statements::ConstantDeclaration *decl) {
	llvm::NamedMDNode *metadata = this->module->getOrInsertNamedMetadata("const_" + decl->getName()->getValue());
	
	llvm::MDNode *typeMetadataNode;
	llvm::Type *type = this->parseTypeExpression(decl->getType(), &typeMetadataNode);
	if (type == NULL) {
		return;
	}
	
	metadata->addOperand(typeMetadataNode);
	
	llvm::Function::LinkageTypes linkType = llvm::Function::PrivateLinkage;
	if (decl->getExported()) {
		linkType = llvm::Function::ExternalLinkage;
	}
	
	llvm::Constant *constant = llvm::Constant::getNullValue(type);
	
	std::string name = decl->getName()->getValue();
	
	llvm::GlobalVariable *var = new llvm::GlobalVariable(*this->module, type,
											true, linkType, constant, name);
}

void DeclarationPass::parseGlobalFunctionDeclarationStatement(AST::Statements::FunctionDeclaration *decl) {
	llvm::NamedMDNode *metadata = this->module->getOrInsertNamedMetadata("func_" + decl->getName()->getValue());
	
	llvm::LLVMContext &ctx = llvm::getGlobalContext();
	
	const int paramsLen = decl->getParameters().size();
	
	std::vector<llvm::Value *> paramMetadatas;
	
	std::vector<llvm::Type *> paramTypes;
	for (int i = 0; i < paramsLen; i++) {
		AST::Expressions::Parameter *param = decl->getParameters()[i];
		llvm::MDNode *metadataNode;
		
		llvm::Type *paramType = this->parseTypeExpression(param->getType(), &metadataNode);
		if (paramType == NULL) {
			return;
		}
		
		paramMetadatas.push_back(metadataNode);
		paramTypes.push_back(paramType);
	}
	
	metadata->addOperand(llvm::MDNode::get(ctx, llvm::ArrayRef<llvm::Value *>(paramMetadatas)));
	
	llvm::MDNode *returnTypeMetadataNode;
	llvm::Type *returnType = this->parseTypeExpression(decl->getType(), &returnTypeMetadataNode);
	if (returnType == NULL) {
		return;
	}
	
	metadata->addOperand(returnTypeMetadataNode);
	
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
