#include <dirent.h>

#include "units/lexical.h"
#include "units/ast.h"
#include "units/code.h"
#include "compiler.h"

void Compiler::compileProject(std::string dir) {
	std::string srcDir = dir + "/src";
	std::string buildDir = dir + "/build";
	
	DIR *dp = opendir(srcDir.c_str());
	if (dp != NULL) {
		dirent *ep;
		
		while ((ep = readdir(dp)) != NULL) {
			std::string name = ep->d_name;
			if (name != "." && name != "..") {
				std::cout << "Building " << srcDir << "/" << name << std::endl;
				compileModule(name, srcDir + "/" + name, buildDir);
			}
		}
		
		closedir(dp);
	}
}

void Compiler::compileModule(std::string name, std::string srcDir, std::string buildDir) {
	Lexical::Analyser analyser;
	
	DIR *dp = opendir(srcDir.c_str());
	if (dp != NULL) {
		dirent *ep;
		
		while ((ep = readdir(dp)) != NULL) {
			std::string filename = ep->d_name;
			if (filename != "." && filename != "..") {
#ifdef DEBUG
				std::cout << "Analysing " << srcDir << "/" << filename << std::endl;
#endif
				
				analyser.parseFile(srcDir + "/" + filename);
			}
		}
		
		closedir(dp);
	}
	
	if (analyser.hasMessages()) {
		analyser.printMessages();
		return;
	}

#ifdef DEBUG
	std::cout << "Parsing " << srcDir << std::endl;
#endif
	
	AST::Parser parser;
	parser.parseTokens(analyser.getTokens());
	
	if (parser.hasMessages()) {
		parser.printMessages();
		return;
	}
	
#ifdef DEBUG
	std::cout << "Generating " << buildDir << "/" << name << ".bc" << std::endl;
#endif
	
	Code::Generator gen(name);
	gen.parseAST(parser.getBlock());
	
	if (gen.hasMessages()) {
		gen.printMessages();
		return;
	}
	
	gen.write(buildDir);
	
	
#ifdef DEBUG
	gen.dump();
#endif
}
