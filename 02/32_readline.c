int readline( SOCKET fd, char *bufptr, size_t len )
{
    char *bufx = bufptr;
    static char *bp;
    static int cnt = 0;
    static char b[ 1500 ];
    char c;
    while ( --len > 0 )
    {
        if ( --cnt <= 0 )
        {
            cnt = recv( fd, b, sizeof( b ), 0 );
            if ( cnt < 0 )
            {
                if ( errno == EINTR )
                {
                    len++;	/* уменьшим на 1 в заголовке while. */
                    continue;
                }
                return -1;
            }
            if ( cnt == 0 )
                return 0;
            bp = b;
        }
        c = *bp++;
        *bufptr++ = c;
        if ( c == "\n" )
        {
            *bufptr = "\0";
            return bufptr - bufx;
        }
    }
    set_errno( EMSGSIZE );
    return -1;
}