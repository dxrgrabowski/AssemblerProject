.code
mmx proc
    movdqu xmm0, [rcx]
    movdqu xmm1, [rdx]
    pand xmm0,xmm1
    movdqu [r8], xmm0
    ret
mmx endp

end