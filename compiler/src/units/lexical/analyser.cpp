#include <fstream>

#include <boost/regex/icu.hpp>

#include "units/lexical.h"

using namespace Lexical;

Analyser::Analyser() {
	loadRules();
}

Analyser::~Analyser() {
	for (Token *token : this->tokens) {
		delete token;
	}

	for (Rule *rule : this->rules) {
		delete rule;
	}
}

void Analyser::parseFile(std::string filename) {
	readFile(filename);
	tokeniseBuffer(filename);
}

std::vector<Token *> Analyser::getTokens() {
	return this->tokens;
}

void Analyser::loadRules() {
	this->rules.push_back(new Rule(Rule::Ignore, "//[^\n\r]+"));
	this->rules.push_back(new Rule(Rule::Ignore, "[\n\r\t ]"));
	
	this->rules.push_back(new Rule(Rule::Keyword, "import|type|struct|interface|var|if|else|for|switch|case|fallthrough|break|continue|const|exported|default|func|return"));
	
	this->rules.push_back(new Rule(Rule::BooleanLiteral, "true|false"));
	this->rules.push_back(new Rule(Rule::IntegerLiteral, "[0-9]+"));
	this->rules.push_back(new Rule(Rule::FloatLiteral, "[0-9]+\\.[0-9]+"));
	this->rules.push_back(new Rule(Rule::StringLiteral, "\"(\\.|[^\"])*\""));
	this->rules.push_back(new Rule(Rule::ComplexLiteral, "([0-9]+)|([0-9]+\\.[0-9]+)i([0-9]+)|([0-9]+\\.[0-9]+)"));
	
	this->rules.push_back(new Rule(Rule::Identifier, "[[:L*:]_][[:L*:]|[:Nd:]|_]*"));
	
	this->rules.push_back(new Rule(Rule::Operator, "\\|\\||&&|\\-\\-|\\+\\+|&\\^|>>|<<|>=|<=|!=|==|\\+|\\-|\\*|/|%|!|&|\\||\\^|>|<|="));
	
	this->rules.push_back(new Rule(Rule::Delimiter, ";|\\.|,|\\(|\\)|\\[|\\]|\\{|\\}"));
}

void Analyser::readFile(std::string filename) {
	this->buffer = "";
	
	std::ifstream in(filename.c_str());
	while (in.good()) {
		std::string line;
		std::getline(in, line);
		
		this->buffer += line;
		this->buffer += "\n";
	}
}

void Analyser::tokeniseBuffer(std::string filename) {
	int pos = 0;
	int end = buffer.length();

	boost::smatch matcher;

	while (pos < end) {
		std::string substr = this->buffer.substr(pos, end - pos);
		
		int line = getLineFromIndex(pos);
		int col = 0;
		bool found = false;
		
		for (Rule *rule : this->rules) {
			if (boost::u32regex_search(substr, matcher, rule->getRegex())) {
				std::string value = matcher[0].str();
				if (substr.substr(0, value.length()) == value) {
					Token *token = new Token(rule, value);
					token->setLineNumber(getLineFromIndex(pos));
					token->setFilename(filename);
					
					this->tokens.push_back(token);
					pos += value.length();
					found = true;
					break;
				}
			}
		}
		
		if (!found) {
			std::string token = "'" + substr.substr(0, 1) + "'";
			std::string msg = "Unknown character, skipping...";
			this->addWarning(filename, line, col, token, msg);
			pos++;
		}
	}
}

int Analyser::getLineFromIndex(int index) {
	int lineno = 1;

	for (int i = 0; i < index; i++) {
		if (this->buffer[i] == '\n') {
			lineno++;
		}
	}

	return lineno;
}
