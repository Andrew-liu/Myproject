.PHONY:clean
CC=g++
CFLAGS=-Wall -g
BIN=a.out
OBJS=Socket.o Rio.o TcpConnection.o Poll_t.o InetAddress.o TcpSever.o Thread.o Exception.o  ./Test/test_tcpsever.cpp
STARD=-std=c++0x -rdynamic
LINKS=-lpthread
$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LINKS)
%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(STARD)
clean:
	rm -rf *.o $(BIN) 
