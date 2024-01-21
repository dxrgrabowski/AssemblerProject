.data
  quarter DD 0.25      
  eighth DD 0.125      
  sixteenth DD 0.0625  


;Init used vars to be pushed on stack, initialized at start with 0, backup for registers
index$1 = 0
x$2 = 4
y$3 = 8
error$4 = 12
tmp_pixel = 16  ;not used to remove
old_pixel$5 = 20
new_pixel$6 = 24
input_image$ = 48 
output_image$ = 56
width$ = 64
height$ = 72
start_row$ = 80
end_row$ = 88

; Used registers
; rax = index

.code
burkesDitheringAsm proc     
        ;Init stack frame
        mov     DWORD PTR [rsp+32], r9d 
        mov     DWORD PTR new_pixel$6[rsp], r8d
        mov     QWORD PTR tmp_pixel[rsp], rdx   ;not used to remove
        mov     QWORD PTR y$3[rsp], rcx 
        sub     rsp, 40                 ; Allocate stack for vars  
        
        mov     eax, DWORD PTR start_row$[rsp] ; Load the value at the memory location start_row$+rsp into eax
        mov     DWORD PTR y$3[rsp], eax        ; Store the value from eax into the y$3 variable on the stack
        jmp     OuterLoopStart 

OuterLoopIncrement:;---------------------------------; Hack to increment y, but not in first iteration
        mov     eax, DWORD PTR y$3[rsp] 	
        inc     eax
        mov     DWORD PTR y$3[rsp], eax
OuterLoopStart:;-------------------------------------; End proc if end_row greater or equal than y
        mov     eax, DWORD PTR end_row$[rsp]  
        cmp     DWORD PTR y$3[rsp], eax
        jge     OuterLoopEnd                 

        mov     DWORD PTR x$2[rsp], 0  	        ; init x with 0, prepare start of inner loop
        jmp     InnerLoopStart
InnerLoopIncrement:
        mov     eax, DWORD PTR x$2[rsp]
        inc     eax
        mov     DWORD PTR x$2[rsp], eax
InnerLoopStart:
        mov     eax, DWORD PTR width$[rsp]
        cmp     DWORD PTR x$2[rsp], eax
        jge     JumpToOuterLoopIncrement            ;if x greater or equal than width jump to increment y
        mov     eax, DWORD PTR y$3[rsp]
        imul    eax, DWORD PTR width$[rsp]          
        add     eax, DWORD PTR x$2[rsp]             
        mov     DWORD PTR index$1[rsp], eax         ; Calculate index = y * width + x and store in index$1
        movsxd  rax, DWORD PTR index$1[rsp]         ; mov index to rax
        mov     rcx, QWORD PTR input_image$[rsp]    ; mov input_image to rcx

        ; Check treshold if greater than 128 set to 255 else 0
        movzx   eax, BYTE PTR [rcx+rax]             ; Load pixel from input_image[index] into eax
        mov     DWORD PTR old_pixel$5[rsp], eax
        cmp     DWORD PTR old_pixel$5[rsp], 128         
        jge     setWhite
        mov     DWORD PTR new_pixel$6[rsp], 0
        jmp     continueAfterTresholding                             
setWhite:
        mov     DWORD PTR new_pixel$6[rsp], 255   
        
continueAfterTresholding:
        mov     eax, DWORD PTR new_pixel$6[rsp]   ; 
        mov     ecx, DWORD PTR old_pixel$5[rsp]   ;
        sub     ecx, eax                          ; Calculate error by old_pixel - new_pixel
        cvtsi2ss xmm0, ecx                        ; CRUCIAL Convert error to float with xmm0 for further float calculations
        movss   DWORD PTR error$4[rsp], xmm0      ; Save error on stack
        movsxd  rax, DWORD PTR index$1[rsp]
        mov     rcx, QWORD PTR output_image$[rsp]
        movzx   edx, BYTE PTR new_pixel$6[rsp]    ; mov new_pixel to edx
        mov     BYTE PTR [rcx+rax], dl            ; mov new_pixel to output_image[index]

skipDiffXp1:;------------x < width - 1--------------; Start error diffusion steps
        mov     eax, DWORD PTR width$[rsp]
        dec     eax
        cmp     DWORD PTR x$2[rsp], eax
        jge     skipDiffXp2
        mov     eax, DWORD PTR index$1[rsp]
        inc     eax
        cdqe
        movss   xmm0, DWORD PTR quarter
        mulss   xmm0, DWORD PTR error$4[rsp]
        cvttss2si ecx, xmm0
        mov     rdx, QWORD PTR input_image$[rsp]
        movzx   eax, BYTE PTR [rdx+rax]
        add     eax, ecx
        mov     ecx, DWORD PTR index$1[rsp]
        inc     ecx
        movsxd  rcx, ecx
        mov     rdx, QWORD PTR input_image$[rsp]
        mov     BYTE PTR [rdx+rcx], al
skipDiffXp2:;------------x < width - 2--------------;
        mov     eax, DWORD PTR width$[rsp]
        sub     eax, 2
        cmp     DWORD PTR x$2[rsp], eax
        jge     skipDiffXm2H
        mov     eax, DWORD PTR index$1[rsp]
        add     eax, 2
        cdqe
        movss   xmm0, DWORD PTR eighth
        mulss   xmm0, DWORD PTR error$4[rsp]
        cvttss2si ecx, xmm0
        mov     rdx, QWORD PTR input_image$[rsp]
        movzx   eax, BYTE PTR [rdx+rax]
        add     eax, ecx
        mov     ecx, DWORD PTR index$1[rsp]
        add     ecx, 2
        movsxd  rcx, ecx
        mov     rdx, QWORD PTR input_image$[rsp]
        mov     BYTE PTR [rdx+rcx], al
skipDiffXm2H:;-------x > 1 && y < height - 1----------;
        cmp     DWORD PTR x$2[rsp], 1
        jle     skipDiffXm1H
        mov     eax, DWORD PTR height$[rsp]
        dec     eax
        cmp     DWORD PTR y$3[rsp], eax
        jge     skipDiffXm1H
        mov     eax, DWORD PTR index$1[rsp]
        mov     ecx, DWORD PTR width$[rsp]
        lea     eax, DWORD PTR [rax+rcx-2]
        cdqe
        movss   xmm0, DWORD PTR sixteenth
        mulss   xmm0, DWORD PTR error$4[rsp]
        cvttss2si ecx, xmm0
        mov     rdx, QWORD PTR input_image$[rsp]
        movzx   eax, BYTE PTR [rdx+rax]
        add     eax, ecx
        mov     ecx, DWORD PTR index$1[rsp]
        mov     edx, DWORD PTR width$[rsp]
        lea     ecx, DWORD PTR [rcx+rdx-2]
        movsxd  rcx, ecx
        mov     rdx, QWORD PTR input_image$[rsp]
        mov     BYTE PTR [rdx+rcx], al
skipDiffXm1H:;-------x > 0 && y < height - 1----------;
        cmp     DWORD PTR x$2[rsp], 0
        jle     skipDiffH
        mov     eax, DWORD PTR height$[rsp]
        dec     eax
        cmp     DWORD PTR y$3[rsp], eax
        jge     skipDiffH
        mov     eax, DWORD PTR index$1[rsp]
        mov     ecx, DWORD PTR width$[rsp]
        lea     eax, DWORD PTR [rax+rcx-1]
        cdqe
        movss   xmm0, DWORD PTR eighth
        mulss   xmm0, DWORD PTR error$4[rsp]
        cvttss2si ecx, xmm0
        mov     rdx, QWORD PTR input_image$[rsp]
        movzx   eax, BYTE PTR [rdx+rax]
        add     eax, ecx
        mov     ecx, DWORD PTR index$1[rsp]
        mov     edx, DWORD PTR width$[rsp]
        lea     ecx, DWORD PTR [rcx+rdx-1]
        movsxd  rcx, ecx
        mov     rdx, QWORD PTR input_image$[rsp]
        mov     BYTE PTR [rdx+rcx], al
skipDiffH:;--------------y < height - 1---------------;
        mov     eax, DWORD PTR height$[rsp]
        dec     eax
        cmp     DWORD PTR y$3[rsp], eax
        jge     skipDiffXp1h
        mov     eax, DWORD PTR width$[rsp]
        mov     ecx, DWORD PTR index$1[rsp]
        add     ecx, eax
        mov     eax, ecx
        cdqe
        movss   xmm0, DWORD PTR quarter
        mulss   xmm0, DWORD PTR error$4[rsp]
        cvttss2si ecx, xmm0
        mov     rdx, QWORD PTR input_image$[rsp]
        movzx   eax, BYTE PTR [rdx+rax]
        add     eax, ecx
        mov     ecx, DWORD PTR width$[rsp]
        mov     edx, DWORD PTR index$1[rsp]
        add     edx, ecx
        mov     ecx, edx
        movsxd  rcx, ecx
        mov     rdx, QWORD PTR input_image$[rsp]
        mov     BYTE PTR [rdx+rcx], al
skipDiffXp1h:;------x < width - 1 && y < height - 1------;
        mov     eax, DWORD PTR width$[rsp]
        dec     eax
        cmp     DWORD PTR x$2[rsp], eax
        jge     skipDiffXp2H
        mov     eax, DWORD PTR height$[rsp]
        dec     eax
        cmp     DWORD PTR y$3[rsp], eax
        jge     skipDiffXp2H
        mov     eax, DWORD PTR index$1[rsp]
        mov     ecx, DWORD PTR width$[rsp]
        lea     eax, DWORD PTR [rax+rcx+1]
        cdqe
        movss   xmm0, DWORD PTR eighth
        mulss   xmm0, DWORD PTR error$4[rsp]
        cvttss2si ecx, xmm0
        mov     rdx, QWORD PTR input_image$[rsp]
        movzx   eax, BYTE PTR [rdx+rax]
        add     eax, ecx
        mov     ecx, DWORD PTR index$1[rsp]
        mov     edx, DWORD PTR width$[rsp]
        lea     ecx, DWORD PTR [rcx+rdx+1]
        movsxd  rcx, ecx
        mov     rdx, QWORD PTR input_image$[rsp]
        mov     BYTE PTR [rdx+rcx], al
skipDiffXp2H:;------x < width - 2 && y < height - 1------;
        mov     eax, DWORD PTR width$[rsp]
        sub     eax, 2
        cmp     DWORD PTR x$2[rsp], eax
        jge     JumpInnerLoopStart
        mov     eax, DWORD PTR height$[rsp]
        dec     eax
        cmp     DWORD PTR y$3[rsp], eax
        jge     JumpInnerLoopStart
        mov     eax, DWORD PTR index$1[rsp]
        mov     ecx, DWORD PTR width$[rsp]
        lea     eax, DWORD PTR [rax+rcx+2]
        cdqe
        movss   xmm0, DWORD PTR sixteenth
        mulss   xmm0, DWORD PTR error$4[rsp]
        cvttss2si ecx, xmm0
        mov     rdx, QWORD PTR input_image$[rsp]
        movzx   eax, BYTE PTR [rdx+rax]
        add     eax, ecx
        mov     ecx, DWORD PTR index$1[rsp]
        mov     edx, DWORD PTR width$[rsp]
        lea     ecx, DWORD PTR [rcx+rdx+2]
        movsxd  rcx, ecx
        mov     rdx, QWORD PTR input_image$[rsp]
        mov     BYTE PTR [rdx+rcx], al
JumpInnerLoopStart:
        jmp     InnerLoopIncrement
JumpToOuterLoopIncrement:
        jmp     OuterLoopIncrement
OuterLoopEnd:
        add     rsp, 40    ; Free stack
        ret     0
burkesDitheringAsm ENDP
END