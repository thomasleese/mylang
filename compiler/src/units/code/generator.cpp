#include <fstream>
#include <sys/stat.h>

#include "units/code.h"

using namespace Code;

extern "C" {
	void _print_integer(int i) {
		std::cout << i << std::endl;
	}
}

Generator::Generator(std::string moduleName) {
	llvm::InitializeNativeTarget();
	
	this->moduleName = moduleName;
	this->module = new llvm::Module(moduleName, llvm::getGlobalContext());
	
	this->engine = llvm::EngineBuilder(this->module).create();
	
	this->fpm = new llvm::FunctionPassManager(this->module);
	this->fpm->add(new llvm::DataLayout(*engine->getDataLayout()));
	this->fpm->add(llvm::createBasicAliasAnalysisPass());
	this->fpm->add(llvm::createInstructionCombiningPass());
	this->fpm->add(llvm::createUnreachableBlockEliminationPass());
	this->fpm->add(llvm::createReassociatePass());
	this->fpm->add(llvm::createGVNPass());
	this->fpm->add(llvm::createCFGSimplificationPass());
	this->fpm->doInitialization();
	
	// add the _print_integer function
	std::vector<llvm::Type *> args;
	args.push_back(llvm::Type::getInt64Ty(llvm::getGlobalContext()));
	
	llvm::FunctionType* type = llvm::FunctionType::get(llvm::Type::getVoidTy(llvm::getGlobalContext()), args, false);
	llvm::Function *func = llvm::Function::Create(type, llvm::Function::ExternalLinkage, "_print_integer", this->module);
	func->setCallingConv(llvm::CallingConv::C);
}

Generator::~Generator() {
	delete this->module;
}

void Generator::addError(Lexical::Token *token, std::string msg) {
	std::string filename = "unknown";
	int line = 0;
	int col = 0;
	std::string tokenValue = "unknown";
	
	if (token != NULL) {
		filename = token->getFilename();
		line = token->getLineNumber();
		col = token->getColumn();
		tokenValue = token->getValue();
	}
	
	this->addMessage(Message("Error", filename, line, col, tokenValue, msg));
}

void Generator::parseAST(AST::Blocks::Module *block) {
	DeclarationPass pass1(this);
	pass1.parseAST(block);
	
	DefinitionPass pass2(this);
	pass2.parseAST(block);
}

std::string Generator::getModuleName() const {
	return this->moduleName;
}

llvm::Module *Generator::getModule() const {
	return this->module;
}

llvm::FunctionPassManager *Generator::getFunctionPassManager() const {
	return this->fpm;
}

void Generator::dump() const {
	std::cout << "Dumping module" << std::endl;
	this->module->dump();
}

void Generator::write(std::string buildDir) const {
	mkdir(buildDir.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
	
	std::string filename = buildDir + "/" + this->moduleName + ".bc";
	
#ifdef DEBUG
	std::cout << "Writing out module to: " << filename << std::endl;
#endif
	
	std::string err;
	llvm::raw_fd_ostream stream(filename.c_str(), err, llvm::raw_fd_ostream::F_Binary);
	
	if (err.empty()) {
		llvm::WriteBitcodeToFile(this->module, stream);
		stream.close();
	} else {
		// sort this out...
	}
}
