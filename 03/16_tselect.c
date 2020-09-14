void untimeout( unsigned int id )
{
	tevent_t **tprev;
	tevent_t *tcur;
	for ( tprev = &active, tcur = active;
		  tcur && id != tcur->id;
		  tprev = &tcur->next, tcur = tcur->next )
	{ ; }
	if ( tcur == NULL )
	{
		error( 0, 0,
		"при вызове untimeout указан несуществующий таймер (%d)\n", id );
		return;
	}
	*tprev = tcur->next;
	tcur->next = free_list;
	free_list = tcur;
}