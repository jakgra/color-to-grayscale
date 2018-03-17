#include <jak_dbg.h>
#include <bstrlib.h>
#include <stdlib.h>
#include <math.h>




bstring get_grayscale( char * in, int len ) {

	bstring s;
	int rgb[3];
	double g;
	double b;
	int gray;
	char buf[3];
	char * end;
	int i;
	int l;
	int j;


	l = ( len == 6 );
	buf[2] = '\0';
	for( i = 0; i < 3; i++ ) {
		j = i * ( l + 1 );
		buf[0] = *( in + j );
		if( l ) {
			buf[1] = *( in + j + 1 );
		} else {
			// #aaa == #aaaaaa and not #a0a0a0 or so people say online
			buf[1] = buf[0];
		}
		end = NULL;
		rgb[i] = (int)strtol( buf, &end, 16 );
		if( ! ( end == &buf[2] && rgb[i] >= 0 && rgb[i] <= 255 ) ) {
			return NULL;
		}
	}
	// These color weights are taken from wikipedia
	gray = lround( (double)rgb[0] * 0.299 + (double)rgb[1] * 0.587 + (double)rgb[2] * 0.114 );
	check( gray >= 0 && gray <= 255, final_cleanup );
	s = bformat( "%02x%02x%02x", gray, gray, gray );
	check( s, final_cleanup );
	return s;

final_cleanup:
	return NULL;

}

int main( int argc, char * * argv ) {

	int rc;

	int i;
	FILE * fp;
	bstring b;
	bstring r;
	int j;
	int len;


	r = NULL;
	for( i = 1; i < argc; i++ ) {
		fp = fopen( argv[i], "rb" );
		check( fp, final_cleanup );
		b = bread( (bNread)fread, fp );
		fclose( fp );
		check( b, final_cleanup );
		for( j = 0; j < b->slen - 4; j++ ) {
			if( *( b->data + j ) == '#' ) {
				if( *( b->data + j + 4 ) == ';' ) {
					// 3 chars hex ex.: #a8e;
					len = 3;
					r = get_grayscale( b->data + j + 1, 3 );
				} else if( j < b->slen - 7 && *( b->data + j + 7 ) == ';' ) {
					// 6 chars hex ex.: #ac83e4;
					len = 6;
					r = get_grayscale( b->data + j + 1, 6 );
				}
				if( r ) {
					if( len == 3 ) {
						rc = binsertch( b, j + 1, 3, 'f' );
						check( rc == BSTR_OK, r_cleanup );
					}
					breplace( b, j + 1, 6, r, 'f' );
					bdestroy( r );
					r = NULL;
				}
			}
		}
		fp = fopen( argv[i], "wb" );
		check( fp, final_cleanup );
		rc = fwrite( b->data, b->slen, 1, fp );
		fclose( fp );
		check( rc == 1, final_cleanup );
	}
	return 0;

r_cleanup:
	bdestroy( r );
	bdestroy( b );
final_cleanup:
	return -1;

}
