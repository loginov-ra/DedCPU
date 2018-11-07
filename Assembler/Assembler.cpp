
#include "Assembler.h"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("At least source code file should be provided\n");
    }

    const char* src_filename    = argv[1];
    const char* output_filename = (argc > 2) ? argv[2] : "a.out";

    Assembler assembler(src_filename, output_filename);
    
    if (assembler.makeAssembling())
    {
        printf("Errors occured, assembler returned non-zero exit code\n");
    }

    assembler.dump();

    return 0; 
}
