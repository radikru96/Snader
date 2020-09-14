int main( int argc, char **argv )
{
	fd_set allreads;
	fd_set readmask;
	msgrec_t *mp;
	int rc;
	int mid;
	int cnt = 0;
	u_int32_t msgid = 0;
	char ack[ ACKSZ ];
	INIT();
	s = tcp_client( argv[ 1 ], argv[ 2 ] );
	FD_ZERO( &allreads );
	FD_SET( s, &allreads );
	FD_SET( 0, &allreads );
	for ( mp = mr; mp < mr + MRSZ; mp++ )
		mp->pkt.len = -1;
	for ( ;; )
	{
		readmask = allreads;
		rc = tselect( s + 1, &readmask, NULL, NULL );
		if ( rc < 0 )
			error( 1, errno, "ошибка вызова tselect" );
		if ( rc == 0 )
			error( 1, 0, "tselect сказала, что нет событий\n" );
		if ( FD_ISSET( s, &readmask ) )
		{
			rc = recv( s, ack + cnt, ACKSZ - cnt, 0 );
			if ( rc == 0 )
				error( 1, 0, "сервер отсоединился\n" );
			else if ( rc < 0 )
				error( 1, errno, "ошибка вызова recv" );
			if ( ( cnt += rc ) < ACKSZ ) /* Целое сообщение? */
				continue;		/* Нет, ещё подождем. */
			cnt = 0;		/* В следующий раз новое сообщение. */
			if ( ack[ 0 ] != ACK )
			{
				error( 0, 0, "предупреждение: неверное пдтверждение\n" );
				continue;
			}
			memcpy( &mid, ack + 1, sizeof( u_int32_t ) );
			mp = findmsgrec( mid );
			if ( mp != NULL )
			{
			untimeout( mp->id );	/* Отменить таймер. */
			freemsgrec( mp ); /* Удалить сохранённое сообщение. */
			}
		}
		if ( FD_ISSET( 0, &readmask ) )
		{
			mp = getfreerec();
			rc = read( 0, mp->pkt.buf, sizeof( mp->pkt.buf ) );
			if ( rc < 0 )
				error( 1, errno, "ошибка вызова read" );
			mp->pkt.buf[ rc ] = '\0';
			mp->pkt.cookie = msgid++;
			mp->pkt.len = htonl( sizeof( u_int32_t ) + rc );
			if ( send( s, &mp->pkt,
				 2 * sizeof( u_int32_t ) + rc, 0 ) < 0 )
				error( 1, errno, "ошибка вызова send" );
			mp->id = timeout( ( tofunc_t )lost_ACK, mp, T1 );
		}
	}
}