#pragma once

#include <iostream>
#include <vector>

#include <llvm/DerivedTypes.h>
#include <llvm/PassManager.h>
#include <llvm/LLVMContext.h>
#include <llvm/GlobalValue.h>
#include <llvm/DataLayout.h>
#include <llvm/MDBuilder.h>
#include <llvm/IRBuilder.h>
#include <llvm/Module.h>
#include <llvm/Pass.h>

#include <llvm/Bitcode/ReaderWriter.h>

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/Interpreter.h>

#include <llvm/Analysis/Verifier.h>
#include <llvm/Analysis/Passes.h>

#include <llvm/Transforms/Scalar.h>

#include <llvm/ExecutionEngine/JIT.h>

#include <llvm/CodeGen/Passes.h>

#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/system_error.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/DataTypes.h>

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
		
		llvm::Module *findModuleByName(std::string name);
		
	protected:
		Generator *generator;
		llvm::Module *module;
		std::string moduleName;
		llvm::IRBuilder<> *irBuilder;
		llvm::MDBuilder *mdBuilder;
		llvm::FunctionPassManager *fpm;
		AST::Blocks::Module *block;
		
	};
	
	class DeclarationPass : public Pass {
		
	public:
		DeclarationPass(Generator *gen);
		
	private:
		void generateCode();
		
		void parseGlobalImportStatement(AST::Statements::Import *import);
		void parseGlobalImportStatement(std::string moduleName);
		
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
		llvm::Value *parseIdentifierExpression(std::string name);
		llvm::Value *parseSelectorExpression(AST::Expressions::Selector *expr);
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
		
		std::string getModuleName() const;
		llvm::Module *getModule() const;
		llvm::FunctionPassManager *getFunctionPassManager() const;
		
		void dump() const;
		void write(std::string buildDir) const;
		
	private:
		std::string moduleName;
		llvm::Module *module;
		llvm::ExecutionEngine *engine;
		llvm::FunctionPassManager *fpm;
		AST::Blocks::Module *block;
	};

}
