
enum COMMANDS
{
    PUSH = 1,
    POP  = 2,
    ADD  = 3,
    MUL  = 4,
    IN   = 5,
    OUT  = 6,
    END  = 7,
    JMP  = 8,
    JE   = 9,
    JG   = 10,
    JGE  = 11 
};

#define DEF_REG(SRC_NAME, CODE_NAME) SRC_NAME,
enum REGISTERS
{
    #include "Registers.h"
    MAX_REG
};
#undef DEF_REG

enum ARGUMENT_TYPES
{
    NUMBER,
    REGISTER
};

struct LongArgument
{
    char argument_type_;
    int value_;
};
