#!/usr/bin/python3
# -----------------------------------------------------------------------------
#  Image converter
#
# require:
#	python3
#	pip3 install Pillow
#	sudo apt-get install libopenjp2-7
# -----------------------------------------------------------------------------

from PIL import Image
import sys

# -----------------------------------------------------------------------------
def usage():
	print( "Usage: %s <input.png> <output>" % sys.argv[0] )
	print( "  <output>.c and <output>.h are generated." )
	print( "  It is the definition of an array." )
	print( "  The array variable name is <input>." )

# -----------------------------------------------------------------------------
def main():
	im = Image.open( sys.argv[1] )
	( width, height ) = im.size
	rgb = im.convert( 'RGB' )

	a_path = sys.argv[1].split( '/' )
	a_full_name = a_path[-1].split( '.' )
	s_name = a_full_name[0]
	print( "%s ( %d x %d )" % ( s_name, width, height ) )

	byte_count = 0
	f = open( "%s.c" % sys.argv[2], "w" )
	f.write( "// --------------------------------------------------------------------\n" )
	f.write( "//  Graphic data: [%s]\n" % a_path[-1] )
	f.write( "// --------------------------------------------------------------------\n" )
	f.write( "\n" )
	f.write( "#include <stdint.h>\n" )
	f.write( "\n" )
	f.write( "typedef struct {\n" )
	f.write( "\tuint32_t\twidth;\n" )
	f.write( "\tuint32_t\theight;\n" )
	f.write( "\tuint8_t\t\timage[%d];\n" % ( width * height ) )
	f.write( "} _BACKBUFFER_T;\n" )
	f.write( "\n" )
	f.write( "const _BACKBUFFER_T _%s = {\n" % sys.argv[2] )
	f.write( "\t%d, %d, {\n" % ( width, height ) )
	f.write( "\t\t" )
	for y in range( 0, height ):
		for x in range( 0, width ):
			( r, g, b ) = rgb.getpixel( ( x, y ) )
			if ( b > r * 2 ) or ( g > b * 2 ) or ( r > b * 2 ):
				d = 0		# transparent
			elif ( r + g + b ) > ( 128 * 3 ):
				d = 1		# white
			else:
				d = 255		# black
			f.write( "0x%02X, " % ( d ) )
			byte_count = byte_count + 1
			if  byte_count >= 16:
				f.write( "\n\t\t" )
				byte_count = 0
	f.write( "\n\t},\n" )
	f.write( "};\n" )
	f.close()

	f = open( "%s.h" % sys.argv[2], "w" )
	f.write( "// --------------------------------------------------------------------\n" )
	f.write( "//  Graphic data: [%s]\n" % a_path[-1] )
	f.write( "// --------------------------------------------------------------------\n" )
	f.write( "\n" )
	f.write( "#ifndef __%s_H__\n" % sys.argv[2].upper() )
	f.write( "#define __%s_H__\n" % sys.argv[2].upper() )
	f.write( "\n" )
	f.write( "#include \"sharpikeebo_glib.h\"\n" )
	f.write( "\n" )
	f.write( "extern const struct _BACKBUFFER_T _%s;\n" % sys.argv[2] )
	f.write( "#define p_%s ((GLIB_BACKBUFFER_T*) &_%s)\n" % ( sys.argv[2], sys.argv[2] ) )
	f.write( "\n" )
	f.write( "#endif // __%s_H__\n" % sys.argv[2].upper() )
	f.close()

	print( "Generated %s.c and %s.h" % ( sys.argv[2], sys.argv[2] ) )

# -----------------------------------------------------------------------------
if __name__ == "__main__":
	print( "Image converter for sharpikeebo_glib" )
	print( "========================================" )
	print( "2022 t.hara" )
	print( "" )
	if len( sys.argv ) < 3:
		usage()
	else:
		main()
