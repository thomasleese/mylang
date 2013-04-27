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
	
	this->parseGenericBlock(decl->getBlock());
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
			
		case AST::Operators::Binary::Subtract:
			return this->irBuilder->CreateSub(left, right, "tmp");
			
		case AST::Operators::Binary::Multiply:
			return this->irBuilder->CreateMul(left, right, "tmp");
			
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
	AST::Expressions::Literal *literalExpr = dynamic_cast<AST::Expressions::Literal *>(expr);
	if (literalExpr != NULL) {
		return this->parseLiteralExpression(literalExpr);
	}
	
	AST::Expressions::Identifier *identifierExpr = dynamic_cast<AST::Expressions::Identifier *>(expr);
	if (identifierExpr != NULL) {
		return this->parseIdentifierExpression(identifierExpr);
	}
	
	AST::Expressions::Call *callExpr = dynamic_cast<AST::Expressions::Call *>(expr);
	if (callExpr != NULL) {
		return this->parseCallExpression(callExpr);
	}
	
	return NULL;
}

llvm::Value *DefinitionPass::parseLiteralExpression(AST::Expressions::Literal *expr) {
	AST::Expressions::IntegerLiteral *intExpr = dynamic_cast<AST::Expressions::IntegerLiteral *>(expr);
	if (intExpr != NULL) {
		return parseIntegerLiteralExpression(intExpr);
	}
	
	return NULL;
}

llvm::Value *DefinitionPass::parseIntegerLiteralExpression(AST::Expressions::IntegerLiteral *expr) {
	llvm::Type *type = llvm::Type::getInt64Ty(llvm::getGlobalContext());
	return llvm::ConstantInt::get(type, expr->getValue(), true);
}

llvm::Value *DefinitionPass::parseIdentifierExpression(AST::Expressions::Identifier *expr) {
	llvm::Function *func = this->irBuilder->GetInsertBlock()->getParent();
	
	// first we see if it's an argument
	for (llvm::Function::arg_iterator i = func->arg_begin(), e = func->arg_end(); i != e; ++i) {
		if (i->getName() == expr->getValue()) {
			return i;
		}
	}
	
	// then we look for local variables
	
	// then we look for global functions
	llvm::Function *otherFunc = this->module->getFunction(expr->getValue());
	if (otherFunc != NULL) {
		return otherFunc;
	}
	
	// then we look for global variables
	
	// clearly it's none of the above...
	return NULL;
}

llvm::Value *DefinitionPass::parseCallExpression(AST::Expressions::Call *expr) {
	llvm::Value *funcValue = this->parseOperandExpression(expr->getOperand());
	
	llvm::Function *func = dynamic_cast<llvm::Function *>(funcValue);
	if (func != NULL) {
		std::vector<llvm::Value *> args;
		
		for (AST::Expressions::Expression *argExpr : expr->getArguments()) {
			args.push_back(this->parseExpression(argExpr));
		}
		
		return this->irBuilder->CreateCall(func, llvm::ArrayRef<llvm::Value *>(args), "tmp");
	}
	
	return NULL;
}

void DefinitionPass::parseGenericBlock(AST::Blocks::Generic *block) {
	for (AST::Statements::Generic *stat : block->getGenericStatements()) {
		this->parseGenericStatement(stat);
	}
}

void DefinitionPass::parseGenericStatement(AST::Statements::Generic *stat) {
	AST::Statements::Expression *exprStatement = dynamic_cast<AST::Statements::Expression *>(stat);
	if (exprStatement != NULL) {
		this->parseExpressionStatement(exprStatement);
	}
	
	AST::Statements::Return *returnStatement = dynamic_cast<AST::Statements::Return *>(stat);
	if (returnStatement != NULL) {
		this->parseReturnStatement(returnStatement);
	}
	
	AST::Statements::Control *controlStatement = dynamic_cast<AST::Statements::Control *>(stat);
	if (controlStatement != NULL) {
		this->parseControlStatement(controlStatement);
	}
}

void DefinitionPass::parseExpressionStatement(AST::Statements::Expression *stat) {
	this->parseExpression(stat->getExpression());
}

void DefinitionPass::parseReturnStatement(AST::Statements::Return *stat) {
	if (stat->getExpression() == NULL) {
		this->irBuilder->CreateRetVoid();
	} else {
		llvm::Value *expr = this->parseExpression(stat->getExpression());
		if (expr != NULL) { // some sort of error above
			this->irBuilder->CreateRet(expr);
		}
	}
}

void DefinitionPass::parseControlStatement(AST::Statements::Control *stat) {
	AST::Statements::If *ifStatement = dynamic_cast<AST::Statements::If *>(stat);
	if (ifStatement != NULL) {
		return this->parseIfStatement(ifStatement);
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
		this->parseGenericBlock(stat->getTrueBlock());
		this->irBuilder->CreateBr(ifAfter);
		
		this->irBuilder->SetInsertPoint(ifFalse);
		
		if (stat->getFalseBlock() != NULL) {
			this->parseGenericBlock(stat->getFalseBlock());
		} else if (stat->getFalseIf() != NULL) {
			this->parseIfStatement(stat->getFalseIf());
		}
		
		this->irBuilder->CreateBr(ifAfter);
		
		this->irBuilder->SetInsertPoint(ifAfter);
	}
}
