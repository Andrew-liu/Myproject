.PHONY:clean
CC=g++
CFLAGS=-Wall -g
BIN=test
OBJS=Socket.o Rio.o TcpConnection.o PollPoller.o test_echo_server.o InetAddress.o TcpServer.o
STARD=-std=c++0x
LINKS=-lpthread
$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LINKS)
%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(STARD)
clean:
	rm -f *.o $(BIN)
