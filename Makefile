#adjust for your needs
CC=clang
CFLAGS=-std=c99 -Wall -pedantic -g
CC_CMD = $(CC) $(CFLAGS)

OBJ = password.o dictionary.o sysrand.o main.o

inap: $(OBJ) 
	$(CC_CMD) -o $@ $(OBJ)

debug: $(OBJ)
	$(CC_CMD) -o inap $(OBJ) -g -fsanitize=address

password.o: src/password.c src/password.h
	$(CC_CMD) -c $<

dictionary.o: src/dictionary.c src/dictionary.h
	$(CC_CMD) -c $<

sysrand.o: src/sysrand.c src/sysrand.h
	$(CC_CMD) -c $<

main.o: src/main.c
	$(CC_CMD) -c $<

.PHONY: clean
clean:
	rm -f inap
	rm -f $(OBJ)
