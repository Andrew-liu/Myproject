.PHONY:clean
CC=g++
CFLAGS=-Wall -g
BIN=test
OBJS=Socket.o Rio.o TcpConnection.o PollPoller.o test_poll2.o InetAddress.o
STARD=-std=c++0x
$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
%.o:%.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(STARD)
clean:
	rm -f *.o $(BIN)
