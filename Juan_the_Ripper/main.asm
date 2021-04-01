extern _ZN6SHA5124hashEPKc              ;Funcion para hashear.
extern _Z7getSizePc                     ;funcion para obtener el tamano.
extern printf                           ;Funcion para imprimir.
section .data
	MAP_FAILED      equ -1
	PROT_READ       equ 0x1
	PROT_WRITE      equ 0x2
	PROT_EXEC       equ 0x4
	PROT_NONE       equ 0x0
	MAP_PRIVATE     equ 0x2
	MAP_ANONYMOUS   equ 0x20

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
    error_Read 		db "Error al leer el archivo!",NL,0
    error_Print 	db "Error al imprimir cadena!",NL,0
    error_Position 	db "Error al posicionar archivo!",NL,0

    formato db "%s",0xA,0x0             ;Formato simple para hacer debug.
    formatoInt db "%d",0xA,0x0

    %define KNRM  `\x1B[0m`
    %define KRED  `\x1B[31m`
    %define KGRN  `\x1B[32m`
    %define KYEL  `\x1B[33m`
    %define KBLU  `\x1B[34m`
    %define KMAG  `\x1B[35m`
    %define KCYN  `\x1B[36m`
    %define KWHT  `\x1B[37m`

    strLimite db KCYN,`--------------------------------------------------------`,0xA,0x0
    nombreAplicacion db KYEL,`\t\tJuan the Ripper\n`,0xA,0x0
    archivoCargado db KWHT, `Diccionario cargado:`, KYEL, ` %s`,0xA,0x0
    hashCargado db KWHT, `Hash cargado:`, KYEL, ` %s`,0xA,0x0
    algoritmo db KWHT, `Algoritmo SHA-512 para todos los hashes.`,0xA,0x0
    msgAbortar db `Presione`, KRED, ` CTRL-C `, KWHT, `para salir del programa.`, KYEL, 0xA, `Descifrando...`,0xA,0x0
    hashEncontrado db KWHT,`Hash Encontrado para %s => (`, KYEL, `%s`, KWHT, `)`,0xA,0x0
    hashNoEncontrado db KRED, `Hash no encontardo en el diccionario!`,0xA,0x0
    tamanoHashPrincipal db KWHT, `Tamano hash principal: `, KYEL,`%d`,0xA,0x0
    tamanoHashSecundario db KWHT, `Tamano hash secundario: `, KYEL,`%d`,0xA,0x0

section .bss
    buffer resq bufferlen               ;Buffer para cada linea del archivo.
    directorioHash resq 1               ;Almacen de la contraseña en texto plano.
    hash resq 1                         ;Hash.
    hash2 resq 1
    sha resq 1                          ;Almacen de la contraseña cifrada.
    diccionario resq 1                  ;Ruta absoluta del diccionario de datos.
    lineSize resq 1                     ;Longitud de cadena de cada linea.
    hashLen resq 1                      ;Longitud del hash.
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
    mov qword [directorioHash], rsi     ;Guardar el directorio del hash.
    mov qword [diccionario], rdx        ;Guardar la ruta del diccionario.

    ;--------------------------------------------------------------------------

    ;Imprimir borde y titulo de la aplicacion.
    ;--------------------------------------------------------------------------

    sub rsp, 20h
    mov rax, 0
    mov rdi, strLimite
    call printf

    mov rax, 0
    mov rdi, nombreAplicacion
    call printf
    add rsp, 20h

    ;--------------------------------------------------------------------------

    ;Abrir el archivo.
    ;--------------------------------------------------------------------------

    ;Archivo que contiene el hash.
    mov rax, NR_open                    ;Syscall Open -> 2.
    mov rdi, [directorioHash]           ;Ruta del archivo.
    mov rsi, O_RDWR                     
    syscall
    mov qword [FD], rax

    cmp rax, 0
    jl openError

    mov r8, 0
    mov [lineSize], r8
    call readLine
    mov [hash], rax                         ;Guardamos el hash.

    mov r10, [nchar]
    mov [hashLen], r10                      ;Guardamos la longitud del hash.

    ;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    ;sub rsp, 20h

    ;mov rdi, tamanoHashPrincipal
    ;mov rsi, [hashLen]
    ;call printf

    ;add rsp, 20h
    ;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    mov rax, 9								;mmap syscall no.
	mov rdi, 0								;hint. 
	mov rsi, [hashLen]						;tamano de la pagina.
	mov rdx, PROT_READ | PROT_WRITE			;Memoria de ;
	mov r10, MAP_PRIVATE | MAP_ANONYMOUS	;no shared pages
	mov r8, 0 								;no file descriptor
	mov r9, 0								;no offset
	syscall

    cmp rax, MAP_FAILED
    je positionError

    mov rdx, rax                        ;Copy hash.
    cld
    mov rsi, [hash]
    mov rdi, rdx
    mov rcx, [hashLen]
    rep movsb

    mov [hash], rdx                     ;Se guarda la copia.

    mov rax, 3
    mov rdi, [FD]                       ;Cerrar el descriptor de datos
    syscall                             ;abierto.

    ;Archivo de diccionario de datos.
    mov rax, NR_open                    ;Syscall Open -> 2.
    mov rdi, [diccionario]              ;Ruta del archivo.
    mov rsi, O_RDWR                     ;Abrir en modo Lectura/Escritura.
    syscall
    mov qword [FD], rax                 ;Guardar descriptor de datos.

    cmp rax, 0                          ;Checar si hubo un error.
    jl openError 

    sub rsp, 20h

    mov rax, 0
    mov rdi, archivoCargado             ;Imprimir la carga del diccionario.
    mov rsi, [diccionario]
    call printf

    mov rax, 0
    mov rdi, hashCargado                ;Imprimir la carga del hash.
    mov rsi, [directorioHash]
    call printf

    mov rax, 0 
    mov rdi, algoritmo                  ;Imprimir el algoritmo detectado.
    call printf

    mov rax, 0
    mov rdi, msgAbortar                 ;Imprimir mensajes de estado.
    call printf

    add rsp, 20h

    ;--------------------------------------------------------------------------

    ;Leer el archivo linea por linea y comparar.
    ;--------------------------------------------------------------------------

    mov r8, 0                           ;Inicializar registros.
    mov [lineSize], r8                  ;Espacio de cada linea en 0.      
readLoop:
    mov r8, [lineSize]
    call readLine                       ;Obtener texto de la linea 'n' del
                                        ;archivo de texto.
    cmp rax, 0                          ;Checar si retorno EOF (fin de archivo).
    jle noEncontrado

    mov r9, rax

    ;Cifrar el texto plano.
    ;--------------------------------------------------------------------------

    sub rsp, 20h                        ;Espacio de sombra para *this, input.

    mov rdi, [this]                     ;Apuntador que hace referencia al objeto.
    mov rsi, rax                         ;Pasar como argumento el mensaje.
    call _ZN6SHA5124hashEPKc            ;llamar la funcion hash(char* input).
    mov qword [sha], rax                ;Guardar el hash (valor de retorno).

    add rsp, 20h                        ;Reestablecer la pila.

    ;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    ;push rax

    ;mov rdi, [sha]
    ;call _Z7getSizePc

    ;sub rsp, 20h

    ;mov rdi, tamanoHashSecundario
    ;mov rsi, rax
    ;mov rax, 0
    ;call printf

    ;add rsp, 20h

    ;pop rax

    ;sub rsp, 20h

    ;mov rdi, formato
    ;mov rsi, [sha]
    ;mov rax, 0

    ;call printf

    ;add rsp, 20h

    ;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    ;--------------------------------------------------------------------------

    mov rcx, 0                          ;Comparar las cadenas.
    mov rsi, [sha]
    mov rdi, [hash]
    mov rcx, [hashLen]
    repe cmpsb
    jne readLoop
    jmp encontrado 

noEncontrado:

    sub rsp, 20h
    mov rdi, hashNoEncontrado             ;Imprimir el borde punteado.
    mov rax, 0
    call printf
    add rsp, 20h
    jmp Salida
    
encontrado: 

    sub rsp, 20h

    mov rdi, hashEncontrado                 ;Imprimir estado de encontrado.
    mov rsi, [directorioHash]
    mov rdx, buffer
    mov rax, 0
    call printf

    add rsp, 20h

    ;--------------------------------------------------------------------------

    ;sub rsp, 20h

    ;mov rax, 0
    ;mov rdi, formato                   ;Imprimir el hash.
    ;mov rsi, [sha]
    ;call printf

    ;add rsp, 20h

    mov rax, 3
    mov rdi, [FD]                       ;Cerrar el descriptor de datos
    syscall                             ;abierto.

    ;Imprimir borde final.
    ;--------------------------------------------------------------------------

Salida:
    sub rsp, 20h
    mov rax, 0
    mov rdi, strLimite
    call printf
    add rsp, 20h

    mov rax, 3
    mov rdi, [FD]
    syscall

    ;--------------------------------------------------------------------------

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

    mov rax, 0
    mov [nchar], rax

    mov rdi, qword [FD]                 ;Descriptor de datos abierto.
    mov rsi, qword r8                   ;Offset de 0.
    mov rdx, 0
    mov rax, NR_lseek                   ;Syscall lseek.
    syscall

    cmp rax, 0                          ;Checar si hubo errores.
    jl positionError                    ;Si hubo, brinca.

    mov rdi, qword [FD]                 ;Lee archivo a partir del offset.
    mov rsi, buffer                     ;Buffer en donde se almacenara el
    mov rdx, bufferlen                  ;contenido.
    mov rax, NR_read                    ;Syscall read.
    syscall

    cmp rax, 0                          ;Checar si hay errores.
    je endOfFile
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
    inc qword [nchar]
    add qword r8, 1
    jmp loop
continue:
    mov byte [r8], byte 0
    ;sub rsp, 20h

    ;mov rax, 0
    ;mov rdi, formato                   ;Imprimir la cadena.
    ;mov rsi, buffer
    ;call printf

    ;add rsp, 20h
    
    mov rax, buffer

    mov rsp, rbp                        ;Reestablecer el apuntador de
    pop rbp                             ;la pila.
    ret

endOfFile:
    mov rax, 0

    mov rsp, rbp                        ;Reestablecer el apuntador de
    pop rbp                             ;la pila.
    ret

    ;#############################################################################
    ;--------------------------------------------------------------------------