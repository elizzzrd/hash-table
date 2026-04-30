section .text

global str_equal_16
global str_len_16


;=====================================================================
;   str_equal_16 - compares 16 bytes and return 1, if they are equal
;   bool str_equal_16(const char * a, const char * b)
;   
;   entry:
;       rdi - 1 str address
;       rsi - 2 str address
;       sse2
;   exit:
;       eax - 1 (equal), 0 (non eqaul)  
;=====================================================================
str_equal_16:
    movdqu xmm0, [rdi]      ; MOVe Double-Quadword Unaligned
    movdqu xmm1, [rsi]

    pcmpeqb xmm0, xmm1      ; Packed CoMPare EQual Bytes

    pmovmskb eax, xmm0      ; Packed Move MaSK Byte

    cmp eax, 0xFFFF
    sete al
    movzx eax, al

    ret


;=====================================================================
;   str_len - return str len (max 16 bytes)
;   int str_len_16(const char * str)
;   
;   entry:
;       rdi - str address
;       sse2
;   exit:
;       eax - 1 (equal), 0 (non eqaul)  
;=====================================================================
str_len_16:
    pxor    xmm1, xmm1
    mov     rax, rdi

.loop:
    movdqu  xmm0, [rdi]     
    pcmpeqb xmm0, xmm1      ; if (*str == '\0')

    pmovmskb ecx, xmm0
    test ecx, ecx
    jnz .found              ; found null-terminator

    add rdi, 16
    jmp .loop

.found:
    bsf ecx, ecx            ; bit scan forward (search for first non-zero byte and save position)
    sub rdi, rax
    lea rax, [rdi + rcx]
    ret

section .note.GNU-stack noalloc noexec nowrite progbits