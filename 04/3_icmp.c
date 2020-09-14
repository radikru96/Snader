#include <sys/types.h>
#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include "etcp.h"
int main( int argc, char **argv )
{
    SOCKET s;
    struct protoent *pp;
    int rc;
    char icmpdg[ 1024 ];
    INIT();
    pp = getprotobyname( "icmp" );
    if ( pp == NULL )
        error( 1, errno, "ошибка вызова getprotobyname" );
    s = socket( AF_INET, SOCK_RAW, pp->p_proto );
    if ( !isvalidsock( s ) )
        error( 1, errno, "ошибка вызова socket" );
    for ( ;; )
    {
        rc = recvfrom( s, icmpdg, sizeof( icmpdg ), 0,
            NULL, NULL );
        if ( rc < 0 )
            error( 1, errno, "ошибка вызова recvfrom" );
        print_dg( icmpdg, rc );
    }
}