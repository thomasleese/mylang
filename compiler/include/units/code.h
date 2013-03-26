#pragma once

#include <iostream>
#include <vector>

#include <llvm/IRBuilder.h>
#include <llvm/DerivedTypes.h>
#include <llvm/IRBuilder.h>
#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/Analysis/Verifier.h>

#include "units/ast.h"

namespace llvm {
	
	class Module;
	
}

namespace Code {
	
	class Pass : public Unit {
		
	public:
		Pass(llvm::Module *module);
		~Pass();
		
		void addError(int i, std::string msg);
		void addError(Lexical::Token *token, std::string msg);
		
		void parseAST(std::vector<AST::Statements::Statement *> statements);
		
	private:
		void readAST(std::vector<AST::Statements::Statement *> statements);
		virtual void tokeniseAST() = 0;
		
	protected:
		llvm::Type *parseTypeExpression(AST::Expressions::Type *expr);
		
	protected:
		llvm::Module *module;
		llvm::IRBuilder<> *builder;
		std::vector<AST::Statements::Statement *> statements;
		
	};
	
	class DeclarationPass : public Pass {
		
	public:
		DeclarationPass(llvm::Module *module);
		
	private:
		void tokeniseAST();
		
		llvm::Value *parseImportStatement(int i);
		llvm::Value *parseImportStatement(AST::Statements::Import *import);
		llvm::Function *parseFunctionDeclarationStatement(int i);
		llvm::Function *parseFunctionDeclarationStatement(AST::Statements::FunctionDeclaration *func);
		
	};
	
	class DefinitionPass : public Pass {
		
	public:
		DefinitionPass(llvm::Module *module);
		
	private:
		void tokeniseAST();
		
	};
	
	class Generator : public Unit {
		
	public:
		Generator(std::string moduleName);
		~Generator();
		
		void parseAST(std::vector<AST::Statements::Statement *> statements);
		void dump() const;
				
	private:
		std::string moduleName;
		llvm::Module *module;
		std::vector<AST::Statements::Statement *> statements;
	};

}
