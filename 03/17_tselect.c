int tselect( int maxp1, fd_set *re, fd_set *we, fd_set *ee )
{
	fd_set rmask;
	fd_set wmask;
	fd_set emask;
	struct timeval now;
	struct timeval tv;
	struct timeval *tvp;
	tevent_t *tp;
	int n;
	if ( re )
		rmask = *re;
	if ( we )
		wmask = *we;
	if ( ee )
		emask = *ee;
	for ( ;; )
	{
		if ( gettimeofday( &now, NULL ) < 0 )
			error( 1, errno, "tselect: ошибка вызова gettimeofday" );
		while ( active && !timercmp( &now, &active->tv, < ) )
		{
			active->func( active->arg );
			tp = active;
			active = active->next;
			tp->next = free_list;
			free_list = tp;
		}
		if ( active )
		{
			tv.tv_sec = active->tv.tv_sec - now.tv_sec;;
			tv.tv_usec = active->tv.tv_usec - now.tv_usec;
			if ( tv.tv_usec < 0 )
			{
				tv.tv_usec += 1000000;
				tv.tv_sec--;
			}
			tvp = &tv;
		}
		else if ( re == NULL && we == NULL && ee == NULL )
			return 0;
		else
			tvp = NULL;
		n = select( maxp1, re, we, ee, tvp );
		if ( n < 0 )
			return -1;
		if ( n > 0 )
			return n;
		if ( re )
			*re = rmask;
		if ( we )
			*we = wmask;
		if ( ee )
			*ee = emask;
	}
}