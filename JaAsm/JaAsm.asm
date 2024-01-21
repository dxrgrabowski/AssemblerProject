.data
  quarter DD 0.25      
  eighth DD 0.125      
  sixteenth DD 0.0625  

index$1 = 0
x$2 = 4
y$3 = 8
error$4 = 12
tv75 = 16
old_pixel$5 = 20
new_pixel$6 = 24
input_image$ = 48
output_image$ = 56
width$ = 64
height$ = 72
start_row$ = 80
end_row$ = 88

.code
burkesDitheringAsm proc     
        ;Init stack frame
        mov     DWORD PTR [rsp+32], r9d 
        mov     DWORD PTR [rsp+24], r8d
        mov     QWORD PTR [rsp+16], rdx
        mov     QWORD PTR [rsp+8], rcx
        sub     rsp, 40                 ;allocate some stack for vars                        
        mov     eax, DWORD PTR start_row$[rsp]
        mov     DWORD PTR y$3[rsp], eax
        jmp     $LN4@burkesDith
OuterLoopStart:
        mov     eax, DWORD PTR y$3[rsp] 		   ; y
        inc     eax
        mov     DWORD PTR y$3[rsp], eax
$LN4@burkesDith:
        mov     eax, DWORD PTR end_row$[rsp]
        cmp     DWORD PTR y$3[rsp], eax
        jge     OuterLoopEnd
        mov     DWORD PTR x$2[rsp], 0
        jmp     InitVars
InnerLoopStart:
        mov     eax, DWORD PTR x$2[rsp]
        inc     eax
        mov     DWORD PTR x$2[rsp], eax
InitVars:
        mov     eax, DWORD PTR width$[rsp]
        cmp     DWORD PTR x$2[rsp], eax
        jge     JumpToOuterLoopStart
        mov     eax, DWORD PTR y$3[rsp]
        imul    eax, DWORD PTR width$[rsp]
        add     eax, DWORD PTR x$2[rsp]
        mov     DWORD PTR index$1[rsp], eax
        movsxd  rax, DWORD PTR index$1[rsp]
        mov     rcx, QWORD PTR input_image$[rsp]
        movzx   eax, BYTE PTR [rcx+rax]
        mov     DWORD PTR old_pixel$5[rsp], eax
        cmp     DWORD PTR old_pixel$5[rsp], 128         ; 00000080H
        jge     SHORT $LN16@burkesDith
        mov     DWORD PTR tv75[rsp], 0
        jmp     SHORT $LN17@burkesDith
$LN16@burkesDith:
        mov     DWORD PTR tv75[rsp], 255      ; 000000ffH
$LN17@burkesDith:
        mov     eax, DWORD PTR tv75[rsp]
        mov     DWORD PTR new_pixel$6[rsp], eax
        mov     eax, DWORD PTR new_pixel$6[rsp]
        mov     ecx, DWORD PTR old_pixel$5[rsp]
        sub     ecx, eax
        mov     eax, ecx
        cvtsi2ss xmm0, eax
        movss   DWORD PTR error$4[rsp], xmm0
        movsxd  rax, DWORD PTR index$1[rsp]
        mov     rcx, QWORD PTR output_image$[rsp]
        movzx   edx, BYTE PTR new_pixel$6[rsp]
        mov     BYTE PTR [rcx+rax], dl
        mov     eax, DWORD PTR width$[rsp]
        dec     eax
        cmp     DWORD PTR x$2[rsp], eax
        jge     SHORT $LN8@burkesDith
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
$LN8@burkesDith:
        mov     eax, DWORD PTR width$[rsp]
        sub     eax, 2
        cmp     DWORD PTR x$2[rsp], eax
        jge     SHORT $LN9@burkesDith
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
$LN9@burkesDith:
        cmp     DWORD PTR x$2[rsp], 1
        jle     SHORT $LN10@burkesDith
        mov     eax, DWORD PTR height$[rsp]
        dec     eax
        cmp     DWORD PTR y$3[rsp], eax
        jge     SHORT $LN10@burkesDith
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
$LN10@burkesDith:
        cmp     DWORD PTR x$2[rsp], 0
        jle     SHORT $LN11@burkesDith
        mov     eax, DWORD PTR height$[rsp]
        dec     eax
        cmp     DWORD PTR y$3[rsp], eax
        jge     SHORT $LN11@burkesDith
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
$LN11@burkesDith:
        mov     eax, DWORD PTR height$[rsp]
        dec     eax
        cmp     DWORD PTR y$3[rsp], eax
        jge     SHORT $LN12@burkesDith
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
$LN12@burkesDith:
        mov     eax, DWORD PTR width$[rsp]
        dec     eax
        cmp     DWORD PTR x$2[rsp], eax
        jge     SHORT $LN13@burkesDith
        mov     eax, DWORD PTR height$[rsp]
        dec     eax
        cmp     DWORD PTR y$3[rsp], eax
        jge     SHORT $LN13@burkesDith
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
$LN13@burkesDith:
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
        jmp     InnerLoopStart
JumpToOuterLoopStart:
        jmp     OuterLoopStart
OuterLoopEnd:
        add     rsp, 40                             ; 00000028H
        ret     0
burkesDitheringAsm ENDP
END