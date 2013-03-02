#include "compiler.h"

int main(int argc, char *argv[]) {
    Compiler c;
    
    for (int i = 1; i < argc; i++) {
        c.compileProject(argv[i]);
    }
    
    return 0;
}
