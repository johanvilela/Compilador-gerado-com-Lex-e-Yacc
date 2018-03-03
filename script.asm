extern scanf
section .data
fmt_d: db "%ld", 0
fmt_f: db "%f", 0
fmt_s: db "%s", 0
fmt_dln: db "%ld", 10, 0
fmt_fln: db "%f", 10, 0
fmt_sln: db "%s", 10, 0

i: dq 0
num: dq 0

section .text
global main

main:
mov rbx,1
push rbx
mov rbx,2
push rbx
pop r8
pop rax
xor rdx,rdx
idiv r8
push rax
mov rbx,1
push rbx
mov rbx,2
push rbx
pop rcx
pop rbx
imul rbx,rcx
push rbx
pop rcx
pop rbx
add rbx,rcx
push rbx
pop rbx
mov [i],rbx
mov rbx,999
push rbx
pop rbx
mov [num],rbx
mov rbx,4
push rbx
pop rcx
mov rbx, [i]
cmp rbx, rcx
jnl rotulo0
mov rdi,fmt_dln
mov rsi,[i]
mov rax,0
call printf
jmp rotulo1
rotulo0:
mov rdi,fmt_dln
mov rsi,[num]
mov rax,0
call printf
rotulo1:
mov rax,0
ret
