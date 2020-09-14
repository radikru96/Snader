void smbsend( SOCKET s, void *b )
{
	int index;
	index = ( smb_t * )b - smbarray;
	if ( send( s, ( char * )&index, sizeof( index ), 0 ) < 0 )
		error( 1, errno, "smbsend: ошибка вызова send" );
}
void *smbrecv( SOCKET s )
{
	int index;
	int rc;
	rc = readn( s, ( char * )&index, sizeof( index ) );
	if ( rc == 0 )
		error( 1, 0, "smbrecv: другой конец отсоединился\n" );
	else if ( rc != sizeof( index ) )
		error( 1, errno, "smbrecv: ошибка вызова readn" );
	return smbarray + index;
}