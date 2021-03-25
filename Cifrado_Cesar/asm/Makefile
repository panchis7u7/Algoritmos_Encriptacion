cesar: cesar.o
	gcc -o cesar cesar.o -no-pie
cesar.o: cesar.asm
	nasm -f elf64 -g -F dwarf cesar.asm -l cesar.lst