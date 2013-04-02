#include <dirent.h>

#include "units/lexical.h"
#include "units/ast.h"
#include "units/code.h"
#include "compiler.h"

void Compiler::compileProject(std::string dir) {
	std::string srcDir = dir + "/src";
	
	DIR *dp = opendir(srcDir.c_str());
	if (dp != NULL) {
		dirent *ep;
		
		while ((ep = readdir(dp)) != NULL) {
			std::string name = ep->d_name;
			if (name != "." && name != "..") {
				compilePackage(name, srcDir + "/" + name);
			}
		}
		
		closedir(dp);
	}
}

void Compiler::compilePackage(std::string name, std::string dir) {
	Lexical::Analyser analyser;
	
	DIR *dp = opendir(dir.c_str());
	if (dp != NULL) {
		dirent *ep;
		
		while ((ep = readdir(dp)) != NULL) {
			std::string filename = ep->d_name;
			if (filename != "." && filename != "..") {
				analyser.parseFile(dir + "/" + filename);
			}
		}
		
		closedir(dp);
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
		//return;
	}
	
	gen.dump();
}
