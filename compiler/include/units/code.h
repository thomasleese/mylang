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
		
		void parseGlobalImportStatement(AST::Statements::Import *import);
		void parseGlobalTypeDeclarationStatement(AST::Statements::TypeDeclaration *decl);
		void parseGlobalVariableDeclarationStatement(AST::Statements::VariableDeclaration *decl);
		void parseGlobalFunctionDeclarationStatement(AST::Statements::FunctionDeclaration *decl);
		
	};
	
	class DefinitionPass : public Pass {
		
	public:
		DefinitionPass(Generator *gen);
		
	private:
		void generateCode();
		
		void parseGlobalTypeDeclarationStatement(AST::Statements::TypeDeclaration *decl);
		void parseGlobalVariableDeclarationStatement(AST::Statements::VariableDeclaration *decl);
		void parseGlobalFunctionDeclarationStatement(AST::Statements::FunctionDeclaration *decl);
		
		llvm::Value *parseExpression(AST::Expressions::Expression *expr);
		llvm::Value *parseBinaryExpression(AST::Expressions::Binary *expr);
		llvm::Value *parseUnaryExpression(AST::Expressions::Unary *expr);
		llvm::Value *parseOperandExpression(AST::Expressions::Operand *expr);
		llvm::Value *parseLiteralExpression(AST::Expressions::Literal *expr);
		llvm::Value *parseIntegerLiteralExpression(AST::Expressions::IntegerLiteral *expr);
		llvm::Value *parseIdentifierExpression(AST::Expressions::Identifier *expr);
		llvm::Value *parseCallExpression(AST::Expressions::Call *call);
		
		void parseGenericBlock(AST::Blocks::Generic *block);
		
		void parseGenericStatement(AST::Statements::Generic *stat);
		void parseExpressionStatement(AST::Statements::Expression *stat);
		void parseReturnStatement(AST::Statements::Return *stat);
		void parseControlStatement(AST::Statements::Control *stat);
		void parseIfStatement(AST::Statements::If *stat);
		
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
