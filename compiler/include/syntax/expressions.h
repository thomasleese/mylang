#pragma once

#include <iostream>
#include <vector>

#include "syntax/operators.h"

namespace Syntax {
	
	namespace Lists {
		class Expressions;
	}
	
	namespace Expressions {
		
		class Expression {
			
		};
		
		class Unary : public Expression {
			
		public:
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
			
		};
		
		class Expr : public Operand {
			
		public:
			void setExpression(Expression *expr);
			
		private:
			Expression *expr;
			
		};
		
		class Literal : public Operand {
			
		public:
			void setValue(std::string value);
			
		private:
			std::string value;
			
		};
		
		class Identifier : public Operand {
			
		public:
			void setValue(std::string value);
			std::string getValue() const;
			
		private:
			std::string value;
			
		};
		
		class Selector : public Operand {
			
		public:
			void setOperand(Operand *expr);
			void setIdentifier(Identifier *identifier);
			
		private:
			Operand *operand;
			Identifier *identifier;
			
		};
		
		class Call : public Operand {
			
		public:
			void setOperand(Operand *expr);
			void addArgument(Expression *arg);
			
		private:
			Operand *operand;
			std::vector<Expression *> arguments;
			
		};
		
		class Slice : public Operand {
			
		public:
			void setOperand(Operand *expr);
			void setIndex(Expression *expr);
			
		private:
			Operand *operand;
			Expression *index;
			
		};
		
		class Type : public Expression {
			
		public:
			void addName(Identifier *name);
			std::vector<Identifier *> getNames();
			
			void addSlice(Slice *slice);
			
		private:
			std::vector<Identifier *> names;
			std::vector<Slice *> slices;
			
		};
		
		class Parameter : public Expression {
			
		public:
			void setType(Type *type);
			Type *getType() const;
			
			void setIdentifier(Identifier *identifier);
			
		private:
			Type *type;
			Identifier *identifier;
			
		};
		
	}
	
}
