#pragma once

#include <iostream>

class Compiler {
    
public:
	void compileProject(std::string dir);
	void compileModule(std::string name, std::string srcDir, std::string buildDir);
    
};
