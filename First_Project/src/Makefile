.PHONY:clean
CC=g++
CFLAGS=-Wall -g
BIN=a.out
OBJS=Framework.o TextQuery.o main.o Config.o Time_Thread.o
STARD=-std=c++0x -rdynamic -Wno-deprecated
LINKS=-lpthread -lecho
$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LINKS)
%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(STARD)
clean:
	rm -rf *.o $(BIN) 
