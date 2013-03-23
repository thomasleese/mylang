#pragma once

#include <iostream>
#include <vector>

#include "llvm/IRBuilder.h"

namespace llvm {
	
	class Module;
	
}

namespace AST {
	
	namespace Statements {
		
		class Statement;
		
	}
	
}

namespace Code {
	
	class Generator {
		
	public:
		Generator(std::string moduleName);
		~Generator();
		
		void parseAST(std::vector<AST::Statements::Statement *> statements);
		void dump() const;
		
	private:
		void genImportStatement(int i);
		void genDeclarationStatement(int i);
		void genFunctionDeclarationStatement(int i);
		
		llvm::Type *genType(AST::Expressions::Type *type);
		
	private:
		std::string moduleName;
		
		llvm::Module *module;
		llvm::IRBuilder<> *builder;
		
		std::vector<AST::Statements::Statement *> statements;
	};

}
