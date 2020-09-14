int isconnected( SOCKET s, fd_set *rd, fd_set *wr, fd_set *ex )
{
	int err;
	int len = sizeof( err );
	errno = 0;			/* Предполагаем, что ошибки нет. */
	if ( !FD_ISSET( s, rd ) && !FD_ISSET( s, wr ) )
		return 0;
	if ( getsockopt( s, SOL_SOCKET, SO_ERROR, &err, &len ) < 0 )
		return 0;
	errno = err;		/* Если мы не соединились. */
	return err == 0;
}