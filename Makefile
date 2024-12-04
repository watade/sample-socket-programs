CC := gcc
RM := rm
CFLAGS := -Wall -O2 
PRG1 := tcp_server
OBJS1 := tcp_server.o
PRG2 := tcp_client
OBJS2 := tcp_client.o
PRG3 := multi_tcp_server
OBJS3 := multi_tcp_server.o utils.o

all: $(PRG1) $(PRG2) $(PRG3)

$(PRG1): $(OBJS1)
	$(CC) $^ $(CFLAGS) -o $@

$(PRG2): $(OBJS2)
	$(CC) $^ $(CFLAGS) -o $@

$(PRG3): $(OBJS3)
	$(CC) $^ $(CFLAGS) -o $@

clean:; $(RM) -f *.o $(PRG1) $(PRG2) $(PRG3)