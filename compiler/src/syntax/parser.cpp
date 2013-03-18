#include "lex/analyser.h"
#include "syntax/parser.h"

using namespace Syntax;

Parser::Parser() {
	
}

Parser::~Parser() {
	
}

void Parser::parseTokens(std::vector<Lex::Token *> tokens) {
	readTokens(tokens);
	generateAST();
}

void Parser::dump() {
	for (Statements::Statement *s : this->statements) {
		std::cout << s->toString() << std::endl;
	}
}

void Parser::readTokens(std::vector<Lex::Token *> tokens) {
	this->tokens.clear();
	
	for (Lex::Token *token : tokens) {
		if (token->getRule()->getType() != Lex::Rule::Comment) {
			this->tokens.push_back(token);
		}
	}
}

void Parser::generateAST() {
	int index = 0;
	for (; index < this->tokens.size(); ) {
		this->statements.push_back(readStatement(&index));
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
	Statements::Block *block = new Statements::Block();
	
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
	Statements::Expression *stat = new Statements::Expression();
	stat->setExpression(readExpression(index));
	readDelimiterToken(index, ";");
	return stat;
}

bool Parser::isImportStatement(int *index) {
	return isKeywordToken(index, "import");
}

Statements::Import *Parser::readImportStatement(int *index) {
	readKeywordToken(index, "import");
	
	Statements::Import *import = new Statements::Import();
	import->setIdentifier(readIdentifierExpression(index));
	readDelimiterToken(index, ";");
	return import;
}

bool Parser::isReturnStatement(int *index) {
	return isKeywordToken(index, "return");
}

Statements::Return *Parser::readReturnStatement(int *index) {
	readKeywordToken(index, "return");
	
	Statements::Return *ret = new Statements::Return();
	ret->setExpression(readExpression(index));
	readDelimiterToken(index, ";");
	return ret;
}

bool Parser::isControlStatement(int *index) {
	return isIfStatement(index);
}

Statements::Control *Parser::readControlStatement(int *index) {
	if (isIfStatement(index)) {
		return readIfStatement(index);
	}
	
	throw "Control";
}

bool Parser::isIfStatement(int *index) {
	return isKeywordToken(index, "if");
}

Statements::If *Parser::readIfStatement(int *index) {
	readKeywordToken(index, "if");
	
	Statements::If *stat = new Statements::If();
	
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

bool Parser::isDeclarationStatement(int *index) {
	return isFunctionDeclarationStatement(index);
}

Statements::Declaration *Parser::readDeclarationStatement(int *index) {
	if (isFunctionDeclarationStatement(index)) {
		return readFunctionDeclarationStatement(index);
	}
	
	throw "Declaration";
}

bool Parser::isFunctionDeclarationStatement(int *index) {
	return isKeywordToken(index, "def");
}

Statements::FunctionDeclaration *Parser::readFunctionDeclarationStatement(int *index) {
	readKeywordToken(index, "def");
	
	Statements::FunctionDeclaration *decl = new Statements::FunctionDeclaration();
	decl->setType(readTypeExpression(index));
	decl->setIdentifier(readIdentifierExpression(index));
	
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
	Expressions::Unary *unary = new Expressions::Unary();
	
	if (isUnaryOperator(index)) {
		unary->setOperator(readUnaryOperator(index));
		unary->setExpression(readUnaryExpression(index));
	} else {
		unary->setOperator(Operators::None);
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
		
		Expressions::Binary *b = new Expressions::Binary();
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
	
	throw "Operand";
}

bool Parser::isExpressionExpression(int *index) {
	return isDelimiterToken(index, "(");
}

Expressions::Expr *Parser::readExpressionExpression(int *index) {
	readDelimiterToken(index, "(");
	Expressions::Expr *expr = new Expressions::Expr();
	expr->setExpression(readExpression(index));
	readDelimiterToken(index, ")");
	return expr;
}

bool Parser::isLiteralExpression(int *index) {
	return isLiteralToken(index);
}

Expressions::Literal *Parser::readLiteralExpression(int *index) {
	Expressions::Literal *lit = new Expressions::Literal();
	lit->setValue(readLiteralToken(index)->getValue());
	return lit;
}

bool Parser::isIdentifierExpression(int *index) {
	return isIdentifierToken(index);
}

Expressions::Identifier *Parser::readIdentifierExpression(int *index) {
	Expressions::Identifier *expr = new Expressions::Identifier();
	expr->setValue(readIdentifierToken(index)->getValue());
	return expr;
}

bool Parser::isSelectorExpression(int *index) {
	int next = *index + 1;
	return isDelimiterToken(index, ".") && isIdentifierToken(&next);
}

Expressions::Selector *Parser::readSelectorExpression(int *index, Expressions::Operand *operand) {
	readDelimiterToken(index, ".");
	
	Expressions::Selector *selector = new Expressions::Selector();
	selector->setOperand(operand);
	selector->setIdentifier(readIdentifierExpression(index));
	return selector;
}

bool Parser::isCallExpression(int *index) {
	return isDelimiterToken(index, "(");
}

Expressions::Call *Parser::readCallExpression(int *index, Expressions::Operand *operand) {
	Expressions::Call *call = new Expressions::Call();
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
	
	Expressions::Slice *slice = new Expressions::Slice();
	slice->setIndex(readExpression(index));
	slice->setOperand(operand);
	
	readDelimiterToken(index, "]");
	
	return slice;
}

bool Parser::isTypeExpression(int *index) {
	return isIdentifierExpression(index);
}

Expressions::Type *Parser::readTypeExpression(int *index) {
	Expressions::Type *type = new Expressions::Type();
	
	type->setIdentifier(readIdentifierExpression(index));
	
	while (isSelectorExpression(index)) {
		type->addSelector(readSelectorExpression(index, NULL));
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
	Expressions::Parameter *param = new Expressions::Parameter();
	param->setType(readTypeExpression(index));
	param->setIdentifier(readIdentifierExpression(index));
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

Operators::Unary Parser::readUnaryOperator(int *index) {
	if (isOperatorToken(index, "+")) {
		(*index)++;
		return Operators::Add;
	} else if (isOperatorToken(index, "-")) {
		(*index)++;
		return Operators::Subtract;
	} else if (isOperatorToken(index, "!")) {
		(*index)++;
		return Operators::Not;
	} else if (isOperatorToken(index, "++")) {
		(*index)++;
		return Operators::Increment;
	} else if (isOperatorToken(index, "--")) {
		(*index)++;
		return Operators::Decrement;
	}
	
	throw "UnaryOperator";
}

bool Parser::isBinaryOperator(int *index) {
	const int opsLength = 31;
	const char *ops[opsLength] = { "+", "-", "*", "/", "%", "!", "&", "|", "^",
		">", "<", "==", "!=", "<=", ">=", "<<", ">>", "&^", "&&", "||", "+=",
		"-=", "*=", "/=", "%=", "&=", "|=", "^=", ">>=", "<<=", "&^=" };
	
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
	} else {
		throw "BinaryOperator";
	}
	
	return op;
}

Operators::Binary *Parser::readBinaryOperator(int *index) {
	Operators::Binary *op = getBinaryOperator(index);
	(*index)++;
	return op;
}

// Tokens
bool Parser::isToken(int *index, Lex::Rule::Type type, std::string value) {
	Lex::Token *token = this->tokens[*index];
	if (token->getRule()->getType() == type) {
		if (value == "") {
			return true;
		}
		
		return token->getValue() == value;
	}
	
	return false;
}

Lex::Token *Parser::readToken(int *index, Lex::Rule::Type type, std::string value) {
	Lex::Token *token = this->tokens[*index];
	if (isToken(index, type, value)) {
		(*index)++;
		return token;
	}
	
	throw ("Token (" + value + ")").c_str();
}

bool Parser::isIdentifierToken(int *index) {
	return isToken(index, Lex::Rule::Identifier, "");
}

Lex::Token *Parser::readIdentifierToken(int *index) {
	return readToken(index, Lex::Rule::Identifier, "");
}

bool Parser::isOperatorToken(int *index, std::string op) {
	return isToken(index, Lex::Rule::Operator, op);
}

Lex::Token *Parser::readOperatorToken(int *index, std::string op) {
	return readToken(index, Lex::Rule::Operator, op);
}

bool Parser::isDelimiterToken(int *index, std::string delimiter) {
	return isToken(index, Lex::Rule::Delimiter, delimiter);
}

Lex::Token *Parser::readDelimiterToken(int *index, std::string delimiter) {
	return readToken(index, Lex::Rule::Delimiter, delimiter);
}

bool Parser::isKeywordToken(int *index, std::string keyword) {
	return isToken(index, Lex::Rule::Keyword, keyword);
}

Lex::Token *Parser::readKeywordToken(int *index, std::string keyword) {
	return readToken(index, Lex::Rule::Keyword, keyword);
}

bool Parser::isLiteralToken(int *index) {
	return isToken(index, Lex::Rule::IntegerLiteral, "")
		|| isToken(index, Lex::Rule::FloatLiteral, "")
		|| isToken(index, Lex::Rule::ComplexLiteral, "")
		|| isToken(index, Lex::Rule::BooleanLiteral, "")
		|| isToken(index, Lex::Rule::StringLiteral, "");
}

Lex::Token *Parser::readLiteralToken(int *index) {
	if (isToken(index, Lex::Rule::IntegerLiteral, "")) {
		return readToken(index, Lex::Rule::IntegerLiteral, "");
	} else if (isToken(index, Lex::Rule::FloatLiteral, "")) {
		return readToken(index, Lex::Rule::FloatLiteral, "");
	} else if (isToken(index, Lex::Rule::ComplexLiteral, "")) {
		return readToken(index, Lex::Rule::ComplexLiteral, "");
	} else if (isToken(index, Lex::Rule::BooleanLiteral, "")) {
		return readToken(index, Lex::Rule::BooleanLiteral, "");
	} else if (isToken(index, Lex::Rule::StringLiteral, "")) {
		return readToken(index, Lex::Rule::StringLiteral, "");
	}
	
	throw "Literal";
}
