#include "compiler.h"

int main(int argc, char *argv[]) {
	Compiler c;
	
	if (argc == 1) {
		c.compileProject(".");
	} else {
		for (int i = 1; i < argc; i++) {
			c.compileProject(argv[i]);
		}
	}
	
	return 0;
}
