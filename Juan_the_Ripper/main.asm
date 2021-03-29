extern _ZN6SHA5124hashEPKc              ;Funcion para hashear.
extern printf                           ;Funcion para imprimir.
section .data
    CREATE 		equ 	1               ;Expresiones usadas para condicionales.
	OVERWRITE 	equ 	1
	APPEND 		equ 	1
	O_WRITE 	equ 	1
	READ 		equ 	1
	O_READ 		equ 	1
	DELETE 		equ 	1

    NR_read  	equ 	0               ;Simbolos para llamadas al sistema.
    NR_write 	equ 	1 
    NR_open 	equ 	2 
    NR_close 	equ 	3
    NR_lseek 	equ 	8 
    NR_create 	equ 	85
    NR_unlink 	equ 	87

	O_CREAT 	equ 	00000100q       ;Banderas de creacion y de estado.  
    O_APPEND 	equ 	00002000q
  
    O_RDONLY 	equ 	000000q         ;Modos de acceso.
    O_WRONLY 	equ 	000001q 
    O_RDWR 		equ 	000002q

    S_IRUSR 	equ 	00400q          ;Permiso de lectura para el usuario.
    S_IWUSR 	equ 	00200q 	        ;Permiso de escritura para el usuario.
 
    NL	  		equ		0xa					
    bufferlen  	equ 	255

    FD 		 dq 	0	                ;File descriptor

    len db 0
    error_Close 	db "Error al cerrar el archivo!",NL,0
    error_Write 	db "Error al escribir al archivo!",NL,0
    error_Open 		db "Error al abrir archivo!",NL,0
    error_Append 	db "error appending to file",NL,0
    error_Delete 	db "error deleting file",NL,0
    error_Read 		db "Error al leer el archivo!",NL,0
    error_Print 	db "error printing string",NL,0
    error_Position 	db "error positioning in file",NL,0
        
    success_Create 	db "File created and opened",NL,0
    success_Close 	db "File closed",NL,NL,0
    success_Write 	db "Written to file",NL,0
    success_Open 	db "File opened for reading/(over)writing/updating",NL,0
    success_Append 	db "File opened for appending",NL,0
    success_Delete 	db "File deleted",NL,0
    success_Read 	db "Reading file",NL,0
    success_Position db "Positioned in file",NL,0

    formato db "%s",0xA,0x0             ;Formato simple para hacer debug.
    formatoInt db "%d",0xA,0x0
    archivoCargado db "Archivo cargado: %s",0xA,0x0

section .bss
    mensajeTextoPlano resq 1            ;Almacen de la contraseña en texto plano.
    sha resq 1                          ;Almacen de la contraseña cifrada.
    diccionario resq 1                  ;Ruta absoluta del diccionario de datos.
    buffer resq bufferlen               ;Buffer para cada linea del archivo.
    lineSize resq 1                     ;Longitud de cadena de cada linea.
    nchar resq 1                        ;Longitud leida de read().
    this resq 1                         ;Apuntador que hace referencia al objeto.

section .text
global start
start: 
    push rbp                            ;Guarda el apuntador de la base de
    mov rbp, rsp                        ;la pila.

    ;Guardar datos provenientes desde C++.
    ;--------------------------------------------------------------------------

    mov qword [this], rdi               ;Guardar la referencia al objeto.
    mov qword [mensajeTextoPlano], rsi  ;Guardar el mensaje en texto plano.
    mov qword [diccionario], rdx        ;Guardar la ruta del diccionario.

    ;--------------------------------------------------------------------------

    ;Abrir el archivo.
    ;--------------------------------------------------------------------------

    mov rax, NR_open                    ;Syscall Open -> 2.
    mov rdi, [diccionario]              ;Ruta del archivo.
    mov rsi, O_RDWR                     ;Abrir en modo Lectura/Escritura.
    syscall
    mov qword [FD], rax                 ;Guardar descriptor de datos.

    cmp rax, 0                          ;Checar si hubo un error.
    jl openError 

    sub rsp, 20h

    mov rax, 0
    mov rdi, archivoCargado             ;Imprimir el borde punteado.
    mov rsi, [diccionario]
    call printf

    add rsp, 20h

    ;--------------------------------------------------------------------------

    ;Leer el archivo linea por linea y comparar.
    ;--------------------------------------------------------------------------

    mov r8, 0
    call readLine
    mov r8, [lineSize]
    call readLine

    ;--------------------------------------------------------------------------

    ;Cifrar el texto plano.
    ;--------------------------------------------------------------------------

    sub rsp, 20h                        ;Espacio de sombra para *this, input.

    mov rdi, [this]                     ;Apuntador que hace referencia al objeto.
    mov rsi, [mensajeTextoPlano]        ;Pasar como argumento el mensaje.
    call _ZN6SHA5124hashEPKc            ;llamar la funcion hash(char* input).
    mov qword [sha], rax                ;Guardar el hash (valor de retorno).

    add rsp, 20h                        ;Reestablecer la pila.

    ;--------------------------------------------------------------------------

    sub rsp, 20h

    mov rax, 0
    mov rdi, formato                    ;Imprimir el borde punteado.
    mov rsi, [sha]
    call printf

    add rsp, 20h

    mov rdi, [FD]                       ;Cerrar el descriptor de datos
    syscall                             ;abierto.

    mov rsp, rbp                        ;Reestablecer el apuntador de
    pop rbp                             ;la pila.
    ret                                 ;Regresamos a c++.

    ;Errores.
    ;--------------------------------------------------------------------------
openError:
    mov rdi, error_Open
    call printString 
    jmp exit_error

positionError:
    mov rdi, error_Position
    call printString
    jmp exit_error

readError:
    mov rdi, error_Read
    call printString
    jmp exit_error

exit_error:
    mov rsp, rbp
    pop rbp

    mov rax, 60                         ;Cerrar con error.
    mov rdi, -1
    syscall
    ret

    ;--------------------------------------------------------------------------

    ;Funciones.
    ;--------------------------------------------------------------------------

;Funcion readLine.
;#############################################################################
global printString
printString:
    push rbp                            ;Guarda el apuntador de la base de
    mov rbp, rsp                        ;la pila.
; Count characters 
    mov r12, rdi
    mov rdx, 0 
strLoop:
    cmp byte [r12], 0
    je strDone
    inc rdx                             ;Longitud en rdx.
    inc r12
    jmp strLoop
strDone:
    cmp rdx, 0                          ;No string (0 length)
    je prtDone
    mov rsi,rdi
    mov rax, 1 
    mov rdi, 1
    syscall
prtDone:
    mov rsp, rbp                        ;Reestablecer el apuntador de
    pop rbp                             ;la pila.
    ret

;#############################################################################

;Funcion readLine.
;#############################################################################
global readLine
readLine:
    push rbp                            ;Guarda el apuntador de la base de
    mov rbp, rsp                        ;la pila.

    mov rdi, qword [FD]                 ;Descriptor de datos abierto.
    mov rsi, qword r8                   ;Offset de 0.
    mov rdx, 0
    mov rax, NR_lseek                   ;Syscall lseek.
    syscall

    cmp rax, 0                          ;Checar si hubo errores.
    jl positionError                    ;Si hubo, brinca.
    mov qword [nchar], rax              ;Guarda el numero de bits leidos.

    mov rdi, qword [FD]                 ;Lee archivo a partir del offset.
    mov rsi, buffer                     ;Buffer en donde se almacenara el
    mov rdx, bufferlen                  ;contenido.
    mov rax, NR_read                    ;Syscall read.
    syscall

    cmp rax, 0                          ;Checar si hay errores.
    jl readError

    mov r8 , buffer                     ;Generar otro apuntador para no 
    mov r9, 0                           ;perder la raiz.
    mov qword rcx, [lineSize]
    cmp qword [lineSize], 0             ;Es la primera linea del archivo?
    cmove rcx, r9                       ;Si es, mueve un 0.
    mov [lineSize], rcx                 ;Si no, deja el valor donde quedo.

loop:
    inc qword [lineSize]
    cmp byte [r8], byte 0xA
    jne increment
    jmp continue
increment:
    add qword r8, 1
    jmp loop
continue:
    mov byte [r8], byte 0
    sub rsp, 20h

    mov rax, 0
    mov rdi, formato                    ;Imprimir el borde punteado.
    mov rsi, buffer
    call printf

    add rsp, 20h

    mov rsp, rbp                        ;Reestablecer el apuntador de
    pop rbp                             ;la pila.
    ret

    ;#############################################################################
    ;--------------------------------------------------------------------------