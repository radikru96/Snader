#include "etcp.h"
int main( int argc, char **argv )
{
	SOCKET s;
	SOCKET s1;
	int rc;
	int len;
	int counter = 1;
	char buf[ 120 ];
	INIT();
	s = tcp_server( NULL, argv[ 1 ] );
	s1 = accept( s, NULL, NULL );
	if ( !isvalidsock( s1 ) )
		error( 1, errno, "ошибка вызова accept" );
	while ( ( rc = readline( s1, buf, sizeof( buf ) ) ) > 0 )
	{
		sleep( 5 );
		len = sprintf( buf, "получено сообщение %d\n", counter++ );
		rc = send( s1, buf, len, 0 );
		if ( rc < 0 )
			error( 1, errno, "ошибка вызова send" );
	}
	EXIT( 0 );
}