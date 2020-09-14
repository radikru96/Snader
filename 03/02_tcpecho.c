#include "etcp.h"
int main( int argc, char **argv )
{
	SOCKET s;
	SOCKET s1;
	char buf[ 1024 ];
	int rc;
	int nap = 0;
	INIT();
	if ( argc == 3 )
		nap = atoi( argv[ 2 ] );
	s = tcp_server( NULL, argv[ 1 ] );
	s1 = accept( s, NULL, NULL );
	if ( !isvalidsock( s1 ) )
		error( 1, errno, "ошибка вызова accept" );
	signal( SIGPIPE, SIG_IGN ); /* Игнорировать сигнал SIGPIPE. */
	for ( ;; )
	{
		rc = recv( s1, buf, sizeof( buf ), 0 );
		if ( rc == 0 )
			error( 1, 0, "клиент отсоединился\n" );
		if ( rc < 0 )
			error( 1, errno, "ошибка вызова recv" );
		if ( nap )
			sleep( nap );
		rc = send( s1, buf, rc, 0 );
		if ( rc < 0 )
			error( 1, errno, "ошибка вызова send" );
	}
}