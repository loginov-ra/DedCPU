
#include "Disassembler.h"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("At least source code file should be provided\n");
    }

    const char* code_filename = argv[1];
    const char* asm_code_filename = (argc > 2) ? argv[2] : "__src.rl";

    Disassembler disasm(code_filename, asm_code_filename);
    disasm.disassemble();
    return 0; 
}
