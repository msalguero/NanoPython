mov esi, 10000000
mov dword[0x10000000], 10


call main
#stop
main:
push ebp
mov ebp, esp
sub esp, 36

push 1

call recursivo
add esp, 4

mov eax, 2
mov dword[ebp - 4], eax
mov eax, 5
add eax, dword[ebp -4 ]
mov dword[ebp - 8], eax 

mov eax, dword[ebp - 8]
mov dword[ebp - 12], eax

push dword[ebp -4 ]

push dword[ebp -12 ]

call suma
add esp, 8
mov dword[ebp - 16], eax
mov eax, dword[ebp - 16]
mov dword[ebp - 20], eax

#show dword[ebp -20 ]

mov eax, 1
mov dword[ebp - 24], eax
label_0:
xor ebx, ebx
mov eax, dword[ebp -24 ]
cmp eax, 10
setl bl 
mov dword[ebp - 28], ebx 

cmp dword[ebp - 28], 0
je label_1

#show dword[ebp -24 ]
mov eax, dword[ebp -24 ]
add eax, 1
mov dword[ebp - 32], eax 

mov eax, dword[ebp - 32]
mov dword[ebp -24 ], eax

jmp label_0
label_1:
xor ebx, ebx
mov eax, dword[ebp -20 ]
cmp eax, 9
sete bl 
mov dword[ebp - 36], ebx 

cmp dword[ebp - 36], 0
je label_2

#show 200

jmp label_3
label_2:

#show 100

label_3:

label_4:
cmp dword[ebp -20 ], 12
jge label_5

#show 1

inc dword[ebp -20 ]
jmp label_4
label_5:
leave
ret

suma:
push ebp
mov ebp, esp
sub esp, 8
mov eax, dword[ebp +8 ]
add eax, dword[ebp +12 ]
mov dword[ebp - 4], eax 

mov eax, dword[ebp - 4]
mov dword[ebp - 8], eax

mov eax, dword[ebp -8 ]
leave
ret

recursivo:
push ebp
mov ebp, esp
sub esp, 12

#show dword[ebp +8 ]
xor ebx, ebx
mov eax, dword[ebp +8 ]
cmp eax, 10
sete bl 
mov dword[ebp - 4], ebx 

cmp dword[ebp - 4], 0
je label_6

mov eax, 1

jmp label_7
label_6:
mov eax, dword[ebp +8 ]
add eax, 1
mov dword[ebp - 8], eax 

mov eax, dword[ebp - 8]
mov dword[ebp +12 ], eax

push dword[ebp +12 ]

call recursivo
add esp, 4
mov dword[ebp - 12], eax
mov eax, dword[ebp - 12]

label_7:
leave
ret


