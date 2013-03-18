#pragma once

#include <iostream>
#include <vector>

namespace Syntax {
	
	namespace Expressions {
		class Expression;
	}
	
	namespace Lists {
		class ParameterExpressions;
	}
	
	namespace Statements {
		
		class Statement {
			
		public:
			virtual std::string toString() const = 0;
			
		};
		
		class Block : public Statement {
			
		public:
			void addStatement(Statement *stat);
			
			std::string toString() const;
			
		private:
			std::vector<Statement *> statements;
			
		};
		
		class Expression : public Statement {
			
		public:
			void setExpression(Expressions::Expression *expr);
			
			std::string toString() const;
			
		private:
			Expressions::Expression *expression;
			
		};
		
		class Import : public Statement {
			
		public:
			void setIdentifier(Expressions::Identifier *identifier);
			
			std::string toString() const;
			
		private:
			Expressions::Identifier *identifier;
			
		};
		
		class Return : public Statement {
			
		public:
			void setExpression(Expressions::Expression *expr);
			
			std::string toString() const;
			
		private:
			Expressions::Expression *expression;
			
		};
		
		class Control : public Statement {
			
		};
		
		class If : public Control {
			
		public:
			If();
			
			void setExpression(Expressions::Expression *expr);
			void setTrueStatement(Statement *statement);
			void setFalseStatement(Statement *statement);
			
			std::string toString() const;
			
		private:
			Expressions::Expression *expression;
			Statement *trueStatement, *falseStatement;
			
		};
		
		class Declaration : public Statement {
			
		};
		
		class FunctionDeclaration : public Declaration {
			
		public:
			void setType(Expressions::Type *type);
			void setIdentifier(Expressions::Identifier *identifier);
			void addParameter(Expressions::Parameter *param);
			void setBlock(Block *block);
			
			std::string toString() const;
			
		private:
			Expressions::Type *type;
			Expressions::Identifier *identifier;
			std::vector<Expressions::Parameter *> parameters;
			Block *block;
			
		};
		
	}
	
}
