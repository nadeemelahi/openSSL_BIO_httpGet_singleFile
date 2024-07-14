# include <stdio.h>
# include <stdlib.h>

# include  "openssl/bio.h"
# include  "openssl/ssl.h"
# include  "openssl/err.h"

#define LIM 4096

int main ( int argc , char * argv [ ] ) {

	// https://developer.ibm.com/tutorials/l-openssl/
	printf ( "main()\n" ) ;
	int ret , len;

	SSL_load_error_strings();
	//ERR_load_BIO_strings(); // warning: depreciated since OpenSSL 3.0
	// not needed anymore
	OpenSSL_add_all_algorithms();
	printf ( "initialized openssl() \n" );

	char hostNportBuf[100];
	char *hostNport = hostNportBuf;

	sprintf( hostNport , "%s:%s" , argv[1] , argv[2] ) ;




	struct bio_st *bio;

	//bio = BIO_new_connect ( "localhost:80" ) ;
	bio = BIO_new_connect ( hostNport ) ;

	if ( bio == NULL ) {
		fprintf ( stderr , "BIO_new_connect() error" );
		exit ( 1 ) ;
	}

	printf ( "BIO_new_connect() CHECK\n" );





	ret = BIO_do_connect ( bio ) ;

	if ( ret < 1 ) {
		fprintf ( stderr , "BIO_do_connect() error" );
		exit ( 1 ) ;
	}

	printf ( "BIO_do_connect() CHECK\n" );




	char getBuf[100] = {0};
	char *get = getBuf;

	sprintf ( get , "GET / HTTP/1.1\r\nHost: %s\r\n\r\n" , argv[1] );

	len = strlen ( get ) ;

	ret =  BIO_write ( bio , get , len ); 

	if ( ret < 1 ) {
		fprintf ( stderr , "BIO_write() error" );
		exit ( 1 ) ;
	}

	printf ( "BIO_write() CHECK\n" );




	char respBuf[LIM] = {0};
	char *resp = respBuf;

	ret = BIO_read ( bio , resp , LIM );

	if ( ret == 0 ) {

		fprintf ( stderr , "BIO_read() error - closed connection" );
		exit ( 1 ) ;
	} else if ( ret < 0 ) {

		fprintf ( stderr , "BIO_read() error - failed to read" );
		exit ( 1 ) ;
	}

	printf ( "BIO_read() CHECK\n" );

	printf ( "GET: \n---\n[%s]\n---\n", get ) ;

	printf( "\n%s\n" , resp ) ;




	// finally closing the connection
	// BIO_reset ( bio ) ; // if want to re-use for a second connection
	BIO_free_all ( bio ) ;


	return 0;

}
