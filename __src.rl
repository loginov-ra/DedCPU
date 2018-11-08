in ax
push 1
pop bx
call 31
push bx
out
end
push ax
push 1
jge 91
push ax
push bx
mul
pop bx
push 1
push ax
sub
pop ax
call 31
ret
