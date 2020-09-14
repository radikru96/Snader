#include "etcp.h"
#include "heartbeat.h"
int main( int argc, char **argv )
{
    fd_set allfd;
    fd_set readfd;
    msg_t msg;
    struct timeval tv;
    SOCKET s;
    SOCKET s1;
    int rc;
    int missed_heartbeats = 0;
    int cnt = sizeof( msg );
    INIT();
    s = tcp_server( NULL, argv[ 1 ] );
    s1 = accept( s, NULL, NULL );
    if ( !isvalidsock( s1 ) )
        error( 1, errno, "ошибка вызова accept" );
    tv.tv_sec = T1 + T2;
    tv.tv_usec = 0;
    FD_ZERO( &allfd );
    FD_SET( s1, &allfd );
    for ( ;; )
    {
	readfd = allfd;
	rc = select( s1 + 1, &readfd, NULL, NULL, &tv );
	if ( rc < 0 )
		error( 1, errno, "ошибка вызова select" );
	if ( rc == 0 )	/* Произошёл таум-аут. */
	{
		if ( ++missed_heartbeats > 3 )
			error( 1, 0, "соединение умерло\n" );
		error( 0, 0, "пропущен пульс #%d\n",
			missed_heartbeats );
		tv.tv_sec = T2;
		continue;
	}
	if ( !FD_ISSET( s1, &readfd ) )
		error( 1, 0, "select вернул некорректный сокет\n" );
	rc = recv( s1, ( char * )&msg + sizeof( msg ) - cnt,
		cnt, 0 );
	if ( rc == 0 )
		error( 1, 0, "клиент завершил работу\n" );
	if ( rc < 0 )
		error( 1, errno, "ошибка вызова recv" );
	missed_heartbeats = 0;
	tv.tv_sec = T1 + T2;
	cnt -= rc;	/* Встроенный readn. */
	if ( cnt > 0 )
		continue;
	cnt = sizeof( msg );
	switch ( ntohl( msg.type ) )
	{
		case MSG_TYPE1 :
			/* Обработать сообщение типа TYPE1. */
			break;
		case MSG_TYPE2 :
			/* Обработать сообщение типа TYPE2. */
			break;
		case MSG_HEARTBEAT :
			rc = send( s1, ( char * )&msg, sizeof( msg ), 0 );
			if ( rc < 0 )
				error( 1, errno, "ошибка вызова send" );
			break;
		default :
			error( 1, 0, "неизвестный тип сообщения (%d)\n",
				ntohl( msg.type ) );
	}
}
EXIT( 0 );
	}