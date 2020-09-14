int readn( SOCKET fd, char *bp, size_t len)
{
	int cnt;
	int rc;
	cnt = len;
	while ( cnt > 0 )
	{
		rc = recv( fd, bp, cnt, 0 );
		if ( rc < 0 )			    /* Ошибка чтения? */
		{
			if ( errno == EINTR )	/* Вызов прерван? */
				continue;			/* Повторить чтение. */
			return -1;		    	/* Вернуть код ошибки. */
		}
		if ( rc == 0 )			    /* Конец файла? */
			return len - cnt;	    /* Вернуть неполный счетчик. */
		bp += rc;
		cnt -= rc;
	}
	return len;
}