#include <dirent.h>

#include "units/lexical.h"
#include "units/ast.h"
#include "units/code.h"
#include "compiler.h"

void Compiler::compileProject(std::string dir) {
	std::cout << "Building " << dir << std::endl;
	
	std::string srcDir = dir + "/src";
	std::string buildDir = dir + "/build";
	
	for (std::string moduleName : this->searchDirectory(srcDir)) {
		this->compileModule(moduleName, srcDir + "/" + moduleName, buildDir);
	}
}

void Compiler::compileModule(std::string name, std::string srcDir, std::string buildDir) {
	std::cout << "Compiling " << srcDir << std::endl;
	
	Lexical::Analyser analyser;
	for (std::string filename : this->searchDirectory(srcDir)) {
		analyser.parseFile(srcDir + "/" + filename);
	}
	
	if (analyser.hasMessages()) {
		analyser.printMessages();
		return;
	}
	
	AST::Parser parser;
	parser.parseTokens(analyser.getTokens());
	
	if (parser.hasMessages()) {
		parser.printMessages();
		return;
	}
	
	Code::Generator gen(name);
	gen.parseAST(parser.getBlock());
	
	if (gen.hasMessages()) {
		gen.printMessages();
		return;
	}
	
#ifdef DEBUG
	gen.dump();
#endif
	
	gen.write(buildDir);
}

std::vector<std::string> Compiler::searchDirectory(std::string dir) {
	std::vector<std::string> filenames;
	
	DIR *dp = opendir(dir.c_str());
	if (dp != NULL) {
		dirent *ep;
		
		while ((ep = readdir(dp)) != NULL) {
			std::string name = ep->d_name;
			if (name != "." && name != "..") {
				filenames.push_back(name);
			}
		}
		
		closedir(dp);
	}
	
	return filenames;
}
