.386
.model flat
.stack 4096
include masm32\include\kernel.inc
includelib masm32\lib\kernel.lib
.data
	a dd 0
	a1 dd 0
	a2 dd 0

.code
func proc
	push ebp
	mov ebp, esp

	mov eax, dword prt[ebp + 12]
	mov a1, eax
	mov eax, dword prt[ebp + 8]
	mov a2, eax

	mov eax, 8
	mov a, eax
for_a_0_Begin:
	mov ebx, a
	mov eax, 9
	cmp ebx, eax
	jge loopEnd_for_a_0

while1_Begin:
	mov eax, 23
	push eax
	mov eax, 22
	push eax
	mov eax, 2
	pop ebx
	mul eax, ebx
	pop ebx
	cmp eax, ebx
	jl loop2
	mov eax, 0
	jmp loop3
	loop2:
	mov eax, 1
	loop3:
	cmp eax, 1
	jne loopEnd_while1

	mov eax, 1
	push eax
	mov eax, a1
	pop ebx
	add eax, ebx
	mov a1, eax

	jmp loopEnd_while1
	jmp while1_Begin
loopEnd_while1:
	mov eax, 5
	mov a2, eax

	inc a
	jmp for_a_0_Begin
loopEnd_for_a_0:
	mov eax, 6
	mov a2, eax


funcEnd:
	mov esp, ebp
	pop ebp
	ret 8
func endp


MAIN:
main proc:
	push ebp
	mov ebp, esp

	mov eax, 7
	mov a2, eax

	mov esp, ebp
	pop ebp
	ret
main endp
