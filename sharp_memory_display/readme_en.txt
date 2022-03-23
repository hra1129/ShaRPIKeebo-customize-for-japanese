Display terminal contents on SharpMemoryDisplay
-------------------------------------------------------------------------------
1. Introduction
	SharpMemoryDisplay can be connected to ShaRPIKeebo.

	https://ja.aliexpress.com/item/4000388720940.html?gatewayAdapt=glo2jpn&spm=a2g0o.new_account_index.0.0.b803429bstofRQ&gps-id=pcAccount&scm=1007.36264.271383.0&scm_id=1007.36264.271383.0&scm-url=1007.36264.271383.0&pvid=878723cf-f182-4217-af85-68a78912f9c0&_t=gps-id:pcAccount,scm-url:1007.36264.271383.0,pvid:878723cf-f182-4217-af85-68a78912f9c0,tpp_buckets:668%232846%238115%232000&pdp_ext_f=%257B%2522sku_id%2522%253A%252210000001588574748%2522%252C%2522sceneId%2522%253A%252226264%2522%257D&pdp_pi=-1%253B2699.0%253B-1%253B-1%2540salePrice%253BJPY%253Brecommend-recommend

	However, this is a type of display that cannot be displayed without sending data via SPI.
	There is a program sharpikeebo_lcd that transfers the contents of the frame buffer 
	to this SharpMemoryDisplay.

	If you have this sharp_terminal resident as a service at startup and running 
	behind the scenes The contents displayed on the terminal are directly displayed on SharpMemoryDisplay.

2. Settings
	(1) Copy sharpikeebo_lcd.service placed here to /etc/systemd/system/.

		$ sudo cp ./sharpikeebo_lcd/sharpikeebo_lcd /usr/local/bin/
		$ sudo cp ./sharpikeebo_lcd.service /etc/systemd/system/

	(2) Give /etc/systemd/system/sharpikeebo_lcd.service the execute attribute

		$ sudo chmod 755 /etc/systemd/system/sharpikeebo_lcd.service

	(3) Enable and start the sharpikeebo_lcd service

		$ sudo systemctl enable sharpikeebo_lcd
		$ sudo systemctl start sharpikeebo_lcd

3. Options
	Options can be specified in the following part of sharpikeebo_lcd.service.

		ExecStart=/usr/local/bin/sharpikeebo_lcd

	The following options can be specified:
		-nosplash ....... No logo at startup
		-invert ......... Inverted terminal display
		-noblink ........ Stop blinking tri-level display and use non-blinking bi-level display
		-threshold xxx .. Binarization threshold

	ex.)
		ExecStart=/usr/local/bin/sharpikeebo_lcd -noblink -invert -threshold 100

4. Operations

	                          [B]
	      [U]         (P)(P)         [ ]
	[L]         [R]   ( )( )   [ ]         [ ]
	      [D]                        [ ]

	[B] ............. Change the magnification rate ( x1 --> x2 --> x4 --> x8 --> x1 )
	[U][L][R][D] .... Scroll position
	(P) ............. Power LEDs

5. Compile and Install
	$ make install
