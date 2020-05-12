all: msg main

msg: main
	@echo Compiled successfully!

main: pageRank.c
	gcc pageRank.c -lm -o pageRank.out
