#pragma once

#include <vector>

#include "syntax/expressions.h"
#include "syntax/operators.h"
#include "syntax/statements.h"

namespace Lex {
	class Token;
}

namespace Syntax {
	
	class ParserError {
		
	public:
		ParserError(Lex::Token *got, std::string expected);
		ParserError(int line, std::string filename, std::string message);
		
		void print();
		
	private:
		int line;
		std::string filename;
		std::string message;
		
	};
	
	class Parser {
		
	public:
		Parser();
		~Parser();
		
		void parseTokens(std::vector<Lex::Token *> tokens);
		void dump();
		
		std::vector<Statements::Statement *> getStatements();
		
	private:
		void readTokens(std::vector<Lex::Token *> tokens);
		void generateAST();
		
		// Statements
		bool isStatement(int *index);
		Statements::Statement *readStatement(int *index);
		
		bool isBlockStatement(int *index);
		Statements::Block *readBlockStatement(int *index);
		
		bool isExpressionStatement(int *index);
		Statements::Expression *readExpressionStatement(int *index);
		
		bool isImportStatement(int *index);
		Statements::Import *readImportStatement(int *index);
		
		bool isReturnStatement(int *index);
		Statements::Return *readReturnStatement(int *index);
		
		bool isControlStatement(int *index);
		Statements::Control *readControlStatement(int *index);
		
		bool isIfStatement(int *index);
		Statements::If *readIfStatement(int *index);
		
		bool isCaseStatement(int *index);
		Statements::Case *readCaseStatement(int *index);
		
		bool isSwitchStatement(int *index);
		Statements::Switch *readSwitchStatement(int *index);
		
		bool isDeclarationStatement(int *index);
		Statements::Declaration *readDeclarationStatement(int *index);
		
		bool isVariableDeclarationStatement(int *index);
		Statements::VariableDeclaration *readVariableDeclarationStatement(int *index);
		
		bool isConstantDeclarationStatement(int *index);
		Statements::ConstantDeclaration *readConstantDeclarationStatement(int *index);
		
		bool isTypeDeclarationStatement(int *index);
		Statements::TypeDeclaration *readTypeDeclarationStatement(int *index);
		
		bool isFunctionDeclarationStatement(int *index);
		Statements::FunctionDeclaration *readFunctionDeclarationStatement(int *index);
		
		// Expressions
		bool isExpression(int *index);
		Expressions::Expression *readExpression(int *index);
		
		bool isUnaryExpression(int *index);
		Expressions::Unary *readUnaryExpression(int *index);
		
		bool isBinaryExpression(int *index);
		Expressions::Binary *readBinaryExpression(int *index, Expressions::Expression *lhs, int minPrecedence);
		
		bool isOperandExpression(int *index);
		Expressions::Operand *readOperandExpression(int *index);
		
		bool isExpressionExpression(int *index);
		Expressions::Expr *readExpressionExpression(int *index);
		
		bool isLiteralExpression(int *index);
		Expressions::Literal *readLiteralExpression(int *index);
		
		bool isIdentifierExpression(int *index);
		Expressions::Identifier *readIdentifierExpression(int *index);
		
		bool isSelectorExpression(int *index);
		Expressions::Selector *readSelectorExpression(int *index, Expressions::Operand *operand);
		
		bool isCallExpression(int *index);
		Expressions::Call *readCallExpression(int *index, Expressions::Operand *operand);
		
		bool isSliceExpression(int *index);
		Expressions::Slice *readSliceExpression(int *index, Expressions::Operand *operand);
		
		bool isTypeExpression(int *index);
		Expressions::Type *readTypeExpression(int *index);
		
		bool isParameterExpression(int *index);
		Expressions::Parameter *readParameterExpression(int *index);
		
		// Operators
		bool isUnaryOperator(int *index);
		Operators::Unary readUnaryOperator(int *index);
		
		bool isBinaryOperator(int *index);
		Operators::Binary *getBinaryOperator(int *index);
		Operators::Binary *readBinaryOperator(int *index);
		
		// Tokens
		bool isToken(int *index, Lex::Rule::Type type, std::string value);
		Lex::Token *readToken(int *index, Lex::Rule::Type type, std::string value);
		
		bool isIdentifierToken(int *index);
		Lex::Token *readIdentifierToken(int *index);
		
		bool isOperatorToken(int *index, std::string op);
		Lex::Token *readOperatorToken(int *index, std::string op);
		
		bool isDelimiterToken(int *index, std::string delimiter);
		Lex::Token *readDelimiterToken(int *index, std::string delimiter);
		
		bool isKeywordToken(int *index, std::string keyword);
		Lex::Token *readKeywordToken(int *index, std::string keyword);
		
		bool isLiteralToken(int *index);
		Lex::Token *readLiteralToken(int *index);
		
	private:
		std::vector<Lex::Token *> tokens;
		std::vector<Statements::Statement *> statements;
		
	};
	
}
