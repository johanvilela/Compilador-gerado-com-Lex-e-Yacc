# Makefile para o CCC - Compilador que Compila Código

all: compile clean-compiled-files

compile: bison flex gcc

bison:
	bison -d codigo/flex-bison/sintatico.y -o codigo/flex-bison/y.tab.c

flex:
	flex -o codigo/flex-bison/lex.yy.c codigo/flex-bison/lexico.l

gcc:
	gcc -g -o CCC codigo/flex-bison/*.c codigo/codigo_c/*.c -Icodigo/flex-bison/ -Icodigo/codigo_h/

clean-compiled-files:
	rm -rf codigo/flex-bison/*.c codigo/flex-bison/*.h

clean:
	rm -rf ccc

compila:
	./ccc $(var).txt

monta:
	nasm -f elf64 $(var).asm

linka:
	gcc -g $(var).o -o $(var)

limpa:
	rm -rf $(var).asm $(var).o $(var)
