#pragma once

#include <iostream>

namespace Lex {
    
	class Rule;

	class Token {
		
	public:
		Token(Rule *rule, std::string value);
		
		Rule *getRule() const;
		std::string getValue() const;
		
		void setLineNumber(int no);
		int getLineNumber();
		
		std::string toString() const;
		
	private:
		Rule *rule;
		std::string value;
		int lineNumber;
		
	};
    
}
