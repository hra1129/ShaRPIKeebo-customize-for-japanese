ここでは、キーボードをカスタマイズして日本語キーボードに変更する方法をまとめておきます。

1. PC に USBキーボードとして認識させる
	Keeboの On/Off を何らかの手段で接続して下さい。(connect.jpg)

	次に Keeboのキーボード面に向かって右側側面に付いている USB-Cポートに USBケーブルを接続し、
	そのケーブルの反対側を PC に接続します。(front_view.jpg)

	この時点で、USBキーボードとして認識します。

2. QMK Toolbox をインストールする
	下記の場所にインストーラーが置いてあるので、ダウンロード＆インストール。
	https://github.com/qmk/qmk_toolbox/releases

3. ファームウェアを書き込む
	1. sharpikeebo (RasPiなし) の On/Off と書かれたシルクのすぐしたにある2つのスルーホールを接続
	2. PCのUSB端子と sharpikeebo 右サイドの USB-C端子を接続
	3. PCで sharpikeebo がキーボードとして認識していることを確認 (メモ帳などに入力してみれば良い)
	4. QMK Toolbox を起動
	5. Local file で qmk_firmware\sharpikeebo_jp.hex を選択
	6. MCU (AVR only) で atmega32u4 を選択
	7. Auto-Flash にチェックを入れる
	8. sharpikeebo の裏側、atmega32u4 の左隣にある 2x3 の端子の下2つを一瞬ショートさせる
	   (これがリセットの意味になる)

				 . .
				 . .
				(. .) ←この2つ (RST, GND の端子)

	9. QMK Toolbox が自動的に認識して hexファイルを書き込むので完了を待つ
	10. USBケーブルを外して繋ぎ直す
	11. PC上でキーボードと認識され、新しいキーマップになっていることを確認する

4. 自分でキーマップを作る場合 (オプション)
	1. QMK をセットアップする
		下記のサイトにセットアップの方法が記載されているので、それに従って WSL上に環境を構築する。
		https://docs.qmk.fm/#/newbs_getting_started

	2. デフォルトキーマップをコピーする
