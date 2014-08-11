.PHONY:clean   #伪脚本
CC=g++
CFLAGS=-Wall -g
BIN=a.out
OBJS=Framework.o TextQuery.o main.o Config.o
STARD=-std=c++0x -rdynamic -Wno-deprecated
LINKS=-lpthread -lecho
$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LINKS) -Iinclude
%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(STARD) -Iinlcude
clean:
	rm -rf *.o $(BIN) 
