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
	
	if (this->moduleName == "main") {
		llvm::Function *origFunc = this->module->getFunction("main_main");
		if (origFunc == NULL) {
			this->generator->addError(NULL, "A 'main' module requires a 'main' function.");
			return;
		}
		
		llvm::Function *func = static_cast<llvm::Function *>(this->module->getOrInsertFunction("main", origFunc->getReturnType(), NULL));
		llvm::BasicBlock *entry = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", func);
		this->irBuilder->SetInsertPoint(entry);
		this->irBuilder->CreateRet(this->irBuilder->CreateCall(origFunc, "tmp"));
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
	std::string name = this->moduleName + "_" + decl->getName()->getValue();
	
	llvm::GlobalVariable *var = this->module->getGlobalVariable(name, true);
	if (var == NULL) { // an error must've happened earlier
		return;
	}
	
	llvm::Value *value = this->parseExpression(decl->getAssignment());
	if (value != NULL) {
		llvm::Constant *constant = dynamic_cast<llvm::Constant *>(value);
		if (constant == NULL) {
			this->generator->addError(static_cast<AST::Statements::Declaration *>(decl)->getToken(), "Initialiser is not constant");
		} else {
			var->setInitializer(constant);
		}
	}
}

void DefinitionPass::parseGlobalFunctionDeclarationStatement(AST::Statements::FunctionDeclaration *decl) {
	std::string name = this->moduleName + "_" + decl->getName()->getValue();
	
	llvm::Function *func = this->module->getFunction(name);
	if (func == NULL) { // an error must've occured earlier on
		return;
	}
	
	llvm::BasicBlock *entry = llvm::BasicBlock::Create(llvm::getGlobalContext(), "entry", func);
	this->irBuilder->SetInsertPoint(entry);
	
	this->parseGenericBlock(decl->getBlock());
	
	if (this->irBuilder->GetInsertBlock()->getTerminator() == NULL) {
		this->irBuilder->CreateRetVoid();
	}
	
	this->fpm->run(*func);
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
			
		case AST::Operators::Binary::LessThan:
			return this->irBuilder->CreateFCmpOLT(left, right, "tmp");
			
		case AST::Operators::Binary::Subtract:
			return this->irBuilder->CreateSub(left, right, "tmp");
			
		case AST::Operators::Binary::Add:
			return this->irBuilder->CreateAdd(left, right, "tmp");
			
		case AST::Operators::Binary::Multiply:
			return this->irBuilder->CreateMul(left, right, "tmp");
			
		case AST::Operators::Binary::Divide:
			return this->irBuilder->CreateSDiv(left, right, "tmp");
			
		default:
			this->generator->addError(expr->getToken(), "Unknown operator");
			return NULL;
	}
}

llvm::Value *DefinitionPass::parseUnaryExpression(AST::Expressions::Unary *expr) {
	llvm::Value *value = this->parseExpression(expr->getExpression());
	llvm::LLVMContext &ctx = llvm::getGlobalContext();
	
	switch (expr->getOperator()->getType()) {
		case AST::Operators::Unary::None:
		case AST::Operators::Unary::Add:
			return value;
			
		case AST::Operators::Unary::Subtract:
			return this->irBuilder->CreateNeg(value, "tmp");
		
		default:
			this->generator->addError(expr->getToken(), "Unknown unary operator");
			return NULL;
	}
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
	
	AST::Expressions::Selector *selectorExpr = dynamic_cast<AST::Expressions::Selector *>(expr);
	if (selectorExpr != NULL) {
		return this->parseSelectorExpression(selectorExpr);
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
	return this->parseIdentifierExpression(expr->getValue());
}

llvm::Value *DefinitionPass::parseIdentifierExpression(std::string name) {
	llvm::Function *func = this->irBuilder->GetInsertBlock()->getParent();
	
	// first we see if it's an argument
	for (llvm::Function::arg_iterator i = func->arg_begin(), e = func->arg_end(); i != e; ++i) {
		if (i->getName() == name) {
			return i;
		}
	}
	
	// TODO then we look for local variables
	
	// then we look for global functions
	llvm::Function *otherFunc = this->module->getFunction(this->moduleName + "_" + name);
	if (otherFunc != NULL) {
		return otherFunc;
	}
	
	otherFunc = this->module->getFunction(name);
	if (otherFunc != NULL) {
		return otherFunc;
	}
	
	// then we look for global variables
	llvm::GlobalVariable *globalVar = this->module->getGlobalVariable(this->moduleName + "_" + name, true);
	if (globalVar != NULL) {
		return globalVar;
	}
	
	// then look for a module name
	llvm::Function *modFunc = this->module->getFunction("module_" + name);
	if (modFunc != NULL) {
		return modFunc;
	}
	
	// clearly it's none of the above...
	this->generator->addError(NULL, "Unknown identifier: " + name);
	return NULL;
}

llvm::Value *DefinitionPass::parseSelectorExpression(AST::Expressions::Selector *expr) {
	llvm::Value *left = this->parseOperandExpression(expr->getOperand());
	
	if (left == NULL) {
		// an error occured above here!
		return NULL;
	}
	
	if (left->hasName() && left->getName().str().substr(0, 7) == "module_") {
		// we're selecting a module!
		std::string moduleName = left->getName().str().substr(7);
		return this->parseIdentifierExpression(moduleName + "_" + expr->getIdentifier()->getValue());
	}
	
	return NULL;
}

llvm::Value *DefinitionPass::parseCallExpression(AST::Expressions::Call *expr) {
	llvm::Value *funcValue = this->parseOperandExpression(expr->getOperand());
	
	llvm::Function *func = dynamic_cast<llvm::Function *>(funcValue);
	if (func != NULL) {
		std::vector<llvm::Value *> args;
		
		for (AST::Expressions::Expression *argExpr : expr->getArguments()) {
			llvm::Value *arg = this->parseExpression(argExpr);
			if (arg != NULL) {
				args.push_back(arg);
			} else { // an error occured above here!
				return NULL;
			}
		}
		
		return this->irBuilder->CreateCall(func, args, "tmp");
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
		
		if (this->irBuilder->GetInsertBlock()->getTerminator() == NULL) {
			this->irBuilder->CreateBr(ifAfter);
		}
		
		this->irBuilder->SetInsertPoint(ifFalse);
		
		if (stat->getFalseBlock() != NULL) {
			this->parseGenericBlock(stat->getFalseBlock());
		} else if (stat->getFalseIf() != NULL) {
			this->parseIfStatement(stat->getFalseIf());
		}
		
		if (this->irBuilder->GetInsertBlock()->getTerminator() == NULL) {
			this->irBuilder->CreateBr(ifAfter);
		}
		
		this->irBuilder->SetInsertPoint(ifAfter);
	}
}
