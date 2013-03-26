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
		
		void addError(Lexical::Token *token, std::string msg);
		
		void parseAST(AST::Blocks::Module *block);
		
	private:
		void readAST(AST::Blocks::Module *block);
		virtual void generateCode() = 0;
		
	protected:
		llvm::Type *parseTypeExpression(AST::Expressions::Type *expr);
		
	protected:
		llvm::Module *module;
		llvm::IRBuilder<> *builder;
		AST::Blocks::Module *block;
		
	};
	
	class DeclarationPass : public Pass {
		
	public:
		DeclarationPass(llvm::Module *module);
		
	private:
		void generateCode();
		
		void parseImportStatement(AST::Statements::Import *import);
		void parseFunctionDeclarationStatement(AST::Statements::FunctionDeclaration *func);
		
	};
	
	class DefinitionPass : public Pass {
		
	public:
		DefinitionPass(llvm::Module *module);
		
	private:
		void generateCode();
		
	};
	
	class Generator : public Unit {
		
	public:
		Generator(std::string moduleName);
		~Generator();
		
		void parseAST(AST::Blocks::Module *block);
		void dump() const;
				
	private:
		std::string moduleName;
		llvm::Module *module;
		AST::Blocks::Module *block;
	};

}
