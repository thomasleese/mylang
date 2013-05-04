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
			
			Lexical::Token *getToken() const;
			
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
			
		};
		
		class IntegerLiteral : public Literal {
			
		public:
			IntegerLiteral(Lexical::Token *token);
			
			void setValue(int val);
			int getValue() const;
			
		private:
			int value;
			
		};
		
		class StringLiteral : public Literal {
			
		public:
			StringLiteral(Lexical::Token *token);
			
			void setValue(std::string val);
			std::string getValue() const;
			
		private:
			std::string value;
			
		};
		
		class QualifiedIdentifier;
		
		class Identifier : public Operand {
			
		public:
			Identifier(Lexical::Token *token);
			
			void setValue(std::string value);
			std::string getValue() const;
			
			QualifiedIdentifier *toQualifiedIdentifier();
			
		private:
			std::string value;
			
		};
		
		class QualifiedIdentifier : public Expression {
			
		public:
			QualifiedIdentifier(Lexical::Token *token);
			
			void setModule(Identifier *identifier);
			Identifier *getModule();
			
			void setName(Identifier *identifier);
			Identifier *getName();
			
		private:
			Identifier *module, *name;
			
		};
		
		class Selector : public Operand {
			
		public:
			Selector(Lexical::Token *token);
			
			void setOperand(Operand *expr);
			Operand *getOperand() const;
			
			void setIdentifier(Identifier *identifier);
			Identifier *getIdentifier() const;
			
		private:
			Operand *operand;
			Identifier *identifier;
			
		};
		
		class Call : public Operand {
			
		public:
			Call(Lexical::Token *token);
			
			void setOperand(Operand *expr);
			Operand *getOperand() const;
			
			void addArgument(Expression *arg);
			std::vector<Expression *> getArguments() const;
			
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
			
			void setIsStruct(bool s);
			bool getIsStruct() const;
			
			void setName(QualifiedIdentifier *name);
			QualifiedIdentifier *getName();
			
			void addSlice(Slice *slice);
			
		private:
			QualifiedIdentifier *name;
			std::vector<Slice *> slices;
			bool isStruct;
			
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
	
	namespace Blocks {
		
		class Block;
		class Generic;
		class Type;
		
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
		
		class Generic : public Statement {
			
		public:
			Generic(Lexical::Token *token);
			
		};
		
		class Expression : public Generic {
			
		public:
			Expression(Lexical::Token *token);
			
			void setExpression(Expressions::Expression *expr);
			Expressions::Expression *getExpression() const;
			
		private:
			Expressions::Expression *expression;
			
		};
		
		class Import : public Statement {
			
		public:
			Import(Lexical::Token *token);
			
			void setName(Expressions::Identifier *identifier);
			Expressions::Identifier *getName() const;
			
		private:
			Expressions::Identifier *name;
			
		};
		
		class Return : public Generic {
			
		public:
			Return(Lexical::Token *token);
			
			void setExpression(Expressions::Expression *expr);
			Expressions::Expression *getExpression() const;
			
		private:
			Expressions::Expression *expression;
			
		};
		
		class Control : public Generic {
			
		public:
			Control(Lexical::Token *token);
			
		};
		
		class If : public Control {
			
		public:
			If(Lexical::Token *token);
			
			void setExpression(Expressions::Expression *expr);
			Expressions::Expression *getExpression() const;
			
			void setTrueBlock(Blocks::Generic *block);
			Blocks::Generic *getTrueBlock() const;
			
			void setFalseBlock(Blocks::Generic *block);
			Blocks::Generic *getFalseBlock() const;
			
			void setFalseIf(If *statement);
			If *getFalseIf() const;
			
		private:
			Expressions::Expression *expression;
			Blocks::Generic *trueBlock, *falseBlock;
			If *falseIf;
			
		};
		
		class Case : public Control {
			
		public:
			Case(Lexical::Token *token);
			
			void setExpression(Expressions::Expression *expr);
			void setBlock(Blocks::Generic *block);
			
		private:
			Expressions::Expression *expression;
			Blocks::Generic *block;
			
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
		
		class VariableDeclaration : public Declaration, public Generic {
			
		public:
			VariableDeclaration(Lexical::Token *token);
			
			void setIsConstant(bool constant);
			bool getIsConstant();
			
			void setType(Expressions::Type *type);
			Expressions::Type *getType() const;
			
			void setAssignment(Expressions::Expression *expr);
			Expressions::Expression *getAssignment() const;
			
		private:
			bool isConstant;
			Expressions::Type *type;
			Expressions::Expression *assignment;
			
		};
		
		class TypeDeclaration : public Declaration {
			
		public:
			TypeDeclaration(Lexical::Token *token);
			
			void setType(Expressions::Type *type);
			Expressions::Type *getType();
			
			void setBlock(Blocks::Type *block);
			Blocks::Type *getBlock() const;
			
		private:
			Expressions::Type *type;
			Blocks::Type *block;
			
		};
		
		class FunctionDeclaration : public Declaration {
			
		public:
			FunctionDeclaration(Lexical::Token *token);
			
			void setType(Expressions::Type *type);
			Expressions::Type *getType() const;
			
			void insertParameter(int index, Expressions::Parameter *param);
			void addParameter(Expressions::Parameter *param);
			std::vector<Expressions::Parameter *> getParameters();
			
			void setBlock(Blocks::Generic *block);
			Blocks::Generic *getBlock() const;
			
		private:
			Expressions::Type *type;
			std::vector<Expressions::Parameter *> parameters;
			Blocks::Generic *block;
			
		};
		
	}
	
	namespace Blocks {
		
		class Block {
			
		public:
			Block(Lexical::Token *token);
			
			Lexical::Token *getToken() const;
			
		private:
			Lexical::Token *token;
			
		};
		
		class Generic : public Block {
			
		public:
			Generic(Lexical::Token *token);
			
			void addGenericStatement(Statements::Generic *stat);
			std::vector<Statements::Generic *> getGenericStatements();
			
		private:
			std::vector<Statements::Generic *> generics;
			
		};
		
		class Type : public Block {
			
		public:
			Type(Lexical::Token *token);
			
			void addVariableDeclarationStatement(Statements::VariableDeclaration *decl);
			std::vector<Statements::VariableDeclaration *> getVariableDeclarationStatements();
			
			void addFunctionDeclarationStatement(Statements::FunctionDeclaration *decl);
			std::vector<Statements::FunctionDeclaration *> getFunctionDeclarationStatements();
			
		private:
			std::vector<Statements::VariableDeclaration *> variables;
			std::vector<Statements::FunctionDeclaration *> functions;
			
		};
		
		class Module : public Type {
			
		public:
			Module(Lexical::Token *token);
			
			void addImportStatement(Statements::Import *import);
			std::vector<Statements::Import *> getImportStatements();
			
			void addTypeDeclarationStatement(Statements::TypeDeclaration *decl);
			std::vector<Statements::TypeDeclaration *> getTypeDeclarationStatements();
			
		private:
			std::vector<Statements::Import *> imports;
			std::vector<Statements::TypeDeclaration *> types;
			
		};
		
	}
	
	class Parser : public Unit {
		
	public:
		Parser();
		~Parser();
		
		void addError(Lexical::Token *token, std::string expected);
		
		void parseTokens(std::vector<Lexical::Token *> tokens);
		
		Blocks::Module *getBlock();
		
	private:
		void readTokens(std::vector<Lexical::Token *> tokens);
		void generateAST();
		
		// Statements
		bool isStatement(int *index);
		Statements::Statement *readStatement(int *index);
		
		bool isGenericStatement(int *index);
		Statements::Generic *readGenericStatement(int *index);
		
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
		
		bool isIntegerLiteralExpression(int *index);
		Expressions::IntegerLiteral *readIntegerLiteralExpression(int *index);
		
		bool isStringLiteralExpression(int *index);
		Expressions::StringLiteral *readStringLiteralExpression(int *index);
		
		bool isIdentifierExpression(int *index);
		Expressions::Identifier *readIdentifierExpression(int *index);
		
		bool isQualifiedIdentifierExpression(int *index);
		Expressions::QualifiedIdentifier *readQualifiedIdentifierExpression(int *index);
		
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
		
		// Blocks
		bool isBlock(int *index);
		Blocks::Block *readBlock(int *index);
		
		bool isGenericBlock(int *index);
		Blocks::Generic *readGenericBlock(int *index);
		
		bool isTypeBlock(int *index);
		Blocks::Type *readTypeBlock(int *index, bool acceptVars);
		
		bool isModuleBlock(int *index);
		Blocks::Module *readModuleBlock(int *index);
		
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
		
		bool isIntegerLiteralToken(int *index);
		Lexical::Token *readIntegerLiteralToken(int *index);
		
		bool isStringLiteralToken(int *index);
		Lexical::Token *readStringLiteralToken(int *index);
		
	private:
		std::vector<Lexical::Token *> tokens;
		Blocks::Module *block;
		
	};
	
}
