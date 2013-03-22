#pragma once

#include <iostream>

namespace Syntax {
	
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
	
}
