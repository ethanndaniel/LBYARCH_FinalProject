section .text
bits 64
default rel

global asmkernel
asmkernel:
    ; retrieve 5th and 6th arguments from the stack
    mov r10, [rsp + 40]
    mov r11d, [rsp + 48]
    xor rax, rax
.loop:
    cmp rax, r11
    jge .done
    
    ; compute dx = X2[i] - X1[i] and square it (dx * dx)
    movss xmm0, [rdx + rax*4]
    subss xmm0, [rcx + rax*4]
    mulss xmm0, xmm0

    ; compute dy = Y2[i] - Y1[i] and square it (dy * dy)
    movss xmm1, [r9 + rax*4]
    subss xmm1, [r8 + rax*4]
    mulss xmm1, xmm1

    ; sum squared differences and compute square root
    addss xmm0, xmm1
    sqrtss xmm0, xmm0

    ; store result in Z[i]
    movss [r10 + rax*4], xmm0

    inc rax
    jmp .loop

.done:
    ret