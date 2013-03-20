#pragma once

#include <iostream>
#include <vector>

#include "llvm/IRBuilder.h"

namespace llvm {
	
	class Module;
	
}

namespace Syntax {
	
	namespace Statements {
		
		class Statement;
		
	}
	
}

namespace Code {
	
	class Generator {
		
	public:
		Generator(std::string moduleName);
		~Generator();
		
		void parseAST(std::vector<Syntax::Statements::Statement *> statements);
		void dump() const;
		
	private:
		void genImportStatement(int i);
		void genDeclarationStatement(int i);
		void genFunctionDeclarationStatement(int i);
		
	private:
		std::string moduleName;
		
		llvm::Module *module;
		llvm::IRBuilder<> *builder;
		
		std::vector<Syntax::Statements::Statement *> statements;
	};

}
