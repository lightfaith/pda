test: test.asm
	nasm -f elf test.asm
	ld -o test test.o

clean:
	rm -f hex plain test.o test 2> /dev/null
