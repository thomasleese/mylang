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
	llvm::LLVMContext &ctx = llvm::getGlobalContext();
	
	std::string moduleName = "";
	if (ident->getModule() != NULL) {
		moduleName = ident->getModule()->getValue();
	}
	
	std::string name = ident->getName()->getValue();
	
	if (name == "Byte") {
		return llvm::Type::getInt8Ty(ctx);
	} else if (name == "Integer") {
		return llvm::Type::getInt64Ty(ctx);
	} else if (name == "Float") {
		return llvm::Type::getDoubleTy(ctx);
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

llvm::Type *Pass::parseTypeExpression(AST::Expressions::Type *expr, AST::Statements::TypeDeclaration *typeDecl) {
	llvm::Type *type;
	
	if (typeDecl != NULL && expr->getIsStruct()) {
		std::vector<llvm::Type *> elements;
		
		AST::Blocks::Type *block = typeDecl->getBlock();
		for (AST::Statements::VariableDeclaration *var : block->getVariableDeclarationStatements()) {
			elements.push_back(parseTypeExpression(var->getType()));
		}
		
		type = llvm::StructType::create(llvm::getGlobalContext(), llvm::ArrayRef<llvm::Type *>(elements), typeDecl->getName()->getValue());
	} else {
		type = parseTypeExpression(expr->getName());
	}
	
	return type;
}

AST::Statements::FunctionDeclaration *Pass::convertTypeFunctionToFunction(AST::Statements::FunctionDeclaration *func, AST::Statements::TypeDeclaration *type) {
	func->getName()->setValue(type->getName()->getValue() + "_method_" + func->getName()->getValue());
	
	AST::Expressions::Parameter *param = new AST::Expressions::Parameter(NULL);
	
	AST::Expressions::Type *paramType = new AST::Expressions::Type(NULL);
	paramType->setName(type->getName()->toQualifiedIdentifier());
	
	param->setType(paramType);
	
	AST::Expressions::Identifier *thisIdentifier = new AST::Expressions::Identifier(NULL);
	thisIdentifier->setValue("this");
	param->setName(thisIdentifier);
	
	func->insertParameter(0, param);
	
	if (func->getExported() && !type->getExported()) {
		this->generator->addError(func->getToken(), "Cannot declare exported function of non-exported type");
		return NULL;
	}
	
	return func;
}
