#pragma once

#include <iostream>
#include <vector>

#include "llvm/IRBuilder.h"

#include "units/ast.h"

namespace llvm {
	
	class Module;
	
}

namespace Code {
	
	class Generator : public Unit {
		
	public:
		Generator(std::string moduleName);
		~Generator();
		
		void addError(int i, std::string msg);
		void addError(Lexical::Token *token, std::string msg);
		
		void parseAST(std::vector<AST::Statements::Statement *> statements);
		void dump() const;
		
	private:
		// Statements
		llvm::Value *parseDeclarationStatement(int i);
		llvm::Function *parseFunctionDeclarationStatement(int i);
		llvm::Function *parseFunctionDeclarationStatement(AST::Statements::FunctionDeclaration *decl);
		
		// Expressions
		llvm::Type *parseTypeExpression(int i);
		llvm::Type *parseTypeExpression(AST::Expressions::Type *expr);
		
	private:
		std::string moduleName;
		
		llvm::Module *module;
		llvm::IRBuilder<> *builder;
		
		std::vector<AST::Statements::Statement *> statements;
	};

}
