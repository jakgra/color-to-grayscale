#include <jak_dbg.h>
#include <bstrlib.h>




bstring get_grayscale( char * in, int len ) {

	bstring s;


	s = bfromcstr( "aaccaa" );
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
					r = get_grayscale( b->data + j, 3 );
				} else if( j < b->slen - 7 && *( b->data + j + 7 ) == ';' ) {
					// 6 chars hex ex.: #ac83e4;
					len = 6;
					r = get_grayscale( b->data + j, 6 );
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
