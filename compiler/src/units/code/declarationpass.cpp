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
	this->parseGlobalImportStatement(import->getName()->getValue());
}

void DeclarationPass::parseGlobalImportStatement(std::string name) {
	llvm::LLVMContext &ctx = llvm::getGlobalContext();
	
#ifdef DEBUG
	std::cout << "Parsing global import statement: " << name << std::endl;
#endif
	
	llvm::Module *module = this->findModuleByName(name);
	
	// go through every function inside the module and determine what should be declared into this module
	for (llvm::Module::iterator i = module->begin(), e = module->end(); i != e; ++i) {
		std::string n = i->getName().str();
		if (n.substr(0, 7) == "module_") {
			this->parseGlobalImportStatement(n.substr(7));
		} else if (n.substr(0, name.length() + 1) == name + "_") {
			if (i->hasDefaultVisibility()) {
				llvm::Function::Create(i->getFunctionType(), llvm::Function::ExternalLinkage, n, this->module);
			}
		}
	}
	
	this->module->getOrInsertFunction("module_" + name, llvm::Type::getVoidTy(ctx), NULL);
}

void DeclarationPass::parseGlobalTypeDeclarationStatement(AST::Statements::TypeDeclaration *decl) {
	std::string name = this->moduleName + "_" + decl->getName()->getValue();
	
#ifdef DEBUG
	std::cout << "Parsing global type declaration statement: " << name << std::endl;
#endif
	
	// Create function which defines the type
	llvm::Type *baseType = this->parseTypeExpression(decl->getType(), decl);
	if (baseType == NULL) {
		return;
	}
	
	llvm::FunctionType *type = llvm::FunctionType::get(baseType, false);
	llvm::Function *func = llvm::Function::Create(type, llvm::Function::ExternalLinkage, name, this->module);
	
	if (decl->getExported()) {
		func->setVisibility(llvm::GlobalValue::DefaultVisibility);
	} else {
		func->setVisibility(llvm::GlobalValue::ProtectedVisibility);
	}
	
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
	std::string name = this->moduleName + "_" + decl->getName()->getValue();
	
#ifdef DEBUG
	std::cout << "Parsing global variable declaration statement: " << name << std::endl;
#endif
	
	llvm::Type *type = this->parseTypeExpression(decl->getType());
	if (type == NULL) {
		return;
	}
	
	llvm::Constant *constant = llvm::Constant::getNullValue(type);
	llvm::GlobalValue *val = new llvm::GlobalVariable(*this->module, type, decl->getIsConstant(), llvm::Function::ExternalLinkage, constant, name);
	
	if (decl->getExported()) {
		val->setVisibility(llvm::GlobalValue::DefaultVisibility);
	} else {
		val->setVisibility(llvm::GlobalValue::ProtectedVisibility);
	}
}

void DeclarationPass::parseGlobalFunctionDeclarationStatement(AST::Statements::FunctionDeclaration *decl) {
	llvm::LLVMContext &ctx = llvm::getGlobalContext();
	
	std::string funcName = this->moduleName + "_" + decl->getName()->getValue();
	
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
	
	llvm::Function *func = llvm::Function::Create(type, llvm::Function::ExternalLinkage, funcName, this->module);
	func->setCallingConv(llvm::CallingConv::C);
	
	if (decl->getExported()) {
		func->setVisibility(llvm::GlobalValue::DefaultVisibility);
	} else {
		func->setVisibility(llvm::GlobalValue::ProtectedVisibility);
	}
	
	llvm::Function::arg_iterator args = func->arg_begin();
	for (int i = 0; i < paramsLen; i++) {
		(args++)->setName(decl->getParameters()[i]->getName()->getValue());
	}
}
