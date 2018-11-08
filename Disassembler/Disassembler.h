
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include "../CommonInfo.h"

#define ASSERT(COND, MSG)                                       \
    if(!(COND))                                                 \
    {                                                           \
        fprintf(stderr, "Error happened: %s\n", MSG);           \
        fprintf(stderr, "Function: %s\n", __PRETTY_FUNCTION__); \
        fprintf(stderr, "Line: %d\n", __LINE__);                \
        assert(COND);                                           \
    }

#define DEF_READER(Type)                                            \
    Type read##Type()                                               \
    {                                                               \
        size_t index_copy = index_;                                 \
        index_ += sizeof(Type);                                     \
        return *reinterpret_cast<Type*>(binary_code_ + index_copy); \
    }

size_t getFileBytesNumber(const char* filename)
{
    struct stat st = {};

    if (stat(filename, &st) != 0)
        return 0;
    
    return st.st_size;
}

class Disassembler
{
private:
    FILE* disassembled_;
    char* binary_code_;
    size_t code_size_;
    size_t index_;
    
    DEF_READER(char)
    DEF_READER(int)
    
    int readArgument()
    {
        return readint();
    }
    
    void translateLongArgument()
    {
        char type = readchar();
        if (type == NUMBER)
        {
            fprintf(disassembled_, "%d", readint());
        }
        else if (type == REGISTER)
        {
            int code = readint();
            #define DEF_REG(SRC_NAME, CODE_NAME)           \
                else if (code == SRC_NAME)                 \
                {                                          \
                    fprintf(disassembled_, #CODE_NAME);    \
                }
            if (false);
            #include "../Registers.h"
            #undef DEF_REG
        }
    }

public:
    Disassembler(const char* exec_filename, const char* disassembled_filename):
        disassembled_(fopen(disassembled_filename, "w")),
        binary_code_(nullptr),
        code_size_(0),
        index_(0)
    {
        FILE* exec_file_ = fopen(exec_filename, "rb");
        ASSERT(exec_file_, "No such binary file exist");
		ASSERT(!ferror(exec_file_), "An error in file occured");

        ASSERT(disassembled_, "No file to write code in\n");
        ASSERT(!ferror(disassembled_), "Error in file to write code in\n");

        code_size_ = getFileBytesNumber(exec_filename);
        binary_code_ = new char[code_size_];

        fread(binary_code_, sizeof(char), code_size_, exec_file_);
        fclose(exec_file_);    
    }

    void disassemble()
    {
        index_ = 0;
        int new_value = 0;
        
        while (index_ < code_size_)
        {
            char cmd_code = readchar();

            switch (cmd_code)
            {
                #define DEF_CMD(NAME, CODE_S, LONG_S, CODE_B, LONG_B, LABELS, SHORTS, ACTION) \
                    case CODE_B:                                                      \
                        fprintf(disassembled_, #NAME);                                \
                        for (size_t i = 0; i < LABELS + SHORTS; ++i)                  \
                            fprintf(disassembled_, " %d", readint());                 \
                        for (size_t i = 0; i < LONG_B; ++i)                           \
                        {                                                             \
                            fprintf(disassembled_, " ");                              \
                            translateLongArgument();                                  \
                        }                                                             \
                        fprintf(disassembled_, "\n");                                 \
                        break;
                #include "../Commands.h"
                #undef DEF_CMD
                default:
                    ASSERT(false, "Unknown command presents! Aborting!");
                    break;
            }
        }
    }

    ~Disassembler()
    {
        delete[] binary_code_;
    }
};
