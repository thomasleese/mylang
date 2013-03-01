#pragma once

#include <iostream>

#include <boost/regex/icu.hpp>

namespace Lex {
	
	class Rule {
		
	public:
		enum Type {
			
			Identifier,
			Operator,
			Delimiter,
			Keyword,
			Comment,
			
			IntegerLiteral,
			FloatLiteral,
			ImaginaryLiteral,
			BooleanLiteral,
			StringLiteral,
			
		};
		
		Rule(Type type, std::string regex);
		Rule(Type type, boost::u32regex regex);
		
		Type getType() const;
		boost::u32regex getRegex() const;
		
		std::string toString() const;
		
	private:
		Type type;
		boost::u32regex regex;
		
	};
	
}
