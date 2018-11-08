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
je 201
push 0
push dx
jg 265
push dx
push 0
jg 181
push 0
out
jmp 309
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
jmp 309
push 2
out
call 310
sub
div
out
call 310
add
div
out
jmp 309
end
push 2
push ax
mul
push dx
sqrt
push bx
push -1
mul
ret
