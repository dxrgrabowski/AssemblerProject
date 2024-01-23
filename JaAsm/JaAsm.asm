.data
  quarter DD 0.25      
  eighth DD 0.125      
  sixteenth DD 0.0625  

; Used registers
; xmm1 = error
; rsi = end_row
; rbx = output_image
; r8 = new_pixel
; r9 = old_pixel
; r10 = width
; r11 = height
; r12 = index
; r13 = x
; r14 = y
; r15 = input_image

.code
burkesDitheringAsm proc  

	    mov     r15, rcx
        mov     rbx, rdx
        mov     r10d, r8d
        mov     r11d, r9d
        mov     r14d, [rsp + 40]                    ; Load the value at the memory location start_row$+rsp into r14b
        mov	    rsi, [rsp + 48]

        jmp     OuterLoopStart 

OuterLoopIncrement:;--------------------------------; Hack to increment y, but not in first iteration	
        inc     r14d

OuterLoopStart:;------------------------------------; End proc if end_row greater or equal than y
        mov     eax, esi  
        cmp     r14d, eax
        jge     OuterLoopEnd                 

        mov     r13d, 0  	                        ; init x with 0, prepare start of inner loop
        jmp     InnerLoopStart
InnerLoopIncrement:;--------------------------------; Hack to increment x, but not in first iteration
        inc     r13d

InnerLoopStart:
        cmp     r13d, r10d
        jge     OuterLoopIncrement                  ;if x greater or equal than width jump to increment y
        mov     eax, r14d
        imul    eax, r10d          
        add     eax, r13d             
        mov     r12d, eax                           ; Calculate index = y * width + x and store in index        
                                                    ; Check treshold if greater than 128 set to 255 else 0
        movzx   eax, BYTE PTR [r15+r12]             ; Load pixel from input_image[index] into eax
        mov     r9d, eax
        cmp     r9, 128         
        jge     setWhite
        mov     r8, 0
        jmp     continueAfterTresholding                             
setWhite:
        mov     r8, 255   
        
continueAfterTresholding:
        mov     eax, r8d                            ; 
        mov     ecx, r9d                            ;
        sub     ecx, eax                            ; Calculate error by old_pixel - new_pixel
        cvtsi2ss xmm1, ecx                          ; CRUCIAL Convert error to float with xmm0 for further float calculations
        mov     rcx, rbx                            
        mov     BYTE PTR [rcx+r12], r8b             ; mov new_pixel(r8) to output_image[index(r12)](rbx)

skipDiffXp1:;------------x < width - 1--------------; Start error diffusion steps
        mov     eax, r10d                           ;
        dec     eax                                 ;
        cmp     r13d, eax                           ;
        jge     skipDiffXp2                         ; next step if x greater or equal than width - 1

        mov     rax, r12                            ;
        inc     rax                                 ;                                       
        movzx   eax, BYTE PTR [r15+rax]             ; Load pixel from input_image[index+1] into eax
        
        movss   xmm0, quarter                       ;
        mulss   xmm0, xmm1                          ; Multiply error with 0.25
        cvttss2si ecx, xmm0                         ; Convert float to int with xmm0 for further int calculations
        
        add     eax, ecx                            ; 
        mov     ecx, r12d                           ;
        inc     ecx                                 ;                          
        mov     BYTE PTR [r15+rcx], al              ; input_image[index + 1] += (0.25 * error)
skipDiffXp2:;------------x < width - 2--------------;
        mov     eax, r10d                           ;
        sub     eax, 2                              ;
        cmp     r13d, eax                           ;
        jge     skipDiffXm2H                        ; next step if x greater or equal than width - 2

        mov     rax, r12                            ;
        add     rax, 2                              ;                                   
        movzx   eax, BYTE PTR [r15+rax]             ; Load pixel from input_image[index+2] into eax
        
        movss   xmm0, eighth                        ;
        mulss   xmm0, xmm1                          ; Multiply error with 0.125
        cvttss2si ecx, xmm0                         ; Convert to int and save in ecx
        
        add     eax, ecx                            ;
        mov     ecx, r12d                           ;
        add     ecx, 2                              ;                           
        mov     BYTE PTR [r15+rcx], al              ; input_image[index + 2] += (0.125 * error)
skipDiffXm2H:;-------x > 1 && y < height - 1--------;
        cmp     r13d, 1               
        jle     skipDiffXm1H                        ; next step if x less or equal than 1
        mov     eax, r11d                           ;
        dec     eax                                 ;
        cmp     r14d, eax             
        jge     skipDiffXm1H                        ; next step if y greater or equal than height - 1
                                                    ;                           
        lea     rax, [r12+r10-2]                                              
        movzx   eax, BYTE PTR [r15+rax]             ; Load pixel from input_image[index + width - 2] into eax
        
        movss   xmm0, sixteenth           
        mulss   xmm0, xmm1                          ; Multiply error with 0.0625
        cvttss2si ecx, xmm0                         ; Convert to int and save in ecx
                                                    ;
        add     eax, ecx                            ;
        lea     ecx, [r12+r10-2]                                        
        mov     BYTE PTR [r15+rcx], al              ; input_image[index + width - 2] += (0.0625 * error)
skipDiffXm1H:;-------x > 0 && y < height - 1--------;
        cmp     r13d, 0 
        jle     skipDiffH                           ; next step if x less or equal than 0       
        mov     eax, r11d
        dec     eax
        cmp     r14d, eax
        jge     skipDiffH                           ; next step if y greater or equal than height - 1

        lea     rax, [r12+r10-1]
        movzx   eax, BYTE PTR [r15+rax]             ; Load pixel from input_image[index + width - 1] into eax

        movss   xmm0, eighth
        mulss   xmm0, xmm1                          ; Multiply error with 0.125
        cvttss2si ecx, xmm0                         ; Convert to int and save in ecx

        add     eax, ecx
        lea     ecx, [r12+r10-1]
        mov     BYTE PTR [r15+rcx], al              ; input_image[index + width - 1] += (0.125 * error)
skipDiffH:;--------------y < height - 1-------------;
        mov     eax, r11d
        dec     eax
        cmp     r14d, eax
        jge     skipDiffXp1h                        ; next step if y greater or equal than height - 1
        
        mov     rax, r12
        add     rax, r10
        movzx   eax, BYTE PTR [r15+rax]             ; Load pixel from input_image[index + width] into eax
        
        movss   xmm0, quarter
        mulss   xmm0, xmm1                          ; Multiply error with 0.25
        cvttss2si ecx, xmm0                         ; Convert to int and save in ecx
        
        add     eax, ecx
        mov     edx, r12d
        add     edx, r10d                            
        mov     BYTE PTR [r15+rdx], al              ; input_image[index + width] += (0.25 * error)
skipDiffXp1h:;---x < width - 1 && y < height - 1----;
        mov     eax, r10d
        dec     eax
        cmp     r13d, eax
        jge     skipDiffXp2H                        ; next step if x greater or equal than width - 1
        mov     eax, r11d
        dec     eax
        cmp     r14d, eax
        jge     skipDiffXp2H                        ; next step if y greater or equal than height - 1

        lea     rax, [r12+r10+1]
        movzx   eax, BYTE PTR [r15+rax]             ; Load pixel from input_image[index + width + 1] into eax

        movss   xmm0, eighth
        mulss   xmm0, xmm1                          ; Multiply error with 0.125
        cvttss2si ecx, xmm0                         ; Convert to int and save in ecx

        add     eax, ecx
        lea     ecx, [r12+r10+1]
        movsxd  rcx, ecx
        mov     BYTE PTR [r15+rcx], al              ; input_image[index + width + 1] += (0.125 * error)
skipDiffXp2H:;---x < width - 2 && y < height - 1----;
        mov     eax, r10d
        sub     eax, 2
        cmp     r13d, eax
        jge     JumpInnerLoopInc                    ; next step if x greater or equal than width - 2
        mov     eax, r11d
        dec     eax
        cmp     r14d, eax
        jge     JumpInnerLoopInc                    ; next step if y greater or equal than height - 1

        lea     rax, [r12+r10+2]
        movzx   eax, BYTE PTR [r15+rax]             ; Load pixel from input_image[index + width + 2] into eax

        movss   xmm0, sixteenth
        mulss   xmm0, xmm1                          ; Multiply error with 0.0625
        cvttss2si ecx, xmm0                         ; Convert to int and save in ecx

        add     eax, ecx
        lea     ecx, [r12+r10+2]
        mov     BYTE PTR [r15+rcx], al              ; input_image[index + width + 2] += (0.0625 * error)               
JumpInnerLoopInc:
        jmp     InnerLoopIncrement
OuterLoopEnd:
        ret     0
burkesDitheringAsm ENDP
END