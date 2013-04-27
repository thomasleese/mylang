#include "units/code.h"

using namespace Code;

DefinitionPass::DefinitionPass(Generator *gen) :
	Pass(gen) {
	
}

void DefinitionPass::generateCode() {
	for (AST::Statements::TypeDeclaration *decl : this->block->getTypeDeclarationStatements()) {
		parseGlobalTypeDeclarationStatement(decl);
	}
	
	for (AST::Statements::VariableDeclaration *decl : this->block->getVariableDeclarationStatements()) {
		parseGlobalVariableDeclarationStatement(decl);
	}
	
	for (AST::Statements::FunctionDeclaration *decl : this->block->getFunctionDeclarationStatements()) {
		parseGlobalFunctionDeclarationStatement(decl);
	}
}

void DefinitionPass::parseGlobalTypeDeclarationStatement(AST::Statements::TypeDeclaration *decl) {
	AST::Blocks::Type *block = decl->getBlock();
	for (AST::Statements::FunctionDeclaration *func : block->getFunctionDeclarationStatements()) {
		// we don't need to convert it, as this has already happend in the definition stage
		
		if (func != NULL) {
			parseGlobalFunctionDeclarationStatement(func);
		}
	}
}

void DefinitionPass::parseGlobalVariableDeclarationStatement(AST::Statements::VariableDeclaration *decl) {
	llvm::GlobalVariable *var = this->module->getGlobalVariable(decl->getName()->getValue());
	if (var == NULL) { // an error must've happened earlier
		return;
	}
	
	
}

void DefinitionPass::parseGlobalFunctionDeclarationStatement(AST::Statements::FunctionDeclaration *decl) {
	llvm::Function *func = this->module->getFunction(decl->getName()->getValue());
	if (func == NULL) { // an error must've occured earlier on
		return;
	}
	
	llvm::BasicBlock *entry = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", func);
	this->irBuilder->SetInsertPoint(entry);
	
	for (AST::Statements::Generic *stat : decl->getBlock()->getGenericStatements()) {
		AST::Statements::If *ifStatement = dynamic_cast<AST::Statements::If *>(stat);
		if (ifStatement != NULL) {
			this->parseIfStatement(ifStatement);
		}
	}
}

void DefinitionPass::parseIfStatement(AST::Statements::If *stat) {
	llvm::LLVMContext &ctx = llvm::getGlobalContext();
	
	llvm::Function *func = this->irBuilder->GetInsertBlock()->getParent();
	
	llvm::Value *expr = this->parseExpression(stat->getExpression());
	if (expr != NULL) {
		llvm::BasicBlock* ifTrue = llvm::BasicBlock::Create(ctx, "if_true", func);
		llvm::BasicBlock* ifFalse = llvm::BasicBlock::Create(ctx, "if_false", func);
		llvm::BasicBlock* ifAfter = llvm::BasicBlock::Create(ctx, "if_after", func);
		
		this->irBuilder->CreateCondBr(expr, ifTrue, ifFalse);
		
		this->irBuilder->SetInsertPoint(ifTrue);
		this->irBuilder->CreateBr(ifAfter);
		
		this->irBuilder->SetInsertPoint(ifFalse);
		this->irBuilder->CreateBr(ifAfter);
		
		this->irBuilder->SetInsertPoint(ifAfter);
	}
}

llvm::Value *DefinitionPass::parseExpression(AST::Expressions::Expression *expr) {
	AST::Expressions::Binary *binExpr = dynamic_cast<AST::Expressions::Binary *>(expr);
	if (binExpr != NULL) {
		return this->parseBinaryExpression(binExpr);
	}
	
	AST::Expressions::Unary *unaryExpr = dynamic_cast<AST::Expressions::Unary *>(expr);
	if (unaryExpr != NULL) {
		return this->parseUnaryExpression(unaryExpr);
	}
	
	AST::Expressions::Operand *operandExpr = dynamic_cast<AST::Expressions::Operand *>(expr);
	if (operandExpr != NULL) {
		return this->parseOperandExpression(operandExpr);
	}
	
	return NULL;
}

llvm::Value *DefinitionPass::parseBinaryExpression(AST::Expressions::Binary *expr) {
	llvm::Value *left = this->parseExpression(expr->getLeft());
	llvm::Value *right = this->parseExpression(expr->getRight());
	
	switch (expr->getOperator()->getType()) {
		case AST::Operators::Binary::Equals:
			return this->irBuilder->CreateICmpEQ(left, right, "tmp");
			
		default:
			return NULL;
	}
}

llvm::Value *DefinitionPass::parseUnaryExpression(AST::Expressions::Unary *expr) {
	if (expr->getOperator()->getType() == AST::Operators::Unary::None) {
		return this->parseExpression(expr->getExpression());
	}
	
	return NULL;
}

llvm::Value *DefinitionPass::parseOperandExpression(AST::Expressions::Operand *expr) {
	AST::Expressions::Identifier *identifierExpr = dynamic_cast<AST::Expressions::Identifier *>(expr);
	if (identifierExpr != NULL) {
		return this->parseIdentifierExpression(identifierExpr);
	}
	
	AST::Expressions::Literal *literalExpr = dynamic_cast<AST::Expressions::Literal *>(expr);
	if (literalExpr != NULL) {
		return this->parseLiteralExpression(literalExpr);
	}
	
	return NULL;
}

llvm::Value *DefinitionPass::parseIdentifierExpression(AST::Expressions::Identifier *expr) {
	llvm::Type *type = llvm::Type::getInt64Ty(llvm::getGlobalContext());
	return llvm::ConstantInt::get(type, 0, true);
}

llvm::Value *DefinitionPass::parseLiteralExpression(AST::Expressions::Literal *expr) {
	llvm::Type *type = llvm::Type::getInt64Ty(llvm::getGlobalContext());
	return llvm::ConstantInt::get(type, 0, true);
}
