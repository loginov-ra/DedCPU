
enum COMMANDS
{
    #define DEF_CMD(NAME, CODE_S, LONG_S, CODE_B, LONG_B, LABELS, SHORTS, ACTION) \
        CODE_B,
    #include "Commands.h"
    MAX_CMD
    #undef DEF_CMD
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
    double value_;
};
