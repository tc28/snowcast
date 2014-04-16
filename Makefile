CC = gcc
CFLAGS = -Wall -Werror -Wextra -Wunused
CFLAGS += -std=c99 -g -O2
OBJS = snowcast.c

client: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o snowcast_client

clean:
	rm -rvf *.o client
