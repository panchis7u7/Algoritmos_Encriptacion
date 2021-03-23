extern printf
section .data
    ;int main(int argc, char** argv)
    ;variables que almacenan los argumentos pasados al 
    ;programa.
    ;------------------------------------------------------
    ;El orden de los registros: 
    ;%rdi, %rsi, %rdx, %rcx, %r8 and %r9 
    ;eso significa que argc esta en %rdi y argv en %rsi.
    argcstr db 'argc = %i\n\0'
    argvstr db 'argv[%u] = %s\n\0'  ;%u -> unsigned int
    mensaje db "hola %i",0xA,0
    sys_stdout equ 1
section .bss
    argc resq 1
    nDesplazamientos resb 1
section .text
global main
main:
    push rbp        ;Guarda el stack frame de main.
    mov rbp, rsp

    xor rbx,rbx                     ;Limpiamos rbx.
    ;mov [argc], rdi                 ;Guardamos el numero de argumentos.
    ;mov r8, qword [rsi+8]           ;Guardamos el primer argumento, el cual
    ;mov [nDesplazamientos], r8      ;debe ser el numero de rotaciones.
    mov bl, [rsi]
    ;sub byte bl, '0'
    mov [nDesplazamientos], bl

    ;mov rax, 1
    ;mov rdi, 1
    ;mov rsi, mensaje
    ;mov rdx, 5
    ;syscall

    mov rdi, mensaje
    mov rax, 0
    mov rsi, [nDesplazamientos]
    call printf
    
    mov rsp, rbp
    pop rbp
ret
