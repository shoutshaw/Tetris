CC=gcc
CXX=g++
CFLAGS= -g -fPIC
LIB=
INC=

OBJ+=Tetris.o

Tetris:$(OBJ)
	$(CXX) $(CFLAGS) -o $@ $^ $(LIB)

%.o: %.cpp
	$(CXX) $(CFLAGS) $(INC) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

clean:
	rm *.o
	rm Tetris
