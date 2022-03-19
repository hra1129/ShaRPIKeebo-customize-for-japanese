CFLAGS=-c -Wall -O2 -DSPI_BUS_NUMBER=0
LIBS = -lpthread -larmbianio
all: sharpikeebo_lcd

sharpikeebo_lcd: sharpikeebo_lcd.o sharpikeebo_effects.o sharp_memory_display_driver.o fb_convert.o
	$(CC) sharpikeebo_lcd.o sharpikeebo_effects.o sharp_memory_display_driver.o fb_convert.o $(LIBS) -o sharpikeebo_lcd

startup_logo.txt: startup_logo.png
	./startup_logo.py

sharpikeebo_effects.o: sharpikeebo_effects.c sharpikeebo_effects.h startup_logo.txt sharp_memory_display_driver.h
	$(CC) $(CFLAGS) sharpikeebo_effects.c

sharp_memory_display_driver.o: sharp_memory_display_driver.c sharp_memory_display_driver.h
	$(CC) $(CFLAGS) sharp_memory_display_driver.c

fb_convert.o: fb_convert.c fb_convert.h
	$(CC) $(CFLAGS) fb_convert.c

sharpikeebo_lcd.o: sharpikeebo_lcd.c sharpikeebo_effects.h sharp_memory_display_driver.h fb_convert.h
	$(CC) $(CFLAGS) sharpikeebo_lcd.c

clean:
	rm -rf *.o sharpikeebo_lcd
