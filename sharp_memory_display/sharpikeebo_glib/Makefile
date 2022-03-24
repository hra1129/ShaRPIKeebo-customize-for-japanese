CFLAGS=-c -Wall -O2 -DSPI_BUS_NUMBER=0
LIBS = -pthread -larmbianio
all: glib_demo

glib_demo: sharpikeebo_glib.o glib_demo.o
	$(CC) glib_demo.o sharpikeebo_glib.o $(LIBS) -o glib_demo

glib_demo.o: sharpikeebo_glib.c sharpikeebo_glib.h glib_demo.c
	$(CC) $(CFLAGS) glib_demo.c

sharpikeebo_glib.o: sharpikeebo_glib.c sharpikeebo_glib.h
	$(CC) $(CFLAGS) sharpikeebo_glib.c

clean:
	rm -rf *.o glib_demo
