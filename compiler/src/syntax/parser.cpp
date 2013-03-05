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
		readDelimiterToken(&index, ";");
	}
}

// Statements
bool Parser::isStatement(int *index) {
	return isExpressionStatement(index);
}

Statements::Statement *Parser::readStatement(int *index) {
	if (/*isVariableStatement...*/false) {
		return NULL;
	} else {
		return readExpressionStatement(index);
	}
}

bool Parser::isExpressionStatement(int *index) {
	return isExpression(index);
}

Statements::Statement *Parser::readExpressionStatement(int *index) {
	Statements::Expression *stat = new Statements::Expression();
	stat->setExpression(readExpression(index));
	return stat;
}

// Expressions
bool Parser::isExpression(int *index) {
	return isUnaryExpression(index);
}

Expressions::Expression *Parser::readExpression(int *index) {
	return readUnaryExpression(index);
}

bool Parser::isUnaryExpression(int *index) {
	return false;
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

bool Parser::isOperandExpression(int *index) {
	return isLiteralExpression(index)
		&& isIdentifierExpression(index);
}

Expressions::Operand *Parser::readOperandExpression(int *index) {
	Expressions::Operand *operand = NULL;
	
	if (isLiteralExpression(index)) {
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
	readDelimiterToken(index, "(");
	
	Expressions::Call *call = new Expressions::Call();
	//call->setArguments(readExpressionsList());
	call->setOperand(operand);
	
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
	
	throw "Token";
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
