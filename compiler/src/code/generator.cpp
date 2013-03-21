#include <llvm/DerivedTypes.h>
#include <llvm/IRBuilder.h>
#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/Analysis/Verifier.h>

#include "units/lex.h"
#include "syntax/parser.h"
#include "code/generator.h"

using namespace Code;

Generator::Generator(std::string moduleName) {
	this->moduleName = moduleName;
	this->module = new llvm::Module(moduleName, llvm::getGlobalContext());
	this->builder = new llvm::IRBuilder<>(llvm::getGlobalContext());
}

Generator::~Generator() {
	delete this->module;
}

void Generator::parseAST(std::vector<Syntax::Statements::Statement *> statements) {
	this->statements = statements;
	
	for (int i = this->statements.size() - 1; i >= 0; i--) {
		Syntax::Statements::Import *stat =
			dynamic_cast<Syntax::Statements::Import *>(this->statements[i]);
		
		if (stat != NULL) {
			this->statements.erase(this->statements.begin() + i);
		}
	}
	
	for (int i = 0; i < this->statements.size(); i++) {
		genDeclarationStatement(i);
	}
}

void Generator::dump() const {
	this->module->dump();
}

void Generator::genImportStatement(int i) {
	Syntax::Statements::Import *stat =
		dynamic_cast<Syntax::Statements::Import *>(this->statements[i]);
	
	if (stat != NULL) {
		return;
	}
	
	throw "import";
}

void Generator::genDeclarationStatement(int i) {
	if (dynamic_cast<Syntax::Statements::FunctionDeclaration *>(this->statements[i]) != NULL) {
		return genFunctionDeclarationStatement(i);
	}
	
	throw "declaration";
}

void Generator::genFunctionDeclarationStatement(int i) {
	Syntax::Statements::FunctionDeclaration *decl =
		dynamic_cast<Syntax::Statements::FunctionDeclaration *>(this->statements[i]);
	
	if (decl != NULL) {
		const int paramsLen = decl->getParameters().size();
		llvm::Type *params[paramsLen];
		for (int i = 0; i < paramsLen; i++) {
			params[i] = this->genType(decl->getParameters()[i]->getType());
		}
		
		llvm::FunctionType *type = llvm::FunctionType::get(this->genType(decl->getType()),
										llvm::ArrayRef<llvm::Type *>(params, paramsLen), false);
		
		llvm::Function::LinkageTypes linkType = llvm::Function::ExternalLinkage;
		if (decl->getExported()) {
			linkType = llvm::Function::PrivateLinkage;
		}
		
		llvm::Function *func = llvm::Function::Create(type, linkType, decl->getName()->getValue(), this->module);
		llvm::BasicBlock *entry = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entrypoint", func);
		
		if (decl->getName()->getValue() == "main") {
			this->builder->SetInsertPoint(entry);
		}
		
		return;
	}
	
	throw "function declaration";
}

llvm::Type *Generator::genType(Syntax::Expressions::Type *type) {
	std::vector<Syntax::Expressions::Identifier *> names = type->getNames();
	std::string name = names[0]->getValue();
	
	if (name == "Integer") {
		return llvm::Type::getInt64Ty(llvm::getGlobalContext());
	} else if (name == "Void") {
		return llvm::Type::getVoidTy(llvm::getGlobalContext());
	} else {
		return NULL;
	}
}
