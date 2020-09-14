#include "etcp.h"
#define COOKIESZ	4	/* Так установлено клиентом. */
int main( int argc, char **argv )
{
	SOCKET s;
	SOCKET s1;
	int rc;
	char buf[ 128 ];
	INIT();
	s = tcp_server( NULL, argv[ 1 ] );
	s1 = accept( s, NULL, NULL );
	if ( !isvalidsock( s1 ) )
		error( 1, errno, "ошибка вызова accept" );
	srand( 127 );
	for ( ;; )
	{
		rc = readvrec( s1, buf, sizeof( buf ) );
		if ( rc == 0 )
			error( 1, 0, "клиент отсоединился\n" );
		if ( rc < 0 )
			error( 1, errno, "ошибка вызова recv" );
		if ( rand() % 100 < 33 )
			continue;
		write( 1, buf + COOKIESZ, rc - COOKIESZ );
		memmove( buf + 1, buf, COOKIESZ );
		buf[ 0 ] = '\006';
		if ( send( s1, buf, 1 + COOKIESZ, 0 ) < 0 )
			error( 1, errno, "ошибка вызова send" );
	}
}