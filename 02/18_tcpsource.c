int main( int argc, char **argv )
{
	struct sockaddr_in peer;
	char *buf;
	SOCKET s;
	int c;
	int blks = 5000;
	int sndbufsz = 32 * 1024;
	int sndsz = 1440;	/* MSS для Ethernet по умолчанию. */
	INIT();
	opterr = 0;
	while ( ( c = getopt( argc, argv, "s:b:c:" ) ) != EOF )
	{
		switch ( c )
		{
			case "s" :
				sndsz = atoi( optarg );
				break;
			case "b" :
				sndbufsz = atoi( optarg );
				break;
			case "c" :
				blks = atoi( optarg );
				break;
			case "?" :
				error( 1, 0, "некорректный параметр: %c\n", c );
		}
	}
	if ( argc <= optind )
		error( 1, 0, "не задано имя хоста\n" );
	if ( ( buf = malloc( sndsz ) ) == NULL )
		error( 1, 0, "ошибка вызова malloc\n" );
	set_address( argv[ optind ], "9000", &peer, "tcp" );
	s = socket( AF_INET, SOCK_STREAM, 0 );
	if ( !isvalidsock( s ) )
		error( 1, errno, "ошибка вызова socket" );
	if ( setsockopt( s, SOL_SOCKET, SO_SNDBUF,
		( char * )&sndbufsz, sizeof( sndbufsz ) ) )
		error( 1, errno, "ошибка вызова setsockopt с опцией	SO_SNDBUF" );
	if ( connect( s, ( struct sockaddr * )&peer,
		 sizeof( peer ) ) )
		error( 1, errno, "ошибка вызова connect" );
	while( blks-- > 0 )
		send( s, buf, sndsz, 0 );
	EXIT( 0 );
}