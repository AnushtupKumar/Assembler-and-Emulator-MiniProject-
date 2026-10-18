# Student Name: Anushtup Kumar
# User ID: 2501CS32
# Declaration of Authorship: I declare that this code is my own work.

CC = g++
CFLAGS = -std=c++11 -Wall -pedantic

all: asm emu

asm: asm.cpp
	$(CC) $(CFLAGS) asm.cpp -o asm

emu: emu.cpp
	$(CC) $(CFLAGS) emu.cpp -o emu

run: all
ifndef FILE
	@echo Error: Please provide an assembly file.
	@echo Usage: mingw32-make run FILE=bubble_sort.asm
else
	@echo --- Running Assembler ---
	asm.exe $(FILE)
	@echo --- Running Emulator ---
	emu.exe $(FILE:.asm=.o)
endif

clean:
	rm -f asm emu *.o *.lst