SharpMemoryDisplay �Ƀ^�[�~�i���̓��e��\������
-------------------------------------------------------------------------------
1. �͂��߂�
	ShaRPIKeebo �ɂ́ASharpMemoryDisplay ��ڑ��ł��܂��B

	https://ja.aliexpress.com/item/4000388720940.html?gatewayAdapt=glo2jpn&spm=a2g0o.new_account_index.0.0.b803429bstofRQ&gps-id=pcAccount&scm=1007.36264.271383.0&scm_id=1007.36264.271383.0&scm-url=1007.36264.271383.0&pvid=878723cf-f182-4217-af85-68a78912f9c0&_t=gps-id:pcAccount,scm-url:1007.36264.271383.0,pvid:878723cf-f182-4217-af85-68a78912f9c0,tpp_buckets:668%232846%238115%232000&pdp_ext_f=%257B%2522sku_id%2522%253A%252210000001588574748%2522%252C%2522sceneId%2522%253A%252226264%2522%257D&pdp_pi=-1%253B2699.0%253B-1%253B-1%2540salePrice%253BJPY%253Brecommend-recommend

	�������A�����SPI�Ńf�[�^�𑗂荞�܂˂Ε\���ł��Ȃ��^�C�v�̃f�B�X�v���C�ł��B
	�t���[���o�b�t�@�̓��e���A���� SharpMemoryDisplay �ɓ]������v���O���� sharpikeebo_lcd
	�ł��B

	���� sharpikeebo_lcd ���N�����ɃT�[�r�X�Ƃ��ď풓�����āA���œ������Ă����ƁA
	�^�[�~�i���ւ̕\�����e�����̂܂� SharpMemoryDisplay �ɕ\�������悤�ɂȂ�܂��B

2. �ݒ���@
	(1) �����ɒu���Ă��� sharpikeebo_lcd.service �� /etc/systemd/system/ �ɃR�s�[����B
		$ sudo cp ./sharpikeebo_lcd/sharpikeebo_lcd /usr/local/bin/
		$ sudo cp ./sharpikeebo_lcd.service /etc/systemd/system/

	(2) /etc/systemd/system/sharpikeebo_lcd.service �Ɏ��s������^����
		$ sudo chmod 755 /etc/systemd/system/sharpikeebo_lcd.service

	(3) sharpikeebo_lcd�T�[�r�X��L�������ċN������
		$ sudo systemctl enable sharpikeebo_lcd
		$ sudo systemctl start sharpikeebo_lcd

3. �I�v�V�����w��
	sharpikeebo_lcd.service �̉��L�̕����ɃI�v�V�������w��ł��܂��B

		ExecStart=/usr/local/bin/sharpikeebo_lcd

	�I�v�V�����͉��L�̂��̂��w��ł��܂��B
	-nosplash ....... �N�����Ƀ��S��\�����Ȃ�
	-invert ......... �^�[�~�i���𔽓]�\������
	-noblink ........ �_��3�l�\������߂āA�_�ł��Ȃ�2�l�\���ɂ���
	-threshold xxx .. ��l����臒l

	��)
		ExecStart=/usr/local/bin/sharpikeebo_lcd -noblink -invert

4. ����

	                          [B]
	      [U]         (P)(P)         [ ]
	[L]         [R]   ( )( )   [ ]         [ ]
	      [D]                        [ ]

	[B] ............. �Y�[���{���̕ύX ( x1 �� x2 �� x4 �� x8 �� x1 )
	[U][L][R][D] .... �X�N���[���ʒu�ړ�
	(P) ............. Power LED
