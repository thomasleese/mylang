#pragma once

#include <iostream>

namespace Lex {
    
	class Rule;

	class Token {
		
	public:
		Token(Rule *rule, std::string value);
		
		Rule *getRule() const;
		std::string getValue() const;
		
		void setFilename(std::string filename);
		std::string getFilename();
		
		void setLineNumber(int no);
		int getLineNumber();
		
		std::string toString() const;
		
	private:
		Rule *rule;
		std::string value;
		
		std::string filename;
		int lineNumber;
		
	};
    
}
