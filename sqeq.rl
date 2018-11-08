
in ax
in bx
in cx

push ax
push cx
push 4
mul
mul
push bx
push bx
mul
sub
pop dx

push dx
push 0
je :zero

push 0
push dx
jg :greater

push dx
push 0
jg :less

:less
    push 0
    out
    jmp :finish

:zero
    push 1
    out
    push 2
    push ax
    mul
    push bx
    push -1
    mul
    div
    out
    jmp :finish

:greater
    push 2
    out
    call :root_part
    sub
    div
    out
    call :root_part
    add
    div
    out
    jmp :finish

:finish
end

:root_part    
    push 2
    push ax
    mul
    push dx
    sqrt
    push bx
    push -1
    mul
    ret   
