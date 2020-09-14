#include "etcp.h"
#define ACK		0x6	/* Символ подтверждения ACK. */
int main( int argc, char **argv )
{
	fd_set allreads;
	fd_set readmask;
	fd_set sockonly;
	struct timeval tv;
	struct timeval *tvp = NULL;
	SOCKET s;
	int rc;
	char buf[ 128 ];
	const static struct timeval T0 = { 2, 0 };
	INIT();
	s = tcp_client( argv[ 1 ], argv[ 2 ] );
	FD_ZERO( &allreads );
	FD_SET( s, &allreads );
	sockonly = allreads;
	FD_SET( 0, &allreads );
	readmask = allreads;
	for ( ;; )
	{
		rc = select( s + 1, &readmask, NULL, NULL, tvp );
		if ( rc < 0 )
			error( 1, errno, "ошибка вызова select" );
		if ( rc == 0 )
			error( 1, 0, "тайм-аут при приёме сообщения\n" );
		if ( FD_ISSET( s, &readmask ) )
		{
			rc = recv( s, buf, sizeof( buf ), 0 );
			if ( rc == 0 )
				error( 1, 0, "сервер отсоединился\n" );
			else if ( rc < 0 )
				error( 1, errno, "ошибка вызова recv" );
			else if ( rc != 1 || buf[ 0 ] != ACK )
				error( 1, 0, "неожиданный вход [%c]\n", buf[ 0 ] );
			tvp = NULL;	/* Откючить таймер */
			readmask = allreads; /* и продолжить чтение из stdin. */
		}
		if ( FD_ISSET( 0, &readmask ) )
		{
		rc = read( 0, buf, sizeof( buf ) );
		if ( rc < 0 )
			error( 1, errno, "ошибка вызова read" );
		if ( send( s, buf, rc, 0 ) < 0 )
			error( 1, errno, "ошибка вызова send" );
		tv = T0;				/* Переустановить таймер. */
		tvp = &tv;				/* Взвести таймер */
		readmask = sockonly;    /* и прекратить чтение из stdin. */
	}
  }
}