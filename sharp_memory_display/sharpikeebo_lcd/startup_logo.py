#!/usr/bin/python3
# -----------------------------------------------------------------------------
#  Start up logo image converter
#
# require:
#   pip3 install Pillow
#	sudo apt-get install libopenjp2-7
# -----------------------------------------------------------------------------

from PIL import Image

# -----------------------------------------------------------------------------
def main():
	im = Image.open( "startup_logo.png" )
	rgb = im.convert( 'RGB' )

	byte_count = 0
	f = open( "startup_logo.txt", "w" )
	f.write( "\t" )
	for y in range( 0, 240 ):
		for x in range( 0, 50 ):
			d = 0
			for bit in range( 0, 8 ):
				d = d << 1
				( r, g, b ) = rgb.getpixel( ( x * 8 + bit, y ) )
				if ( r + g + b ) > ( 128 * 3 ):
					d = d | 1
			f.write( "0x%02X, " % ( d ) )
			byte_count = byte_count + 1
			if  byte_count >= 16:
				f.write( "\n\t" )
				byte_count = 0
	f.close()

# -----------------------------------------------------------------------------
if __name__ == "__main__":
	main()
