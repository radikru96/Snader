int daemon( int nocd, int noclose )
{
    struct rlimit rlim;
    pid_t pid;
    int i;
    umask( 0 );		/* Очистить маску создания файлов. */
    /* Получить максимальное число открытых файлов. */
    if ( getrlimit( RLIMIT_NOFILE, &rlim ) < 0 )
        error( 1, errno, "getrlimit failed" );
    /* Стать лидером сессии, потеряв при этом управляющий терминал... */
    pid = fork();
    if ( pid < 0 )
        return -1;
    if ( pid != 0 )
        exit( 0 );
    setsid();
    /* ... и гарантировать, что больше его не будет. */
    signal( SIGHUP, SIG_IGN );
    pid = fork();
    if ( pid < 0 )
        return -1;
    if ( pid != 0 )
        exit( 0 );
    /* Сделать текущим корневой каталог, если не требовалось обратное */
    if ( !nocd )
        chdir( "/" );
    /*
    * Если нас не просили этого не делать, закрыть все файлы.
    * Затем перенаправить stdin, stdout и stderr
    * на /dev/null.
    */
    if ( !noclose )
    {
        #if 0	/* Заменить на 1 для закрытия всех файлов. */
        if ( rlim.rlim_max == RLIM_INFINITY )
            rlim.rlim_max = 1024;
        for ( i = 0; i < rlim.rlim_max; i++ )
            close( i );
        #endif
        i = open( "/dev/null", O_RDWR );
        if ( i < 0 )
            return -1;
        dup2( i, 0 );
        dup2( i, 1 );
        dup2( i, 2 );
        if ( i > 2 )
            close( i );
    }
    return 0;
}