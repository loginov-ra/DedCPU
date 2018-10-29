
#include "CPU.h"

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("At least source code file should be provided\n");
    }

    const char* code_filename = argv[1];

    CPU cpu(code_filename);
    cpu.execute();
    return 0; 
}
