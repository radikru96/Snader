int readvrec( SOCKET fd, char *bp, size_t len )
{
	u_int32_t reclen;
	int rc;
	/* Прочитать длину записи. */
	rc = readn( fd, ( char * )&reclen, sizeof( u_int32_t ) );
	if ( rc != sizeof( u_int32_t ) )
		return rc < 0 ? -1 : 0;
	reclen = ntohl( reclen );
	if ( reclen > len )
	{
		/*
		 *  Не хватает места в буфере для размещения данных -
		 *  отбросить их и вернуть код ошибки.
		 */
		while ( reclen > 0 )
		{
			rc = readn( fd, bp, len );
			if ( rc != len )
				return rc < 0 ? -1 : 0;
			reclen -= len;
			if ( reclen < len )
				len = reclen;
		}
		set_errno( EMSGSIZE );
		return -1;
	}
	/* Прочитать саму запись */
	rc = readn( fd, bp, reclen );
	if ( rc != reclen )
		return rc < 0 ? -1 : 0;
	return rc;
}