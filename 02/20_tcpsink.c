static void server( SOCKET s, int rcvbufsz )
{
	char *buf;
	int rc;
	int bytes = 0;
	if ( ( buf = malloc( rcvbufsz ) ) == NULL )
		error( 1, 0, "ошибка вызова malloc\n" );
	for ( ;; )
	{
		rc = recv( s, buf, rcvbufsz, 0 );
		if ( rc <= 0 )
			break;
	bytes += rc;
}
error( 0, 0, "получено байт: %d\n", bytes );
}