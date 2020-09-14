int isconnected( SOCKET s, fd_set *rd, fd_set *wr, fd_set *ex )
{
	WSASetLastError( 0 );
	if ( !FD_ISSET( s, rd ) && !FD_ISSET( s, wr ) )
		return 0;
	if ( FD_ISSET( s, ex ) )
		return 0;
	return 1;
}