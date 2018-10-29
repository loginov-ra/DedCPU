
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "Stack.h"
#include "../CommandCodes.h"

#define ASSERT(COND, MSG)                                       \
    if(!(COND))                                                 \
    {                                                           \
        fprintf(stderr, "Error happened: %s\n", MSG);           \
        fprintf(stderr, "Function: %s\n", __PRETTY_FUNCTION__); \
        fprintf(stderr, "Line: %d\n", __LINE__);                \
        assert(COND);                                           \
    }

#define DEF_READER(Type)                                         \
    Type read##Type()                                            \
    {                                                            \
        size_t ip_copy = ip_;                                    \
        ip_ += sizeof(Type);                                     \
        return *reinterpret_cast<Type*>(binary_code_ + ip_copy); \
    }

int stackPopAndDelete(Stack<int>& st)
{
    int res = st.top();
    st.pop();
    return res;
}

#define S_PUSH(arg) stack_.push((arg))
#define S_POP stackPopAndDelete(stack_)

size_t getFileBytesNumber(const char* filename)
{
    struct stat st = {};

    if (stat(filename, &st) != 0)
        return 0;
    
    return st.st_size;
}

class CPU
{
private:
    Stack<int> stack_;
    char* binary_code_;
    size_t code_size_;
    size_t ip_;
    
    DEF_READER(char)
    DEF_READER(int)
    
    int readArgument()
    {
        return readint();
    }

public:
    bool ok() const
    {
        return stack_.ok();
    }

    void dump(const char* reason) const
    {
        printf("This is the processor dump [%s]\n", reason);
        printf("Here is my memory stack\n");
        stack_.dump();
        
        printf("Status: %s\n", ok() ? "OK" : "FAILURE");
    }
 
    CPU(const char* exec_filename):
        stack_(),
        binary_code_(nullptr),
        code_size_(0),
        ip_(0)
    {
        FILE* exec_file_ = fopen(exec_filename, "rb");
        ASSERT(exec_file_, "No such binary file exist");
		ASSERT(!ferror(exec_file_), "An error in file occured");

        code_size_ = getFileBytesNumber(exec_filename);
        binary_code_ = new char[code_size_];

        fread(binary_code_, sizeof(char), code_size_, exec_file_);
        fclose(exec_file_);    
    }

    void execute()
    {
        FUNCTION_GUARD(CPU);
        ip_ = 0;
        int new_value = 0;
        
        while (ip_ < code_size_)
        {
            char cmd_code = readchar();

            switch (cmd_code)
            {
                case PUSH:
                    S_PUSH(readArgument());
                    break;

                case POP:
                    S_POP;
                    break;

                case ADD:
                    S_PUSH(S_POP + S_POP);
                    break;

                case MUL:
                    S_PUSH(S_POP * S_POP);
                    break;

                case IN:
                    new_value = 0;
                    scanf("%d", &new_value);
                    S_PUSH(new_value);
                    break;

                case OUT:
                    printf("%d\n", S_POP);
                    break;

                case END:
                    ip_ = code_size_;
                    break;

                default:
                    ASSERT(false, "Unknown command presents! Aborting!");
                    break;
            }
        }
    }


    ~CPU()
    {
        delete[] binary_code_;
    }
};
