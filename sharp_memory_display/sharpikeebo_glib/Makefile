CFLAGS=-c -Wall -O2 -DSPI_BUS_NUMBER=0 -I.
LIBS = -pthread -larmbianio
all: spk_demo game_demo rotate_demo

spk_demo: sharpikeebo_glib.o sample/spk_demo.o sample/startup_logo.o sample/usa.o
	$(CC) sharpikeebo_glib.o sample/spk_demo.o sample/startup_logo.o sample/usa.o $(LIBS) -o spk_demo

spk_demo.o: sharpikeebo_glib.c sharpikeebo_glib.h sample/spk_demo.c sample/startup_logo.h sample/usa.h
	$(CC) $(CFLAGS) sample/spk_demo.c -o sample/spk_demo.o

sharpikeebo_glib.o: sharpikeebo_glib.c sharpikeebo_glib.h
	$(CC) $(CFLAGS) sharpikeebo_glib.c -o sharpikeebo_glib.o

sample/startup_logo.o : sample/startup_logo.c sample/startup_logo.h
	$(CC) $(CFLAGS) sample/startup_logo.c -o sample/startup_logo.o

sample/usa.o : sample/usa.c sample/usa.h
	$(CC) $(CFLAGS) sample/usa.c -o sample/usa.o

sample/startup_logo.h : sample/startup_logo.png image_converter.py
	./image_converter.py sample/startup_logo.png sample/startup_logo

sample/usa.h : sample/usa.png image_converter.py
	./image_converter.py sample/usa.png sample/usa


game_demo: sharpikeebo_glib.o sample/game_demo.o sample/game.o
	$(CC) sharpikeebo_glib.o sample/game_demo.o sample/game.o $(LIBS) -o sample/game_demo

sample/game_demo.o: sharpikeebo_glib.c sharpikeebo_glib.h sample/game_demo.c sample/game.h
	$(CC) $(CFLAGS) sample/game_demo.c -o sample/game_demo.o

sample/game.o : sample/game.c sample/game.h
	$(CC) $(CFLAGS) sample/game.c -o sample/game.o

sample/game.h : sample/game.png image_converter.py
	./image_converter.py sample/game.png sample/game


rotate_demo: sharpikeebo_glib.o sample/rotate_demo.o sample/startup_logo.o
	$(CC) sharpikeebo_glib.o sample/rotate_demo.o sample/startup_logo.o -lm $(LIBS) -o rotate_demo

sample/rotate_demo.o: sharpikeebo_glib.c sharpikeebo_glib.h sample/rotate_demo.c sample/startup_logo.h
	$(CC) $(CFLAGS) sample/rotate_demo.c -o sample/rotate_demo.o

clean:
	rm -rf *.o sample/*.o spk_demo game_demo rotate_demo
