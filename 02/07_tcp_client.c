SOCKET tcp_client( char *hname, char *sname )
{
	struct sockaddr_in peer;
	SOCKET s;
	set_address( hname, sname, &peer, "tcp" );
	s = socket( AF_INET, SOCK_STREAM, 0 );
	if ( !isvalidsock( s ) )
		error( 1, errno, "ошибка вызова socket" );
	if ( connect( s, ( struct sockaddr * )&peer,
		 sizeof( peer ) ) )
		error( 1, errno, "ошибка вызова connect" );
	return s;
}