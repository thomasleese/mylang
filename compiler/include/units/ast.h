#pragma once

#include "units/lexical.h"

namespace AST {
	
	namespace Operators {
		
		class Unary {
		
		public:
			enum Type {
				None,
				Add,
				Subtract,
				Not,
				Increment,
				Decrement,
			};
			
			void setType(Type t);
			Type getType();
			
		private:
			Type type;
		};
		
		class Binary {
			
		public:
			enum Type {
				None,
				Add,
				Subtract,
				Multiply,
				Divide,
				Modulus,
				BitwiseAnd,
				BitwiseOr,
				BitwiseNot,
				GreaterThan,
				LessThan,
				Equals,
				NotEquals,
				LessThanEquals,
				GreaterThanEquals,
				LeftShift,
				RightShift,
				BitwiseAndOr,
				And,
				Or,
				Assignment,
			};
			
			void setType(Type t);
			Type getType();
			
			void setPrecedence(int p);
			int getPrecedence();
			
		private:
			Type type;
			int precedence;
			
		};
		
	}
	
	namespace Expressions {
		
		class Expression {
			
		public:
			Expression(Lexical::Token *token);
			virtual ~Expression();
			
			Lexical::Token *getToken();
			
		private:
			Lexical::Token *token;
			
		};
		
		class Unary : public Expression {
			
		public:
			Unary(Lexical::Token *token);
			
			void setOperator(Operators::Unary *op);
			Operators::Unary *getOperator();
			
			void setExpression(Expression *expr);
			Expression *getExpression();
			
		private:
			Operators::Unary *op;
			Expression *expr;
			
		};
		
		class Binary : public Expression {
			
		public:
			Binary(Lexical::Token *token);
			
			void setOperator(Operators::Binary *op);
			Operators::Binary *getOperator();
			
			void setLeft(Expression *expr);
			Expression *getLeft();
			
			void setRight(Expression *expr);
			Expression *getRight();
			
		private:
			Operators::Binary *op;
			Expression *lhs, *rhs;
			
		};
		
		class Operand : public Expression {
			
		public:
			Operand(Lexical::Token *token);
			
		};
		
		class Expr : public Operand {
			
		public:
			Expr(Lexical::Token *token);
			
			void setExpression(Expression *expr);
			
		private:
			Expression *expr;
			
		};
		
		class Literal : public Operand {
			
		public:
			Literal(Lexical::Token *token);
			
			void setValue(std::string value);
			
		private:
			std::string value;
			
		};
		
		class Identifier : public Operand {
			
		public:
			Identifier(Lexical::Token *token);
			
			void setValue(std::string value);
			std::string getValue() const;
			
		private:
			std::string value;
			
		};
		
		class Selector : public Operand {
			
		public:
			Selector(Lexical::Token *token);
			
			void setOperand(Operand *expr);
			void setIdentifier(Identifier *identifier);
			
		private:
			Operand *operand;
			Identifier *identifier;
			
		};
		
		class Call : public Operand {
			
		public:
			Call(Lexical::Token *token);
			
			void setOperand(Operand *expr);
			void addArgument(Expression *arg);
			
		private:
			Operand *operand;
			std::vector<Expression *> arguments;
			
		};
		
		class Slice : public Operand {
			
		public:
			Slice(Lexical::Token *token);
			
			void setOperand(Operand *expr);
			void setIndex(Expression *expr);
			
		private:
			Operand *operand;
			Expression *index;
			
		};
		
		class Type : public Expression {
			
		public:
			Type(Lexical::Token *token);
			
			void addName(Identifier *name);
			std::vector<Identifier *> getNames();
			
			void addSlice(Slice *slice);
			
		private:
			std::vector<Identifier *> names;
			std::vector<Slice *> slices;
			
		};
		
		class Parameter : public Expression {
			
		public:
			Parameter(Lexical::Token *token);
			
			void setType(Type *type);
			Type *getType() const;
			
			void setName(Identifier *name);
			Identifier *getName();
			
		private:
			Type *type;
			Identifier *name;
			
		};
		
	}
	
	namespace Statements {
		
		class Statement {
			
		public:
			Statement(Lexical::Token *token);
			virtual ~Statement();
			
			Lexical::Token *getToken();
			
		private:
			Lexical::Token *token;
			
		};
		
		class Block : public Statement {
			
		public:
			Block(Lexical::Token *token);
			
			void addStatement(Statement *stat);
			
		private:
			std::vector<Statement *> statements;
			
		};
		
		class Expression : public Statement {
			
		public:
			Expression(Lexical::Token *token);
			
			void setExpression(Expressions::Expression *expr);
			
		private:
			Expressions::Expression *expression;
			
		};
		
		class Import : public Statement {
			
		public:
			Import(Lexical::Token *token);
			
			void setIdentifier(Expressions::Identifier *identifier);
			
		private:
			Expressions::Identifier *identifier;
			
		};
		
		class Return : public Statement {
			
		public:
			Return(Lexical::Token *token);
			
			void setExpression(Expressions::Expression *expr);
			
		private:
			Expressions::Expression *expression;
			
		};
		
		class Control : public Statement {
			
		public:
			Control(Lexical::Token *token);
			
		};
		
		class If : public Control {
			
		public:
			If(Lexical::Token *token);
			
			void setExpression(Expressions::Expression *expr);
			void setTrueStatement(Statement *statement);
			void setFalseStatement(Statement *statement);
			
		private:
			Expressions::Expression *expression;
			Statement *trueStatement, *falseStatement;
			
		};
		
		class Case : public Control {
			
		public:
			Case(Lexical::Token *token);
			
			void setExpression(Expressions::Expression *expr);
			void setBlock(Block *block);
			
		private:
			Expressions::Expression *expression;
			Block *block;
			
		};
		
		class Switch : public Control {
			
		public:
			Switch(Lexical::Token *token);
			
			void setExpression(Expressions::Expression *expr);
			void addCase(Case *statement);
			
		private:
			Expressions::Expression *expression;
			std::vector<Case *> cases;
			
		};
		
		class Declaration : public Statement {
			
		public:
			Declaration(Lexical::Token *token);
			
			void setExported(bool exported);
			bool getExported();
			
			void setName(Expressions::Identifier *name);
			Expressions::Identifier *getName();
			
		protected:
			bool exported;
			Expressions::Identifier *name;
			
		};
		
		class VariableDeclaration : public Declaration {
			
		public:
			VariableDeclaration(Lexical::Token *token);
			
			void setType(Expressions::Type *type);
			void setAssignment(Expressions::Expression *expr);
			
		private:
			Expressions::Type *type;
			Expressions::Expression *assignment;
			
		};
		
		class ConstantDeclaration : public Declaration {
			
		public:
			ConstantDeclaration(Lexical::Token *token);
			
			void setType(Expressions::Type *type);
			void setAssignment(Expressions::Expression *expr);
			
		private:
			Expressions::Type *type;
			Expressions::Expression *assignment;
			
		};
		
		class TypeDeclaration : public Declaration {
			
		public:
			TypeDeclaration(Lexical::Token *token);
			
			void setType(Expressions::Type *type);
			void setBlock(Block *block);
			
		private:
			Expressions::Type *type;
			Block *block;
			
		};
		
		class FunctionDeclaration : public Declaration {
			
		public:
			FunctionDeclaration(Lexical::Token *token);
			
			void setType(Expressions::Type *type);
			Expressions::Type *getType() const;
			
			void addParameter(Expressions::Parameter *param);
			std::vector<Expressions::Parameter *> getParameters();
			
			void setBlock(Block *block);
			
		private:
			Expressions::Type *type;
			std::vector<Expressions::Parameter *> parameters;
			Block *block;
			
		};
		
	}
	
	class Parser : public Unit {
		
	public:
		Parser();
		~Parser();
		
		void addError(Lexical::Token *token, std::string expected);
		
		void parseTokens(std::vector<Lexical::Token *> tokens);
		
		std::vector<Statements::Statement *> getStatements();
		
	private:
		void readTokens(std::vector<Lexical::Token *> tokens);
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
		Operators::Unary *readUnaryOperator(int *index);
		
		bool isBinaryOperator(int *index);
		Operators::Binary *getBinaryOperator(int *index);
		Operators::Binary *readBinaryOperator(int *index);
		
		// Tokens
		bool isToken(int *index, Lexical::Rule::Type type, std::string value);
		Lexical::Token *readToken(int *index, Lexical::Rule::Type type, std::string value);
		
		bool isIdentifierToken(int *index);
		Lexical::Token *readIdentifierToken(int *index);
		
		bool isOperatorToken(int *index, std::string op);
		Lexical::Token *readOperatorToken(int *index, std::string op);
		
		bool isDelimiterToken(int *index, std::string delimiter);
		Lexical::Token *readDelimiterToken(int *index, std::string delimiter);
		
		bool isKeywordToken(int *index, std::string keyword);
		Lexical::Token *readKeywordToken(int *index, std::string keyword);
		
		bool isLiteralToken(int *index);
		Lexical::Token *readLiteralToken(int *index);
		
	private:
		std::vector<Lexical::Token *> tokens;
		std::vector<Statements::Statement *> statements;
		
	};
	
}
