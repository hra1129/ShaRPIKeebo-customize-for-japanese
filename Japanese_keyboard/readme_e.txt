Here is a summary of how to customize the keyboard and change it to a Japanese keyboard.

1. Recognize it as a USB keyboard on your PC
	Connect Keebo On/Off by some means. (connect.jpg)

	Next, connect the USB cable to the USB-C port on the right side facing the keyboard 
	surface of the Keebo and connect the other end of the cable to the PC. (front_view.jpg)

	At this point, it is recognized as a USB keyboard.

2. Install QMK Toolbox
	Download and install the installer at the following location.
	https://github.com/qmk/qmk_toolbox/releases

3. Write firmware
	1. Connect the two through-holes just below the silk labeled On/Off in sharpikeebo (without RasPi)
	2. Connect the USB port on the PC to the USB-C port on the right side of the sharpikeebo.
	3. Check that sharpikeebo is recognized as a keyboard on your PC (you can try typing it into Notepad, etc.)
	4. Launch QMK Toolbox.
	5. Select qmk_firmware\sharpikeebo_jp.hex in "Local file".
	6. Select atmega32u4 under "MCU (AVR only)"
	7. Check Auto-Flash
	8. On the back side of the sharpikeebo, momentarily short the bottom two 2x3 terminals to the left of the atmega32u4.
	   (This would mean a reset.)

				 . .
				 . .
				(. .) <--- This pins (RST, GND pin)

	9. Wait for QMK Toolbox to automatically recognize and write the hex file and wait for completion.
	10. Disconnect and reconnect the USB cable.
	11. Confirm that the keyboard is recognized as a keyboard on the PC and is in the new keymap.

4. To create your own keymap (optional)
	1. Set up QMK
		Follow the setup instructions on the following site to build an environment on WSL.
		https://docs.qmk.fm/#/newbs_getting_started

	2. Copy the default keymap


