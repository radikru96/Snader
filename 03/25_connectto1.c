int main( int argc, char **argv )
{
	fd_set rdevents;
	fd_set wrevents;
	fd_set exevents;
	struct sockaddr_in peer;
	struct timeval tv;
	SOCKET s;
	int flags;
	int rc;
	INIT();
	set_address( argv[ 1 ], argv[ 2 ], &peer, "tcp" );
	s = socket( AF_INET, SOCK_STREAM, 0 );
	if ( !isvalidsock( s ) )
		error( 1, errno, "ошибка вызова socket " );
	if( ( flags = fcntl( s, F_GETFL, 0 ) ) < 0 )
		error( 1, errno, "ошибка вызова fcntl (F_GETFL)" );
	if ( fcntl( s, F_SETFL, flags | O_NONBLOCK ) < 0 )
		error( 1, errno, "ошибка вызова fcntl (F_SETFL)" );
	if ( ( rc = connect( s, ( struct sockaddr * )&peer,
		 sizeof( peer ) ) ) && errno != EINPROGRESS )
		error( 1, errno, "ошибка вызова connect" );
	if ( rc == 0 )	/* Уже соединён? */
	{
		if ( fcntl( s, F_SETFL, flags ) < 0 )
		error( 1, errno, "ошибка вызова fcntl (восстановление флагов)" );
		client( s, &peer );
		EXIT( 0 );
	}
	FD_ZERO( &rdevents );
	FD_SET( s, &rdevents );
	wrevents = rdevents;
	exevents = rdevents;
	tv.tv_sec = 5;
	tv.tv_usec = 0;
	rc = select( s + 1, &rdevents, &wrevents, &exevents, &tv );
	if ( rc < 0 )
		error( 1, errno, "ошибка вызова select" );
	else if ( rc == 0 )
		error( 1, 0, "истёк тайм-аут connect\n" );
	else if ( isconnected( s, &rdevents, &wrevents, &exevents ) )
	{
		if ( fcntl( s, F_SETFL, flags ) < 0 )
		error( 1, errno, "ошибка вызова fcntl (восстановление флагов)" );
		client( s, &peer );
	}
	else
		error( 1, errno, "ошибка вызова connect" );
	EXIT( 0 );
}