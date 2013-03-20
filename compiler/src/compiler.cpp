#include <dirent.h>

#include "lex/analyser.h"
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
	
	analyser.dump();
	std::cout << std::endl;
	
	Syntax::Parser parser;
	
	try {
		parser.parseTokens(analyser.getTokens());
	} catch (Syntax::ParserError err) {
		err.print();
	}
	
	parser.dump();
	std::cout << std::endl;
	
	Code::Generator gen(name);
	
	try {
		gen.parseAST(parser.getStatements());
	} catch (const char *s) {
		std::cout << s << std::endl;
	}
	
	gen.dump();
}
