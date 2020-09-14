int main( int argc, char **argv )
{
	struct sockaddr_in local;
	struct sockaddr_in peer;
	int peerlen;
	SOCKET s1;
	SOCKET s;
	int c;
	int rcvbufsz = 32 * 1024;
	const int on = 1;
	INIT();
	opterr = 0;
	while ( ( c = getopt( argc, argv, "b:" ) ) != EOF )
	{
		switch ( c )
		{
			case "b" :
				rcvbufsz = atoi( optarg );
				break;
			case "?" :
				error( 1, 0, "недопустимая опция: %c\n", c );
		}
	}
	set_address( NULL, "9000", &local, "tcp" );
	s = socket( AF_INET, SOCK_STREAM, 0 );
	if ( !isvalidsock( s ) )
		error( 1, errno, "ошибка вызова socket" );
	if ( setsockopt( s, SOL_SOCKET, SO_REUSEADDR,
		( char * )&on, sizeof( on ) ) )
		error( 1, errno, "ошибка вызова setsockopt SO_REUSEADDR" );
	if ( setsockopt( s, SOL_SOCKET, SO_RCVBUF,
		( char * )&rcvbufsz, sizeof( rcvbufsz ) ) )
		error( 1, errno, "ошибка вызова setsockopt SO_RCVBUF" );
	if ( bind( s, ( struct sockaddr * ) &local,
		 sizeof( local ) ) )
		error( 1, errno, "ошибка вызова bind" );
	listen( s, 5 );
	do
	{
		peerlen = sizeof( peer );
		s1 = accept( s, ( struct sockaddr * )&peer, &peerlen );
		if ( !isvalidsock( s1 ) )
			error( 1, errno, "ошибка вызова accept" );
		server( s1, rcvbufsz );
		CLOSE( s1 );
	} while ( 0 );
	EXIT( 0 );
}