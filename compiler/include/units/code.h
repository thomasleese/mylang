#pragma once

#include <iostream>
#include <vector>

#include <llvm/DerivedTypes.h>
#include <llvm/IRBuilder.h>
#include <llvm/MDBuilder.h>
#include <llvm/LLVMContext.h>
#include <llvm/Module.h>
#include <llvm/Bitcode/ReaderWriter.h>
#include <llvm/Support/raw_ostream.h>

#include "units/ast.h"

namespace llvm {
	
	class Module;
	
}

namespace Code {
	
	class Generator;
	
	class Pass {
		
	public:
		Pass(Generator *gen);
		~Pass();
		
		void parseAST(AST::Blocks::Module *block);
		
	private:
		void readAST(AST::Blocks::Module *block);
		virtual void generateCode() = 0;
		
	protected:
		llvm::Type *parseTypeExpression(AST::Expressions::QualifiedIdentifier *ident);
		llvm::Type *parseTypeExpression(AST::Expressions::Type *expr, AST::Statements::TypeDeclaration *typeDecl = NULL);
		
		AST::Statements::FunctionDeclaration *convertTypeFunctionToFunction(AST::Statements::FunctionDeclaration *func, AST::Statements::TypeDeclaration *type);
		
	protected:
		Generator *generator;
		llvm::Module *module;
		llvm::IRBuilder<> *irBuilder;
		llvm::MDBuilder *mdBuilder;
		AST::Blocks::Module *block;
		
	};
	
	class DeclarationPass : public Pass {
		
	public:
		DeclarationPass(Generator *gen);
		
	private:
		void generateCode();
		
		void parseImportStatement(AST::Statements::Import *import);
		void parseTypeDeclarationStatement(AST::Statements::TypeDeclaration *decl);
		void parseConstantDeclarationStatement(AST::Statements::ConstantDeclaration *decl);
		void parseFunctionDeclarationStatement(AST::Statements::FunctionDeclaration *decl);
		
	};
	
	class DefinitionPass : public Pass {
		
	public:
		DefinitionPass(Generator *gen);
		
	private:
		void generateCode();
		
		void parseTypeDeclarationStatement(AST::Statements::TypeDeclaration *decl);
		void parseConstantDeclarationStatement(AST::Statements::ConstantDeclaration *decl);
		void parseFunctionDeclarationStatement(AST::Statements::FunctionDeclaration *decl);
		
	};
	
	class Generator : public Unit {
		
	public:
		Generator(std::string moduleName);
		~Generator();
		
		void addError(Lexical::Token *token, std::string msg);
		
		void parseAST(AST::Blocks::Module *block);
		
		llvm::Module *getModule() const;
		
		void dump() const;
		void write(std::string buildDir) const;
		
	private:
		std::string moduleName;
		llvm::Module *module;
		AST::Blocks::Module *block;
	};

}
