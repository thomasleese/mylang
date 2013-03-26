#include "unit.h"

Message::Message(std::string id, std::string filename, int line, int col, std::string token, std::string msg) {
	this->id = id;
	this->filename = filename;
	this->line = line;
	this->col = col;
	this->token = token;
	this->message = msg;
}

Message::~Message() {
	
}

void Message::print() const {
	std::cout << this->id << ": " << FORMAT_BOLD << this->filename << ":" << this->line << FORMAT_NONE << std::endl;
	std::cout << "\t" << "Col " << FORMAT_BOLD << this->col << FORMAT_NONE << ": " << this->token << std::endl;
	std::cout << "\t" << "Msg: " << this->message << std::endl;
	std::cout << std::endl;
}

Unit::Unit() {
	
}

Unit::~Unit() {
	
}

void Unit::addMessage(Message msg) {
	this->messages.push_back(msg);
}

void Unit::addMessage(std::string id, std::string filename, int line, int col, std::string token, std::string msg) {
	this->addMessage(Message(id, filename, line, col, token, msg));
}

void Unit::addWarning(std::string filename, int line, int col, std::string token, std::string msg) {
	this->addMessage("Warning", filename, line, col, token, msg);
}

void Unit::addError(std::string filename, int line, int col, std::string token, std::string msg) {
	this->addMessage("Error", filename, line, col, token, msg);
}

std::vector<Message> Unit::getMessages() {
	return this->messages;
}

bool Unit::hasMessages() {
	return !this->messages.empty();
}

void Unit::printMessages() {
	for (Message msg : this->messages) {
		msg.print();
	}
}
