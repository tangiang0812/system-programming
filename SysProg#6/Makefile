CC = gcc

all: server client

server: server.c
	$(CC) server.c -lpthread -o server

client: client.c
	$(CC) client.c -o client

clean:
	rm client server

.PHONY: clean all
