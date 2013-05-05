#pragma once

#include <iostream>
#include <vector>

class Compiler {
    
public:
	void compileProject(std::string dir);
	void compileModule(std::string name, std::string srcDir, std::string buildDir);
	
private:
	std::vector<std::string> searchDirectory(std::string dir);
	
};
