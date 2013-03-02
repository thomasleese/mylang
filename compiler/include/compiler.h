#pragma once

#include <iostream>

class Compiler {
    
public:
	void compileProject(std::string dir);
	void compilePackage(std::string name, std::string dir);
    
};
