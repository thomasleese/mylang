#include "units/code.h"

using namespace Code;

Pass::Pass(Generator *gen) {
	this->generator = gen;
	this->module = gen->getModule();
	this->irBuilder = new llvm::IRBuilder<>(llvm::getGlobalContext());
	this->mdBuilder = new llvm::MDBuilder(llvm::getGlobalContext());
}

Pass::~Pass() {
	delete this->irBuilder;
	delete this->mdBuilder;
}

void Pass::parseAST(AST::Blocks::Module *block) {
	this->readAST(block);
	this->generateCode();
}

void Pass::readAST(AST::Blocks::Module *block) {
	this->block = block;
}

llvm::Type *Pass::parseTypeExpression(AST::Expressions::QualifiedIdentifier *ident) {
	std::string moduleName = "";
	if (ident->getModule() != NULL) {
		moduleName = ident->getModule()->getValue();
	}
	
	std::string name = ident->getName()->getValue();
	
	llvm::LLVMContext &ctx = llvm::getGlobalContext();
	
	if (name == "Integer") {
		return llvm::Type::getInt64Ty(ctx);
	} else if (name == "Void") {
		return llvm::Type::getVoidTy(ctx);
	} else {
		llvm::Function *func = this->module->getFunction(name);
		if (func != NULL) {
			return func->getReturnType();
		}
		
		this->generator->addError(ident->getToken(), "Unknown type");
		return NULL;
	}
}

llvm::Type *Pass::parseTypeExpression(AST::Expressions::Type *expr) {
	llvm::Type *type = parseTypeExpression(expr->getName());
	
	
	
	return type;
}
