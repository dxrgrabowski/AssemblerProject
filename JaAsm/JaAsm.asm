.code 
burkesDitheringAsm proc

; Przyjêcie argumentów z rejestrów
	mov rdi, [rsp + 8]      ; inputImage
	mov rsi, [rsp + 16]     ; outputImage
	mov rdx, [rsp + 24]     ; width
	mov rcx, [rsp + 32]     ; height
	mov r8, [rsp + 40]      ; current y ; startRow
	mov r9, [rsp + 48]      ; endRow

	mov r13, r8 ; init y

nextRow:  ; y = startRow ; r13 = y ; 
	add r13, 1
	cmp r13, r9
jge procEnd
	
	mov r15, 1 ; init x
columnLoop:  ; x = 1 ; r15 = xx
	cmp r15, rdx
	jge nextRow

	imul rax, r13, rdx ; y * width
	add rax, r15 ; y * width + x


	mov r14, r15 ; init x

	add r15, 1        ; x += 1
	jmp columnLoop    ; goto columnLoop



		

procEnd:
	ret

burkesDitheringAsm endp
end