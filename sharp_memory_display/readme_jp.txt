SharpMemoryDisplay にターミナルの内容を表示する
-------------------------------------------------------------------------------
1. はじめに
	ShaRPIKeebo には、SharpMemoryDisplay を接続できます。

	https://ja.aliexpress.com/item/4000388720940.html?gatewayAdapt=glo2jpn&spm=a2g0o.new_account_index.0.0.b803429bstofRQ&gps-id=pcAccount&scm=1007.36264.271383.0&scm_id=1007.36264.271383.0&scm-url=1007.36264.271383.0&pvid=878723cf-f182-4217-af85-68a78912f9c0&_t=gps-id:pcAccount,scm-url:1007.36264.271383.0,pvid:878723cf-f182-4217-af85-68a78912f9c0,tpp_buckets:668%232846%238115%232000&pdp_ext_f=%257B%2522sku_id%2522%253A%252210000001588574748%2522%252C%2522sceneId%2522%253A%252226264%2522%257D&pdp_pi=-1%253B2699.0%253B-1%253B-1%2540salePrice%253BJPY%253Brecommend-recommend

	しかし、これはSPIでデータを送り込まねば表示できないタイプのディスプレイです。
	フレームバッファの内容を、この SharpMemoryDisplay に転送するプログラム sharpikeebo_lcd
	です。

	この sharpikeebo_lcd を起動時にサービスとして常駐させて、裏で動かしておくと、
	ターミナルへの表示内容がそのまま SharpMemoryDisplay に表示されるようになります。

2. 設定方法
	(1) ここに置いてある sharpikeebo_lcd.service を /etc/systemd/system/ にコピーする。
		$ sudo cp ./sharpikeebo_lcd/sharpikeebo_lcd /usr/local/bin/
		$ sudo cp ./sharpikeebo_lcd.service /etc/systemd/system/

	(2) /etc/systemd/system/sharpikeebo_lcd.service に実行属性を与える
		$ sudo chmod 755 /etc/systemd/system/sharpikeebo_lcd.service

	(3) sharpikeebo_lcdサービスを有効化して起動する
		$ sudo systemctl enable sharpikeebo_lcd
		$ sudo systemctl start sharpikeebo_lcd

3. オプション指定
	sharpikeebo_lcd.service の下記の部分にオプションを指定できます。

		ExecStart=/usr/local/bin/sharpikeebo_lcd

	オプションは下記のものが指定できます。
	-nosplash ....... 起動時にロゴを表示しない
	-invert ......... ターミナルを反転表示する
	-noblink ........ 点滅3値表示をやめて、点滅しない2値表示にする
	-threshold xxx .. 二値化の閾値

	例)
		ExecStart=/usr/local/bin/sharpikeebo_lcd -noblink -invert

4. 操作

	                          [B]
	      [U]         (P)(P)         [ ]
	[L]         [R]   ( )( )   [ ]         [ ]
	      [D]                        [ ]

	[B] ............. ズーム倍率の変更 ( x1 → x2 → x4 → x8 → x1 )
	[U][L][R][D] .... スクロール位置移動
	(P) ............. Power LED
