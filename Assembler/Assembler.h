
#include <cstdio>
#include <cassert>
#include <cstring>
#include <cstdlib>
#include "../CommandCodes.h"

#define ASSERT(COND, MSG)                                       \
    if(!(COND))                                                 \
    {                                                           \
        fprintf(stderr, "Error happened: %s\n", MSG);           \
        fprintf(stderr, "Function: %s\n", __PRETTY_FUNCTION__); \
        fprintf(stderr, "Line: %d\n", __LINE__);                \
        assert(COND);                                           \
    }

#define PROC_ARG                                          \
    if (!readArgument(argbuf, &argval))                   \
    {                                                     \
        printf("Read argument of unsupported format!\n"); \
        return 1;                                         \
    }

const size_t MAX_LINE_LENGTH = 256;
const size_t MAX_ARG_LENGTH  = 256;

class Assembler
{
private:
    FILE* source_code_file_;
    FILE* output_file_;
	
	bool readArgument(char* buf, int* argval)
	{
        fscanf(source_code_file_, "%s", buf);
		char* end = 0;
		*argval = strtol(buf, &end, 10);
        printf("Checking argument: %s\n", buf);    

		return *end == 0;
	}
	
	void writeInt(int number)
	{
		fwrite(&number, sizeof(int), 1, output_file_);
	}
    
    void writeChar(char c)
    {
        fwrite(&c, sizeof(char), 1, output_file_);
    }

public:
    Assembler(FILE* file, const char* output_filename = "a.out"):
        source_code_file_(file),
        output_file_(fopen(output_filename, "wb"))
    {
        ASSERT(file,          "Provided zero file");
        ASSERT(!ferror(file), "Provided invalid file");

        ASSERT(output_file_ && !ferror(output_file_), "Cannot open output file");
    }

    Assembler(const char* filename, const char* output_filename):
		Assembler(fopen(filename, "r"), output_filename)
    {}
	
    int makeAssembling()
    {
        while (!feof(source_code_file_))
        {
            char cmd_name[MAX_LINE_LENGTH] = "";
            fscanf(source_code_file_, "%s", cmd_name);
     		
            if (strlen(cmd_name) == 0)
                continue;

			int argval = 0;
			char argbuf[MAX_ARG_LENGTH] = "";
       
            if (false);
            else if (!strcmp(cmd_name, "push"))
            {
     			PROC_ARG
                writeChar(PUSH);
                writeInt(argval);
            }
            else if (!strcmp(cmd_name, "pop"))
            {
                writeChar(POP);
            }
            else if (!strcmp(cmd_name, "add"))
            {
                writeChar(ADD);
            }
            else if (!strcmp(cmd_name, "mul"))
            {
                writeChar(MUL);
            }
            else if (!strcmp(cmd_name, "in"))
            {
                writeChar(IN);
            }
            else if (!strcmp(cmd_name, "out"))
            {
                writeChar(OUT);
            }
            else if (!strcmp(cmd_name, "end"))
            {
                writeChar(END);
            }
            else
            {
                printf("Unknown command: %s\n", cmd_name);
                return 1;
            }
        }

        return 0;
    }

    ~Assembler()
    {
        fclose(output_file_);
        fclose(source_code_file_);
    }
};
