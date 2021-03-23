section .data
    ;int main(int argc, char** argv)
    ;variables que almacenan los argumentos pasados al 
    ;programa.
    ;argcstr db 'argc = %d\n\0'
    ;argvstr db 'argv[%u] = %s\n\0'  ;%u -> unsigned int
    mensaje db "hola",0xA,0
    sys_stdout equ 1
section .bss
    argc resb 8
    argv resb 8
section .text
global main
main:
    mov rax,0       ;Limpiamos el registro RAX.
    lea argv, rax   ;RAX apunta a argv.
    pop rax          ;Guardar el numero de argumentos (int argc)
    pop r9
    push rbp        ;Guarda el stack frame de main.
    mov rbp, rsp

    mov rax, 1
    mov rdi, 1
    mov rsi, r9
    mov rdx, 100
    syscall

    mov rsp, rbp
    pop rbp
ret
