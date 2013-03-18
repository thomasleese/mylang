#pragma once

#include <iostream>

namespace Syntax {
	
	namespace Operators {
		
		enum Unary {
			None,
			Add,
			Subtract,
			Not,
			Increment,
			Decrement,
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
			
			std::string toString() const;
			
		private:
			Type type;
			int precedence;
			
		};
		
	}
	
}
