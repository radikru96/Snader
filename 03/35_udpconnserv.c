#include "etcp.h"
int main( int argc, char **argv )
{
    struct sockaddr_in peer;
    SOCKET s;
    int rc;
    int len;
    char buf[ 120 ];
    INIT();
    s = udp_server( NULL, argv[ 1 ] );
    len = sizeof( peer );
    rc = recvfrom( s, buf, sizeof( buf ),
        0, ( struct sockaddr * )&peer, &len );
    if ( rc < 0 )
        error( 1, errno, "ошибка вызова recvfrom" );
    if ( connect( s, ( struct sockaddr * )&peer, len ) )
        error( 1, errno, "ошибка вызова connect" );
    while ( strncmp( buf, "done", 4 ) != 0 )
    {
        if ( send( s, buf, rc, 0 ) < 0 )
            error( 1, errno, "ошибка вызова send" );
        rc = recv( s, buf, sizeof( buf ), 0 );
        if ( rc < 0 )
            error( 1, errno, "ошибка вызова recv" );
    }
    EXIT( 0 );
}