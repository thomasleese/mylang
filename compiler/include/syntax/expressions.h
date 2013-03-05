#pragma once

#include <iostream>

#include "syntax/operators.h"

namespace Syntax {
	
	namespace Expressions {
		
		class Expression {
			
		public:
			virtual std::string toString() const = 0;
			
		};
		
		class Unary : public Expression {
			
		public:
			void setOperator(Operators::Unary op);
			Operators::Unary getOperator();
			
			void setExpression(Expression *expr);
			Expression *getExpression();
			
			std::string toString() const;
			
		private:
			Operators::Unary op;
			Expression *expr;
			
		};
		
		class Operand : public Expression {
			
		};
		
		class Literal : public Operand {
			
		public:
			void setValue(std::string value);
			
			std::string toString() const;
			
		private:
			std::string value;
			
		};
		
		class Identifier : public Operand {
			
		public:
			void setValue(std::string value);
			
			std::string toString() const;
			
		private:
			std::string value;
			
		};
		
		class Selector : public Operand {
			
		public:
			void setOperand(Operand *expr);
			void setIdentifier(Identifier *identifier);
			
			std::string toString() const;
			
		private:
			Operand *operand;
			Identifier *identifier;
			
		};
		
		class Call : public Operand {
			
		public:
			void setOperand(Operand *expr);
			
			std::string toString() const;
			
		private:
			Operand *operand;
			
		};
		
		class Slice : public Operand {
			
		public:
			void setOperand(Operand *expr);
			void setIndex(Expression *expr);
			
			std::string toString() const;
			
		private:
			Operand *operand;
			Expression *index;
			
		};
		
	}
	
}
