#pragma once

#include <iostream>
#include <vector>

#define FORMAT_NONE			"\033[0m"
#define FORMAT_BOLD			"\033[1m"
#define FORMAT_UNDERLINE	"\033[4m"

#define FORMAT_BLUE			"\033[94m"
#define FORMAT_YELLOW		"\033[93m"
#define FORMAT_RED			"\033[91m"

class Message {
	
public:
	Message(std::string id, std::string filename, int line, int col, std::string token, std::string msg);
	~Message();
	
	void print() const;
	
private:
	std::string id;
	std::string filename;
	int line;
	int col;
	std::string token;
	std::string message;
	
};

class Unit {
	
public:
	Unit();
	~Unit();
	
	void addMessage(Message msg);
	
	void addMessage(std::string id, std::string filename, int line, int col, std::string token, std::string msg);
	void addWarning(std::string filename, int line, int col, std::string token, std::string msg);
	void addError(std::string filename, int line, int col, std::string token, std::string msg);
	
	std::vector<Message> getMessages();
	bool hasMessages();
	void printMessages();
	
private:
	std::vector<Message> messages;
	
};
