#include <dirent.h>

#include "units/lex.h"
#include "syntax/parser.h"
#include "code/generator.h"
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
	Lex::Analyser analyser;
	
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
	
	Syntax::Parser parser;
	parser.parseTokens(analyser.getTokens());
	
	if (parser.hasMessages()) {
		parser.printMessages();
		return;
	}
	
	Code::Generator gen(name);
	
	try {
		gen.parseAST(parser.getStatements());
	} catch (const char *s) {
		std::cout << s << std::endl;
	}
	
	gen.dump();
}
