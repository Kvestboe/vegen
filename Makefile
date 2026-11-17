CC = gcc
CFLAGS = -O2 -Wall -Wextra -Wno-unused-result -std=gnu11

OBJ = main.o node-reader.o heap.o dijkstra.o alt.o map.o

vegen: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

%.o: %.c codec.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o vegen rute.txt rute.html poi.html
