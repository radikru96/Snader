#include "etcp.h"
#include "heartbeat.h"
int main( int argc, char **argv )
{
	fd_set allfd;
	fd_set readfd;
	msg_t msg;
	struct timeval tv;
	SOCKET s;
	int rc;
	int heartbeats = 0;
	int cnt = sizeof( msg );
	INIT();
	s = tcp_client( argv[ 1 ], argv[ 2 ] );
	FD_ZERO( &allfd );
	FD_SET( s, &allfd );
	tv.tv_sec = T1;
	tv.tv_usec = 0;
	for ( ;; )
	{
		readfd = allfd;
		rc = select( s + 1, &readfd, NULL, NULL, &tv );
		if ( rc < 0 )
			error( 1, errno, "ошибка вызова select" );
		if ( rc == 0 )	/* Произошёл тайм-аут. */
		{
			if ( ++heartbeats > 3 )
			error( 1, 0, "соединения нет\n" );
		error( 0, 0, "посылаю пульс #%d\n", heartbeats );
		msg.type = htonl( MSG_HEARTBEAT );
		rc = send( s, ( char * )&msg, sizeof( msg ), 0 );
		if ( rc < 0 )
			error( 1, errno, "ошибка вызова send" );
		tv.tv_sec = T2;
		continue;
	}
	if ( !FD_ISSET( s, &readfd ) )
		error( 1, 0, "select вернул некорректный сокет\n" );
	rc = recv( s, ( char * )&msg + sizeof( msg ) - cnt,
		cnt, 0 );
	if ( rc == 0 )
		error( 1, 0, "сервер закончил работу\n" );
	if ( rc < 0 )
		error( 1, errno, "ошибка вызова recv" );
	heartbeats = 0;
	tv.tv_sec = T1;
	cnt -= rc;					/* Встроенный readn. */
	if ( cnt > 0 )
		continue;
	cnt = sizeof( msg );
	/* Обработка сообщения. */
	}
}