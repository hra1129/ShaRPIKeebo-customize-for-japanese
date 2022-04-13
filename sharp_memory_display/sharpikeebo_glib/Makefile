CFLAGS=-c -Wall -O2 -DSPI_BUS_NUMBER=0 -I.
LIBS = -L. -lsharpikeebo_glib -pthread -larmbianio -lm -lpulse -lpulse-simple
all: spk_demo game_demo rotate_demo sound_demo psg_test pulse_audio_test

###############################################################################
#  build for library
###############################################################################
libsharpikeebo_glib.a: sharpikeebo_glib.o sharpikeebo_slib.o psg_emulator.o scc_emulator.o
	ar rcs libsharpikeebo_glib.a sharpikeebo_glib.o sharpikeebo_slib.o psg_emulator.o scc_emulator.o

sharpikeebo_glib.o: sharpikeebo_glib.c sharpikeebo_glib.h
	$(CC) $(CFLAGS) sharpikeebo_glib.c -o sharpikeebo_glib.o

sharpikeebo_slib.o: sharpikeebo_slib.c sharpikeebo_slib.h
	$(CC) $(CFLAGS) sharpikeebo_slib.c -o sharpikeebo_slib.o

psg_emulator.o: psg_emulator.c psg_emulator.h
	$(CC) $(CFLAGS) psg_emulator.c -o psg_emulator.o

scc_emulator.o: scc_emulator.c scc_emulator.h
	$(CC) $(CFLAGS) scc_emulator.c -o scc_emulator.o

###############################################################################
#  build for spk_demo
###############################################################################
spk_demo: libsharpikeebo_glib.a sample/spk_demo.o sample/startup_logo.o sample/usa.o
	$(CC) sample/spk_demo.o sample/startup_logo.o sample/usa.o $(LIBS) -o spk_demo

spk_demo.o: sharpikeebo_glib.h sample/spk_demo.c sample/startup_logo.h sample/usa.h
	$(CC) $(CFLAGS) sample/spk_demo.c -o sample/spk_demo.o

sample/startup_logo.o : sample/startup_logo.c sample/startup_logo.h
	$(CC) $(CFLAGS) sample/startup_logo.c -o sample/startup_logo.o

sample/startup_logo.h : sample/startup_logo.png image_converter.py
	./image_converter.py sample/startup_logo.png sample/startup_logo

sample/usa.o : sample/usa.c sample/usa.h
	$(CC) $(CFLAGS) sample/usa.c -o sample/usa.o

sample/usa.h : sample/usa.png image_converter.py
	./image_converter.py sample/usa.png sample/usa

###############################################################################
#  build for game_demo
###############################################################################
game_demo: libsharpikeebo_glib.a sample/game_demo.o sample/game.o
	$(CC) sample/game_demo.o sample/game.o $(LIBS) -o game_demo

sample/game_demo.o: sharpikeebo_glib.h sample/game_demo.c sample/game.h
	$(CC) $(CFLAGS) sample/game_demo.c -o sample/game_demo.o

sample/game.o : sample/game.c sample/game.h
	$(CC) $(CFLAGS) sample/game.c -o sample/game.o

sample/game.h : sample/game.png image_converter.py
	./image_converter.py sample/game.png sample/game

###############################################################################
#  build for rotate_demo
###############################################################################
rotate_demo: libsharpikeebo_glib.a sample/rotate_demo.o sample/startup_logo.o
	$(CC) sample/rotate_demo.o sample/startup_logo.o $(LIBS) -o rotate_demo

sample/rotate_demo.o: sharpikeebo_glib.h sample/rotate_demo.c sample/startup_logo.h
	$(CC) $(CFLAGS) sample/rotate_demo.c -o sample/rotate_demo.o

###############################################################################
#  build for sound_demo
###############################################################################
sound_demo: libsharpikeebo_glib.a sharpikeebo_slib.o sample/sound_demo.o
	$(CC) sharpikeebo_slib.o sample/sound_demo.o $(LIBS) -o sound_demo

sample/sound_demo.o: sharpikeebo_glib.h sharpikeebo_slib.h sample/sound_demo.c
	$(CC) $(CFLAGS) sample/sound_demo.c -o sample/sound_demo.o

###############################################################################
#  test
###############################################################################
psg_test: libsharpikeebo_glib.a test/psg_test.o
	$(CC) test/psg_test.o $(LIBS) -o psg_test

test/psg_test.o: sharpikeebo_slib.h test/psg_test.c
	$(CC) $(CFLAGS) test/psg_test.c -o test/psg_test.o

pulse_audio_test: test/pulse_audio_test.o
	$(CC) test/pulse_audio_test.o $(LIBS) -o pulse_audio_test

test/pulse_audio_test.o: test/pulse_audio_test.c
	$(CC) $(CFLAGS) test/pulse_audio_test.c -o test/pulse_audio_test.o

###############################################################################
#  clean
###############################################################################
clean:
	rm -rf *.o sample/*.o test/*.o spk_demo game_demo rotate_demo sound_demo psg_test
