static tevent_t *allocate_timer( void )
{
    tevent_t *tp;
    if ( free_list == NULL ) /* нужен новый блок таймеров? */
    {
        free_list = malloc( NTIMERS * sizeof( tevent_t ) );
        if ( free_list == NULL )
            error( 1, 0, "не удалось получить таймеры\n" );
        for ( tp = free_list;
            tp < free_list + NTIMERS - 1; tp++ )
            tp->next = tp + 1;
        tp->next = NULL;
    }
    tp = free_list;	/* Выделить первый. */
    free_list = tp->next;	/* Убрать его из списка. */
    return tp;
        }
    unsigned int timeout( void ( *func )( void * ), void *arg, int ms )
        {
    tevent_t *tp;
    tevent_t *tcur;
    tevent_t **tprev;
    static unsigned int id = 1;	/* Идентификатор таймера. */
    tp = allocate_timer();
    tp->func = func;
    tp->arg = arg;
    if ( gettimeofday( &tp->tv, NULL ) < 0 )
        error( 1, errno, "timeout: ошибка вызова gettimeofday" );
    tp->tv.tv_usec += ms * 1000;
    if ( tp->tv.tv_usec > 1000000 )
    {
        tp->tv.tv_sec += tp->tv.tv_usec / 1000000;
        tp->tv.tv_usec %= 1000000;
    }
    for ( tprev = &active, tcur = active;
        tcur && !timercmp( &tp->tv, &tcur->tv, < ); /* XXX */
        tprev = &tcur->next, tcur = tcur->next )
    { ; }
    *tprev = tp;
    tp->next = tcur;
    tp->id = id++;	/* Присвоить значение индетификатору таймера. */
    return tp->id;
}