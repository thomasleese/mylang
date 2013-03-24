#include <llvm/DerivedTypes.h>
#include <llvm/IRBuilder.h>
#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/Analysis/Verifier.h>

#include "units/code.h"

using namespace Code;

Generator::Generator(std::string moduleName) {
	this->moduleName = moduleName;
	this->module = new llvm::Module(moduleName, llvm::getGlobalContext());
	this->builder = new llvm::IRBuilder<>(llvm::getGlobalContext());
}

Generator::~Generator() {
	delete this->module;
}

void Generator::addError(int i, std::string msg) {
	this->addError(this->statements[i]->getToken(), msg);
}

void Generator::addError(Lexical::Token *token, std::string msg) {
	std::string filename = token->getFilename();
	int line = token->getLineNumber();
	int col = token->getColumn();
	std::string tokenValue = token->getValue();
	
	this->addMessage(Message("Error", filename, line, col, tokenValue, msg));
}

void Generator::parseAST(std::vector<AST::Statements::Statement *> statements) {
	this->statements = statements;
	
	for (int i = this->statements.size() - 1; i >= 0; i--) {
		AST::Statements::Import *stat =
			dynamic_cast<AST::Statements::Import *>(this->statements[i]);
		
		if (stat != NULL) {
			this->statements.erase(this->statements.begin() + i);
		}
	}
	
	for (int i = 0; i < this->statements.size(); i++) {
		parseDeclarationStatement(i);
	}
}

void Generator::dump() const {
	this->module->dump();
}

llvm::Value *Generator::parseDeclarationStatement(int i) {
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
	
	llvm::BasicBlock *entry = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", func);
	
	if (decl->getName()->getValue() == "main") {
		this->builder->SetInsertPoint(entry);
	}
	
	return func;
}

llvm::Type *Generator::parseTypeExpression(int i) {
	AST::Expressions::Type *expr = dynamic_cast<AST::Expressions::Type *>(this->statements[i]);
	if (expr != NULL) {
		return parseTypeExpression(expr);
	} else {
		this->addError(i, "Expected type expression");
	}
	
	return NULL;
}

llvm::Type *Generator::parseTypeExpression(AST::Expressions::Type *expr) {
	std::vector<AST::Expressions::Identifier *> names = expr->getNames();
	std::string name = names[0]->getValue();
	
	if (name == "Integer") {
		return llvm::Type::getInt64Ty(llvm::getGlobalContext());
	} else if (name == "Void") {
		return llvm::Type::getVoidTy(llvm::getGlobalContext());
	} else {
		this->addError(expr->getToken(), "Unknown type");
		return NULL;
	}
}
