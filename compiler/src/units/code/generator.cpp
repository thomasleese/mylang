#include <fstream>
#include <sys/stat.h>

#include "units/code.h"

using namespace Code;

Generator::Generator(std::string moduleName) {
	this->moduleName = moduleName;
	this->module = new llvm::Module(moduleName, llvm::getGlobalContext());
}

Generator::~Generator() {
	delete this->module;
}

void Generator::addError(Lexical::Token *token, std::string msg) {
	std::string filename = token->getFilename();
	int line = token->getLineNumber();
	int col = token->getColumn();
	std::string tokenValue = token->getValue();
	
	this->addMessage(Message("Error", filename, line, col, tokenValue, msg));
}

void Generator::parseAST(AST::Blocks::Module *block) {
	DeclarationPass pass1(this);
	pass1.parseAST(block);
	
	DefinitionPass pass2(this);
	pass2.parseAST(block);
}

llvm::Module *Generator::getModule() const {
	return this->module;
}

void Generator::dump() const {
	this->module->dump();
}

void Generator::write(std::string buildDir) const {
	mkdir(buildDir.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	
	std::string filename = buildDir + "/" + this->moduleName + ".bc";
	
	std::string err;
	llvm::raw_fd_ostream stream(filename.c_str(), err, llvm::raw_fd_ostream::F_Binary);
	
	if (err.empty()) {
		llvm::WriteBitcodeToFile(this->module, stream);
		stream.close();
	} else {
		// sort this out...
	}
}
