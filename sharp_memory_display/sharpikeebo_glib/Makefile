CFLAGS=-c -Wall -O2 -DSPI_BUS_NUMBER=0
LIBS = -pthread -larmbianio
all: spk_demo

spk_demo: sharpikeebo_glib.o spk_demo.o startup_logo.o usa.o
	$(CC) *.o $(LIBS) -o spk_demo

spk_demo.o: sharpikeebo_glib.c sharpikeebo_glib.h spk_demo.c startup_logo.h usa.h
	$(CC) $(CFLAGS) spk_demo.c

sharpikeebo_glib.o: sharpikeebo_glib.c sharpikeebo_glib.h
	$(CC) $(CFLAGS) sharpikeebo_glib.c

startup_logo.o : startup_logo.c startup_logo.h
	$(CC) $(CFLAGS) startup_logo.c

usa.o : usa.c usa.h
	$(CC) $(CFLAGS) usa.c

startup_logo.h : startup_logo.png image_converter.py
	./image_converter.py startup_logo.png startup_logo

usa.h : usa.png image_converter.py
	./image_converter.py usa.png usa

clean:
	rm -rf *.o spk_demo
