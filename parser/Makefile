OBJ = GEDCOMparser.o GEDCOMutilities.o LinkedListAPI.o
INC_DIR = include
CC = gcc
CFLAGS = -Wall -std=c11 -g -I$(INC_DIR) 

#UNAME Shell Variable
UNAME_S := $(shell uname -s)

# OS X Dependant Variables
ifeq ($(UNAME_S), Darwin)
    LIB := libparser.dylib
# GNU/Linux Depedant Variables
else ifeq ($(UNAME_S), Linux)
    LIB := libparser.so
endif

$(LIB): $(OBJ)
	$(CC) -shared -o $(LIB) $(OBJ)
	mv libparser.so ../

LinkedListAPI.o: src/LinkedListAPI.c
	$(CC) $(CFLAGS) -c -fpic src/LinkedListAPI.c

GEDCOMparser.o: src/GEDCOMparser.c
	$(CC) $(CFLAGS) -c -fpic src/GEDCOMparser.c 

GEDCOMutilities.o: src/GEDCOMutilities.c
	$(CC) $(CFLAGS) -c -fpic src/GEDCOMutilities.c

clean:
	rm -rf bin/*.a obj *.o vgcore.* ../$(LIB) *.o
