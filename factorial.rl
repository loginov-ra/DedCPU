
in ax
push 1
pop bx
call :factorial
push bx
out
end

:factorial
    push ax
    push 1
    jge :stopcrit

    push ax
    push bx
    mul
    pop bx

    push 1
    push ax
    sub
    pop ax

    call :factorial
    :stopcrit
    ret
    
