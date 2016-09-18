format ELF
extrn printf
public main
section '.data'
format_string db "%d ", 0
x dd 5

section '.text'
main:
push ebp
mov ebp, esp
sub esp, 4

push 12
mov eax, 2
add eax, 2
mov dword[ebp - 4], eax 

push dword[ebp - 4]

call test
add esp, 8
leave
ret

test:
push ebp
mov ebp, esp
sub esp, 4

mov eax, 2
mov dword[ebp - 4], eax
leave
ret


