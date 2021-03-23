;--------------------------------------------------------------------------------
;Cifrado Cesar - Carlos Sebastian Madrigal Rodriguez
;23 de Marzo, 2021
;--------------------------------------------------------------------------------
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
    mensaje db "hola %d",0xA,0
    sys_stdout equ 1
section .bss
    argc resq 1
    nDesplazamientos resb 1
section .text
global main
main:
    push rbp                            ;Guarda el apuntador de la base de
    mov rbp, rsp                        ;la pila.

    xor rax,rax                         ;Limpiamos rax.
    xor rdx, rdx                        ;Limpiamos rdx.
    mov [argc], rdi                     ;Guardamos el numero de argumentos.
    mov rax, [rsi+8]                    ;Direccion de memoria de argv[1] en r8.
    mov rdx, [rax]                      ;Desreferencia *argv[1].
    sub dl, '0'                         ;Convertimos de string a int.
    mov [nDesplazamientos], byte dl     ;Guardamos el numero de desplazamientos.


    mov rdi, mensaje
    mov rax, 0
    mov rsi, [nDesplazamientos]
    call printf
    
    mov rsp, rbp
    pop rbp
ret
