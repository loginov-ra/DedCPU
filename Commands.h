
//File for code generation.
//Here are parameters for command:
// - name in user code
// - small-argnum code
// - small-argnum
// - big-argnum code
// - big-argnum
// - label-argnum
// - short-argnum
// - action

DEF_CMD(push, PUSH, 1, _PUSH, 1, 0, 0, {
    S_PUSH(readLongArgument());
})

DEF_CMD(pop, POP, 0, _POP, 1, 0, 0, {
    arg = readchar();
    ASSERT(arg == REGISTER, "Not a register in pop");
    ptr = getRegisterPtr();
    ASSERT(ptr, "Null pointer for register in pop");
    *ptr = S_POP;
})

DEF_CMD(add, ADD, 0, _ADD, 0, 0, 0, {
    S_PUSH(S_POP + S_POP);
})

DEF_CMD(mul, MUL, 0, _MUL, 0, 0, 0, {
    S_PUSH(S_POP * S_POP);
})

DEF_CMD(in, IN, 0, _IN, 1, 0, 0, {
    arg = readchar();
    ASSERT(arg == REGISTER, "Not a register in in");
    ptr = getRegisterPtr();
    ASSERT(ptr, "Null pointer for register in in");
    scanf("%d", ptr);
})

DEF_CMD(out, OUT, 0, _OUT, 0, 0, 0, {
    printf("%d\n", S_POP);
})

DEF_CMD(sub, SUB, 0, _SUB, 0, 0, 0, {
    top = S_POP;
    top_snd = S_POP;
    S_PUSH(top - top_snd);
})

DEF_CMD(div, DIV, 0, _DIV, 0, 0, 0, {
    top = S_POP;
    top_snd = S_POP;
    S_PUSH(top / top_snd);
})

DEF_CMD(sqrt, SQRT, 0, _SQRT, 0, 0, 0, {
    S_PUSH(sqrt(S_POP));
})

DEF_CMD(end, END, 0, _END, 0, 0, 0, {
    ip_ = code_size_;
})

DEF_CMD(jmp, JMP, 0, _JMP, 0, 1, 0, {
    ip_ = readArgument();
})

DEF_CMD(call, CALL, 0, _CALL, 0, 1, 0, {
    arg = readArgument();
    returns_.push(ip_);
    ip_ = arg;
})

DEF_CMD(ret, RET, 0, _RET, 0, 0, 0, {
    ip_ = stackPopAndDelete(returns_);
})

DEF_CMD(je, JE, 0, _JE, 0, 1, 0, {
    arg = readint();
    if (S_POP == S_POP)
        ip_ = arg;
})

DEF_CMD(jg, JG, 0, _JG, 0, 1, 0, {
    arg = readint();
    top = S_POP;
    top_snd = S_POP;
    if (top > top_snd)
        ip_ = arg;
})

DEF_CMD(jge, JGE, 0, _JGE, 0, 1, 0, {
    arg = readint();
    top = S_POP;
    top_snd = S_POP;
    if (top >= top_snd)
        ip_ = arg;
})

