#include "etcp.h"
#define MAXARGS	10		/* максимальное число аргументов сервера. */
#define MAXLINE	256		/* максимальная длина строки в tcpmux.conf. */
#define NSERVTAB	10		/* Число элементов в таблице service_table. */
#define CONFIG	"tcpmux.conf"
typedef struct
{
	int flag;
	char *service;
	char *path;
	char *args[ MAXARGS + 1 ];
	 } servtab_t;
		int ls;				/* Прослушиваемый сокет. */
		servtab_t service_table[ NSERVTAB + 1 ];
		int main( int argc, char **argv )
		{
	struct sockaddr_in peer;
	int s;
	int peerlen;
	/* Инициализировать и запустить сервер tcpmux. */
	INIT();
	parsetab();
	switch ( argc )
	{
		case 1:		/* Всё по умолчанию. */
			ls = tcp_server( NULL, "tcpmux" );
			break;
		case 2:		/* Задан интерфейс и номер порта. */
			ls = tcp_server( argv[ 1 ], "tcpmux" );
			break;
		case 3:		/* Заданы все парметры. */
			ls = tcp_server( argv[ 1 ], argv[ 2 ] );
			break;
		default:
			error( 1, 0, "Вызов: %s [ интерфейс [ порт ] ]\n",
				program_name );
	}
	daemon( 0, 0 );
	signal( SIGCHLD, reaper );
	/* Принять соединения с портом tcpmux. */
	for ( ;; )
	{
		peerlen = sizeof( peer );
		s = accept( ls, ( struct sockaddr * )&peer, &peerlen );
		if ( s < 0 )
			continue;
		start_server( s );
		CLOSE( s );
	}
}