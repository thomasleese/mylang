#pragma once

#include <iostream>

namespace Syntax {
	
	namespace Expressions {
		class Expression;
	}
	
	namespace Statements {
		
		class Statement {
			
		public:
			virtual std::string toString() const = 0;
			
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
		
	}
	
}
