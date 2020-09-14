msgrec_t *getfreerec( void )
{
	msgrec_t *mp;
	for ( mp = mr; mp < mr + MRSZ; mp++ )
		if ( mp->pkt.len == -1 )	/* Запись свободна? */
			return mp;
	error( 1, 0, "getfreerec: исчерпан пул записей сообщений\n" );
	return NULL;	/* "Во избежание предупреждений компилятора. */
}
msgrec_t *findmsgrec( u_int32_t mid )
{
	msgrec_t *mp;
	for ( mp = mr; mp < mr + MRSZ; mp++ )
		if ( mp->pkt.len != -1 && mp->pkt.cookie == mid )
			return mp;
	error( 0, 0, "findmsgrec: нет сообщения, соответствующего ACK %d\n", mid );
	return NULL;
}
void freemsgrec( msgrec_t *mp )
{
	if ( mp->pkt.len == -1 )
	error( 1, 0, "freemsgrec: запись сообщения уже освобождена\n" );
	mp->pkt.len = -1;
}
static void drop( msgrec_t *mp )
{
	error( 0, 0, "Сообщение отбрасывается:   %s", mp->pkt.buf );
	freemsgrec( mp );
}
static void lost_ACK( msgrec_t *mp )
{
	error( 0, 0, "Повтор сообщения:   %s", mp->pkt.buf );
	if ( send( s, &mp->pkt,
		 sizeof( u_int32_t ) + ntohl( mp->pkt.len ), 0 ) < 0 )
		error( 1, errno, "потерян ACK: ошибка вызова send" );
	mp->id = timeout( ( tofunc_t )drop, mp, T2 );
}