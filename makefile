all: msg trab1

msg: trab1

	@echo Compilado com sucesso!

trab1: trab1.c
	gcc trab1.c -lm -o trab1.out

