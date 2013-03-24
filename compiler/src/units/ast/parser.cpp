#include "units/ast.h"

using namespace AST;

Parser::Parser() {
	
}

Parser::~Parser() {
	
}

void Parser::addError(Lexical::Token *token, std::string expected) {
	std::string filename = token->getFilename();
	int line = token->getLineNumber();
	int col = token->getColumn();
	std::string tokenValue = token->getValue();
	std::string msg = "Got '" + tokenValue + "', expected '" + expected + "'.";
	
	this->addMessage(Message("Error", filename, line, col, tokenValue, msg));
}

void Parser::parseTokens(std::vector<Lexical::Token *> tokens) {
	readTokens(tokens);
	generateAST();
}

std::vector<Statements::Statement *> Parser::getStatements() {
	return this->statements;
}

void Parser::readTokens(std::vector<Lexical::Token *> tokens) {
	this->tokens.clear();
	
	for (Lexical::Token *token : tokens) {
		if (token->getRule()->getType() != Lexical::Rule::Ignore) {
			this->tokens.push_back(token);
		}
	}
}

void Parser::generateAST() {
	int index = 0;
	for (; index < this->tokens.size(); ) {
		if (isImportStatement(&index)) {
			this->statements.push_back(readImportStatement(&index));
		} else {
			this->statements.push_back(readDeclarationStatement(&index));
		}
	}
}

// Statements
bool Parser::isStatement(int *index) {
	return isExpressionStatement(index);
}

Statements::Statement *Parser::readStatement(int *index) {
	if (isBlockStatement(index)) {
		return readBlockStatement(index);
	} else if (isImportStatement(index)) {
		return readImportStatement(index);
	} else if (isReturnStatement(index)) {
		return readReturnStatement(index);
	} else if (isControlStatement(index)) {
		return readControlStatement(index);
	} else if (isDeclarationStatement(index)) {
		return readDeclarationStatement(index);
	} else {
		return readExpressionStatement(index);
	}
}

bool Parser::isBlockStatement(int *index) {
	return isDelimiterToken(index, "{");
}

Statements::Block *Parser::readBlockStatement(int *index) {
	Statements::Block *block = new Statements::Block(this->tokens[*index]);
	
	readDelimiterToken(index, "{");
	while (!isDelimiterToken(index, "}")) {
		block->addStatement(readStatement(index));
	}
	readDelimiterToken(index, "}");
	
	return block;
}

bool Parser::isExpressionStatement(int *index) {
	return isExpression(index);
}

Statements::Expression *Parser::readExpressionStatement(int *index) {
	Statements::Expression *stat = new Statements::Expression(this->tokens[*index]);
	stat->setExpression(readExpression(index));
	readDelimiterToken(index, ";");
	return stat;
}

bool Parser::isImportStatement(int *index) {
	return isKeywordToken(index, "import");
}

Statements::Import *Parser::readImportStatement(int *index) {
	readKeywordToken(index, "import");
	
	Statements::Import *import = new Statements::Import(this->tokens[*index]);
	import->setIdentifier(readIdentifierExpression(index));
	readDelimiterToken(index, ";");
	return import;
}

bool Parser::isReturnStatement(int *index) {
	return isKeywordToken(index, "return");
}

Statements::Return *Parser::readReturnStatement(int *index) {
	readKeywordToken(index, "return");
	
	Statements::Return *ret = new Statements::Return(this->tokens[*index]);
	ret->setExpression(readExpression(index));
	readDelimiterToken(index, ";");
	return ret;
}

bool Parser::isControlStatement(int *index) {
	return isIfStatement(index) || isSwitchStatement(index);
}

Statements::Control *Parser::readControlStatement(int *index) {
	if (isIfStatement(index)) {
		return readIfStatement(index);
	} else if (isSwitchStatement(index)) {
		return readSwitchStatement(index);
	}
	
	this->addError(this->tokens[*index], "control");
	(*index)++;
	return NULL;
}

bool Parser::isIfStatement(int *index) {
	return isKeywordToken(index, "if");
}

Statements::If *Parser::readIfStatement(int *index) {
	readKeywordToken(index, "if");
	
	Statements::If *stat = new Statements::If(this->tokens[*index]);
	
	readDelimiterToken(index, "(");
	stat->setExpression(readExpression(index));
	readDelimiterToken(index, ")");
	
	stat->setTrueStatement(readBlockStatement(index));
	
	if (isKeywordToken(index, "else")) {
		readKeywordToken(index, "else");
		if (isIfStatement(index)) {
			stat->setFalseStatement(readIfStatement(index));
		} else {
			stat->setFalseStatement(readBlockStatement(index));
		}
	}
	
	return stat;
}

bool Parser::isCaseStatement(int *index) {
	return isKeywordToken(index, "case") || isKeywordToken(index, "default");
}

Statements::Case *Parser::readCaseStatement(int *index) {
	Statements::Case *statement = new Statements::Case(this->tokens[*index]);
	if (isKeywordToken(index, "case")) {
		readKeywordToken(index, "case");
		readDelimiterToken(index, "(");
		statement->setExpression(readExpression(index));
		readDelimiterToken(index, ")");
	} else if (isKeywordToken(index, "default")) {
		readKeywordToken(index, "default");
		statement->setExpression(NULL);
	} else {
		this->addError(this->tokens[*index], "case/default");
	}
	
	statement->setBlock(readBlockStatement(index));
	return statement;
}

bool Parser::isSwitchStatement(int *index) {
	return isKeywordToken(index, "switch");
}

Statements::Switch *Parser::readSwitchStatement(int *index) {
	readKeywordToken(index, "switch");
	
	Statements::Switch *statement = new Statements::Switch(this->tokens[*index]);
	
	readDelimiterToken(index, "(");
	statement->setExpression(readExpression(index));
	readDelimiterToken(index, ")");
	
	readDelimiterToken(index, "{");
	
	while (isCaseStatement(index)) {
		statement->addCase(readCaseStatement(index));
	}
	
	readDelimiterToken(index, "}");
	
	return statement;
}

bool Parser::isDeclarationStatement(int *index) {
	return isFunctionDeclarationStatement(index)
			|| isTypeDeclarationStatement(index)
			|| isVariableDeclarationStatement(index)
			|| isConstantDeclarationStatement(index);
}

Statements::Declaration *Parser::readDeclarationStatement(int *index) {
	if (isFunctionDeclarationStatement(index)) {
		return readFunctionDeclarationStatement(index);
	} else if (isTypeDeclarationStatement(index)) {
		return readTypeDeclarationStatement(index);
	} else if (isVariableDeclarationStatement(index)) {
		return readVariableDeclarationStatement(index);
	} else if (isConstantDeclarationStatement(index)) {
		return readConstantDeclarationStatement(index);
	}
	
	this->addError(this->tokens[*index], "declaration");
	(*index)++;
	return NULL;
}

bool Parser::isVariableDeclarationStatement(int *index) {
	return isKeywordToken(index, "var");
}

Statements::VariableDeclaration *Parser::readVariableDeclarationStatement(int *index) {
	readKeywordToken(index, "var");
	
	Statements::VariableDeclaration *decl = new Statements::VariableDeclaration(this->tokens[*index]);
	
	if (isKeywordToken(index, "exported")) {
		readKeywordToken(index, "exported");
		decl->setExported(true);
	}
	
	decl->setType(readTypeExpression(index));
	decl->setName(readIdentifierExpression(index));
	
	if (isOperatorToken(index, "=")) {
		readOperatorToken(index, "=");
		decl->setAssignment(readExpression(index));
	}
	
	readDelimiterToken(index, ";");
	
	return decl;
}

bool Parser::isConstantDeclarationStatement(int *index) {
	return isKeywordToken(index, "const");
}

Statements::ConstantDeclaration *Parser::readConstantDeclarationStatement(int *index) {
	readKeywordToken(index, "const");
	
	Statements::ConstantDeclaration *decl = new Statements::ConstantDeclaration(this->tokens[*index]);
	
	if (isKeywordToken(index, "exported")) {
		readKeywordToken(index, "exported");
		decl->setExported(true);
	}
	
	decl->setType(readTypeExpression(index));
	decl->setName(readIdentifierExpression(index));
	
	if (isOperatorToken(index, "=")) {
		readOperatorToken(index, "=");
		decl->setAssignment(readExpression(index));
	}
	
	readDelimiterToken(index, ";");
	
	return decl;
}

bool Parser::isTypeDeclarationStatement(int *index) {
	return isKeywordToken(index, "type");
}

Statements::TypeDeclaration *Parser::readTypeDeclarationStatement(int *index) {
	readKeywordToken(index, "type");
	
	Statements::TypeDeclaration *decl = new Statements::TypeDeclaration(this->tokens[*index]);
	
	if (isKeywordToken(index, "exported")) {
		readKeywordToken(index, "exported");
		decl->setExported(true);
	}
	
	decl->setType(readTypeExpression(index));
	decl->setName(readIdentifierExpression(index));
	decl->setBlock(readBlockStatement(index));
	return decl;
}

bool Parser::isFunctionDeclarationStatement(int *index) {
	return isKeywordToken(index, "func");
}

Statements::FunctionDeclaration *Parser::readFunctionDeclarationStatement(int *index) {
	readKeywordToken(index, "func");
	
	Statements::FunctionDeclaration *decl = new Statements::FunctionDeclaration(this->tokens[*index]);
	
	if (isKeywordToken(index, "exported")) {
		readKeywordToken(index, "exported");
		decl->setExported(true);
	}
	
	decl->setType(readTypeExpression(index));
	decl->setName(readIdentifierExpression(index));
	
	readDelimiterToken(index, "(");
	
	while (!isDelimiterToken(index, ")")) {
		decl->addParameter(readParameterExpression(index));
		
		if (isDelimiterToken(index, ",")) {
			readDelimiterToken(index, ",");
		} else {
			break;
		}
	}
	
	readDelimiterToken(index, ")");
	
	decl->setBlock(readBlockStatement(index));
	return decl;
}

// Expressions
bool Parser::isExpression(int *index) {
	return isUnaryExpression(index) || isBinaryExpression(index);
}

Expressions::Expression *Parser::readExpression(int *index) {
	Expressions::Unary *unary = readUnaryExpression(index);
	if (isBinaryOperator(index)) {
		return readBinaryExpression(index, unary, 0);
	}
	
	return unary;
}

bool Parser::isUnaryExpression(int *index) {
	return isUnaryOperator(index) || isOperandExpression(index);
}

Expressions::Unary *Parser::readUnaryExpression(int *index) {
	Expressions::Unary *unary = new Expressions::Unary(this->tokens[*index]);
	
	if (isUnaryOperator(index)) {
		unary->setOperator(readUnaryOperator(index));
		unary->setExpression(readUnaryExpression(index));
	} else {
		Operators::Unary *op = new Operators::Unary();
		op->setType(Operators::Unary::None);
		
		unary->setOperator(op);
		unary->setExpression(readOperandExpression(index));
	}
	
	return unary;
}

bool Parser::isBinaryExpression(int *index) {
	return isBinaryOperator(index);
}

Expressions::Binary *Parser::readBinaryExpression(int *index, Expressions::Expression *lhs, int minPrecedence) {
	while (isBinaryOperator(index) && getBinaryOperator(index)->getPrecedence() >= minPrecedence) {
		Operators::Binary *op = readBinaryOperator(index);
		Expressions::Expression *rhs = readOperandExpression(index);
		
		while (isBinaryOperator(index) && getBinaryOperator(index)->getPrecedence() > op->getPrecedence()) {
			rhs = readBinaryExpression(index, rhs, getBinaryOperator(index)->getPrecedence());
		}
		
		Expressions::Binary *b = new Expressions::Binary(this->tokens[*index]);
		b->setOperator(op);
		b->setLeft(lhs);
		b->setRight(rhs);
		lhs = b;
	}
	
	return static_cast<Expressions::Binary *>(lhs);
}

bool Parser::isOperandExpression(int *index) {
	return isLiteralExpression(index)
		|| isIdentifierExpression(index)
		|| isExpressionExpression(index);
}

Expressions::Operand *Parser::readOperandExpression(int *index) {
	Expressions::Operand *operand = NULL;
	
	if (isExpressionExpression(index)) {
		operand = readExpressionExpression(index);
	} else if (isLiteralExpression(index)) {
		operand = readLiteralExpression(index);
	} else if (isIdentifierExpression(index)) {
		operand = readIdentifierExpression(index);
	}
	
	if (operand != NULL) {
		for ( ; *index < this->tokens.size(); ) {
			if (isSelectorExpression(index)) {
				operand = readSelectorExpression(index, operand);
			} else if (isCallExpression(index)) {
				operand = readCallExpression(index, operand);
			} else if (isSliceExpression(index)) {
				operand = readSliceExpression(index, operand);
			} else {
				break;
			}
		}
		
		return operand;
	}
	
	this->addError(this->tokens[*index], "operand");
	(*index)++;
	return NULL;
}

bool Parser::isExpressionExpression(int *index) {
	return isDelimiterToken(index, "(");
}

Expressions::Expr *Parser::readExpressionExpression(int *index) {
	readDelimiterToken(index, "(");
	Expressions::Expr *expr = new Expressions::Expr(this->tokens[*index]);
	expr->setExpression(readExpression(index));
	readDelimiterToken(index, ")");
	return expr;
}

bool Parser::isLiteralExpression(int *index) {
	return isLiteralToken(index);
}

Expressions::Literal *Parser::readLiteralExpression(int *index) {
	Expressions::Literal *lit = new Expressions::Literal(this->tokens[*index]);
	lit->setValue(readLiteralToken(index)->getValue());
	return lit;
}

bool Parser::isIdentifierExpression(int *index) {
	return isIdentifierToken(index);
}

Expressions::Identifier *Parser::readIdentifierExpression(int *index) {
	Expressions::Identifier *expr = new Expressions::Identifier(this->tokens[*index]);
	expr->setValue(readIdentifierToken(index)->getValue());
	return expr;
}

bool Parser::isSelectorExpression(int *index) {
	int next = *index + 1;
	return isDelimiterToken(index, ".") && isIdentifierToken(&next);
}

Expressions::Selector *Parser::readSelectorExpression(int *index, Expressions::Operand *operand) {
	readDelimiterToken(index, ".");
	
	Expressions::Selector *selector = new Expressions::Selector(this->tokens[*index]);
	selector->setOperand(operand);
	selector->setIdentifier(readIdentifierExpression(index));
	return selector;
}

bool Parser::isCallExpression(int *index) {
	return isDelimiterToken(index, "(");
}

Expressions::Call *Parser::readCallExpression(int *index, Expressions::Operand *operand) {
	Expressions::Call *call = new Expressions::Call(this->tokens[*index]);
	call->setOperand(operand);
	
	readDelimiterToken(index, "(");
	
	while (!isDelimiterToken(index, ")")) {
		call->addArgument(readExpression(index));
		
		if (isDelimiterToken(index, ",")) {
			readDelimiterToken(index, ",");
		} else {
			break;
		}
	}
	
	readDelimiterToken(index, ")");
	
	return call;
}

bool Parser::isSliceExpression(int *index) {
	return isDelimiterToken(index, "[");
}

Expressions::Slice *Parser::readSliceExpression(int *index, Expressions::Operand *operand) {
	readDelimiterToken(index, "[");
	
	Expressions::Slice *slice = new Expressions::Slice(this->tokens[*index]);
	slice->setIndex(readExpression(index));
	slice->setOperand(operand);
	
	readDelimiterToken(index, "]");
	
	return slice;
}

bool Parser::isTypeExpression(int *index) {
	return isIdentifierExpression(index);
}

Expressions::Type *Parser::readTypeExpression(int *index) {
	Expressions::Type *type = new Expressions::Type(this->tokens[*index]);
	
	type->addName(readIdentifierExpression(index));
	
	while (isSelectorExpression(index)) {
		readDelimiterToken(index, ".");
		type->addName(readIdentifierExpression(index));
	}
	
	while (isSliceExpression(index)) {
		type->addSlice(readSliceExpression(index, NULL));
	}
	
	return type;
}

bool Parser::isParameterExpression(int *index) {
	return isTypeExpression(index);
}

Expressions::Parameter *Parser::readParameterExpression(int *index) {
	Expressions::Parameter *param = new Expressions::Parameter(this->tokens[*index]);
	param->setType(readTypeExpression(index));
	param->setName(readIdentifierExpression(index));
	return param;
}

// Operators
bool Parser::isUnaryOperator(int *index) {
	return isOperatorToken(index, "+")
		|| isOperatorToken(index, "-")
		|| isOperatorToken(index, "!")
		|| isOperatorToken(index, "++")
		|| isOperatorToken(index, "--");
}

Operators::Unary *Parser::readUnaryOperator(int *index) {
	Operators::Unary *op = new Operators::Unary();
	
	if (isOperatorToken(index, "+")) {
		op->setType(Operators::Unary::Add);
	} else if (isOperatorToken(index, "-")) {
		op->setType(Operators::Unary::Subtract);
	} else if (isOperatorToken(index, "!")) {
		op->setType(Operators::Unary::Not);
	} else if (isOperatorToken(index, "++")) {
		op->setType(Operators::Unary::Increment);
	} else if (isOperatorToken(index, "--")) {
		op->setType(Operators::Unary::Decrement);
	} else {
		this->addError(this->tokens[*index], "unary operator");
		(*index)++;
		return NULL;
	}
	
	(*index)++;
	return op;
}

bool Parser::isBinaryOperator(int *index) {
	const int opsLength = 32;
	const char *ops[opsLength] = { "=", "+", "-", "*", "/", "%", "!", "&", "|",
		"^", ">", "<", "==", "!=", "<=", ">=", "<<", ">>", "&^", "&&", "||",
		"+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", ">>=", "<<=", "&^=" };
	
	for (int i = 0; i < opsLength; i++) {
		if (isOperatorToken(index, ops[i])) {
			return true;
		}
	}
	
	return false;
}

Operators::Binary *Parser::getBinaryOperator(int *index) {
	Operators::Binary *op = new Operators::Binary();
	
	if (isOperatorToken(index, "+")) {
		op->setType(Operators::Binary::Add);
	} else if (isOperatorToken(index, "-")) {
		op->setType(Operators::Binary::Subtract);
	} else if (isOperatorToken(index, "*")) {
		op->setType(Operators::Binary::Multiply);
	} else if (isOperatorToken(index, "/")) {
		op->setType(Operators::Binary::Divide);
	} else if (isOperatorToken(index, "%")) {
		op->setType(Operators::Binary::Modulus);
	} else if (isOperatorToken(index, "&")) {
		op->setType(Operators::Binary::BitwiseAnd);
	} else if (isOperatorToken(index, "|")) {
		op->setType(Operators::Binary::BitwiseOr);
	} else if (isOperatorToken(index, "^")) {
		op->setType(Operators::Binary::BitwiseNot);
	} else if (isOperatorToken(index, ">")) {
		op->setType(Operators::Binary::GreaterThan);
	} else if (isOperatorToken(index, "<")) {
		op->setType(Operators::Binary::LessThan);
	} else if (isOperatorToken(index, "==")) {
		op->setType(Operators::Binary::Equals);
	} else if (isOperatorToken(index, "!=")) {
		op->setType(Operators::Binary::NotEquals);
	} else if (isOperatorToken(index, "<=")) {
		op->setType(Operators::Binary::LessThanEquals);
	} else if (isOperatorToken(index, ">=")) {
		op->setType(Operators::Binary::GreaterThanEquals);
	} else if (isOperatorToken(index, "<<")) {
		op->setType(Operators::Binary::LeftShift);
	} else if (isOperatorToken(index, ">>")) {
		op->setType(Operators::Binary::RightShift);
	} else if (isOperatorToken(index, "&^")) {
		op->setType(Operators::Binary::BitwiseAndOr);
	} else if (isOperatorToken(index, "&&")) {
		op->setType(Operators::Binary::And);
	} else if (isOperatorToken(index, "||")) {
		op->setType(Operators::Binary::Or);
	} else if (isOperatorToken(index, "=")) {
		op->setType(Operators::Binary::Assignment);
	} else {
		this->addError(this->tokens[*index], "binary operator");
		return NULL;
	}
	
	return op;
}

Operators::Binary *Parser::readBinaryOperator(int *index) {
	Operators::Binary *op = getBinaryOperator(index);
	(*index)++;
	return op;
}

// Tokens
bool Parser::isToken(int *index, Lexical::Rule::Type type, std::string value) {
	Lexical::Token *token = this->tokens[*index];
	if (token->getRule()->getType() == type) {
		if (value == "") {
			return true;
		}
		
		return token->getValue() == value;
	}
	
	return false;
}

Lexical::Token *Parser::readToken(int *index, Lexical::Rule::Type type, std::string value) {
	Lexical::Token *token = this->tokens[*index];
	if (isToken(index, type, value)) {
		(*index)++;
		return token;
	}
	
	(*index)++;
	
	if (value.empty()) {
		this->addError(token, "token");
	} else {
		this->addError(token, value);
	}
	
	return NULL;
}

bool Parser::isIdentifierToken(int *index) {
	return isToken(index, Lexical::Rule::Identifier, "");
}

Lexical::Token *Parser::readIdentifierToken(int *index) {
	return readToken(index, Lexical::Rule::Identifier, "");
}

bool Parser::isOperatorToken(int *index, std::string op) {
	return isToken(index, Lexical::Rule::Operator, op);
}

Lexical::Token *Parser::readOperatorToken(int *index, std::string op) {
	return readToken(index, Lexical::Rule::Operator, op);
}

bool Parser::isDelimiterToken(int *index, std::string delimiter) {
	return isToken(index, Lexical::Rule::Delimiter, delimiter);
}

Lexical::Token *Parser::readDelimiterToken(int *index, std::string delimiter) {
	return readToken(index, Lexical::Rule::Delimiter, delimiter);
}

bool Parser::isKeywordToken(int *index, std::string keyword) {
	return isToken(index, Lexical::Rule::Keyword, keyword);
}

Lexical::Token *Parser::readKeywordToken(int *index, std::string keyword) {
	return readToken(index, Lexical::Rule::Keyword, keyword);
}

bool Parser::isLiteralToken(int *index) {
	return isToken(index, Lexical::Rule::IntegerLiteral, "")
		|| isToken(index, Lexical::Rule::FloatLiteral, "")
		|| isToken(index, Lexical::Rule::ComplexLiteral, "")
		|| isToken(index, Lexical::Rule::BooleanLiteral, "")
		|| isToken(index, Lexical::Rule::StringLiteral, "");
}

Lexical::Token *Parser::readLiteralToken(int *index) {
	if (isToken(index, Lexical::Rule::IntegerLiteral, "")) {
		return readToken(index, Lexical::Rule::IntegerLiteral, "");
	} else if (isToken(index, Lexical::Rule::FloatLiteral, "")) {
		return readToken(index, Lexical::Rule::FloatLiteral, "");
	} else if (isToken(index, Lexical::Rule::ComplexLiteral, "")) {
		return readToken(index, Lexical::Rule::ComplexLiteral, "");
	} else if (isToken(index, Lexical::Rule::BooleanLiteral, "")) {
		return readToken(index, Lexical::Rule::BooleanLiteral, "");
	} else if (isToken(index, Lexical::Rule::StringLiteral, "")) {
		return readToken(index, Lexical::Rule::StringLiteral, "");
	}
	
	this->addError(this->tokens[*index], "literal");
	return NULL;
}
