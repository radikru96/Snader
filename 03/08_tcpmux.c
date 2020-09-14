static void start_server( int s )
{
	char line[ MAXLINE ];
	servtab_t *stp;
	int rc;
	static char err1[] = "-не могу прочесть имя сервиса \r\n";
	static char err2[] = "-неизвестный сервис\r\n";
	static char err3[] = "-не могу запустить сервис\r\n";
	static char ok[] = "+OK\r\n";
	rc = fork();
	if ( rc < 0 )		/* ошибка вызова fork. */
	{
		write( s, err3, sizeof( err3 ) - 1 );
		return;
	}	
	if ( rc != 0 )		/* Родитель. */
		return;
	/* Процесс-потомок. */
	CLOSE( ls );		/* Закрыть прослушивающий сокет. */
	alarm( 10 );
	rc = readcrlf( s, line, sizeof( line ) );
	alarm( 0 );
	if ( rc <= 0 )
	{
		write( s, err1, sizeof( err1 ) - 1 );
		EXIT( 1 );
	}
	for ( stp = service_table; stp->service; stp++ )
		if ( strcasecmp( line, stp->service ) == 0 )
			break;
	if ( !stp->service )
	{
		write( s, err2, sizeof( err2 ) - 1 );
		EXIT( 1 );
	}
	if ( stp->flag )
		if ( write( s, ok, sizeof( ok ) - 1 ) < 0 )
			EXIT( 1 );
	dup2( s, 0 );
	dup2( s, 1 );
	dup2( s, 2 );
	CLOSE( s );
	execv( stp->path, stp->args );
	write( 1, err3, sizeof( err3 ) - 1 );
	EXIT( 1 );
}