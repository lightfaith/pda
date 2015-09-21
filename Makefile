CC=g++
CFLAGS=-g -std=c++11
#CFILES=main.cpp lib.cpp lib.hpp instructionset.cpp instructionset.hpp instructioninfo.cpp instructioninfo.hpp platform.hpp log.cpp log.hpp
OFILES=lib.o program.o main.o log.o instructionset.o instructioninfo.o opcode.o block.o instruction.o platform.o

pda: Makefile $(OFILES)
	$(CC) $(CFLAGS) -o pda $(OFILES)
	echo -n `pwd`"/" > path
	# PDA succesfully compiled. #

main.o: Makefile main.cpp instructionset.hpp log.hpp program.hpp lib.hpp
	$(CC) $(CFLAGS) -c main.cpp

log.o: Makefile log.cpp log.hpp lib.hpp
	$(CC) $(CFLAGS) -c log.cpp

lib.o: Makefile lib.cpp lib.hpp log.hpp
	$(CC) $(CFLAGS) -c lib.cpp

instructioninfo.o: Makefile instructioninfo.cpp instructioninfo.hpp platform.hpp opcode.hpp
	$(CC) $(CFLAGS) -c instructioninfo.cpp

instructionset.o: Makefile instructionset.cpp instructionset.hpp opcode.hpp lib.hpp instructioninfo.hpp
	$(CC) $(CFLAGS) -c instructionset.cpp

opcode.o: Makefile opcode.cpp opcode.hpp platform.hpp
	$(CC) $(CFLAGS) -c opcode.cpp

code.hpp: platform.hpp log.hpp 


program.o: Makefile program.cpp program.hpp code.hpp platform.hpp
	$(CC) $(CFLAGS) -c program.cpp

block.o: Makefile block.cpp block.hpp code.hpp
	$(CC) $(CFLAGS) -c block.cpp

instruction.o: Makefile instruction.cpp instruction.hpp code.hpp instructionset.hpp log.hpp
	$(CC) $(CFLAGS) -c instruction.cpp

platform.o: Makefile platform.cpp platform.hpp
	$(CC) $(CFLAGS) -c platform.cpp

clear: clean
	rm -f pda

clean:
	rm -f *~
	rm -f *.o 2> /dev/null
