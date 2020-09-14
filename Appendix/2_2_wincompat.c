#include <sys/timeb.h>
#include "etcp.h"
#include <winsock2.h>
#define MINBSDSOCKERR			( WSAEWOULDBLOCK )
#define MAXBSDSOCKERR			( MINBSDSOCKERR + \
								( sizeof( bsdsocketerrs ) / \
								sizeof( bsdsocketerrs[ 0 ] ) ) )
extern int sys_nerr;
extern char *sys_errlist[];
extern char *program_name;
static char *bsdsocketerrs[] =
{
"Resource temporarily unavailable",		/* Ресурс временнно недоступен. */
"Operation now in progress",			/* Операция начала выполняться. */
"Operation already in progress",		/* Операция уже выполняется. */
"Socket operation on non-socket",		/* Операция сокета не над сокетом. */
"Destination address required",			/* Нужен адрес назначения. */
"Message too long",						/* Слишком длинное сообщение. */
"Protocol wrong type for socket",		/* Неверный тип протокола для сокета. */
"Bad protocol option",					/* Некорректная опция протокола. */
"Protocol not supported",				/* Протокол не поддерживается. */
"Socket type not supported",			/* Тип сокета не поддеживается. */
"Operation not supported",				/* Операция не поддерживается. */
"Protocol family not supported",		/* Семейство протоколов не поддерживается. */
"Address family not supported by protocol family", /* Адресное семейство не поддерживается семейством протоколов */	
"Address already in use",				/* Адрес уже испльзуется. */
"Can't assign requested address",		/* Не могу выделить затребованный адресс. */
"Network is down",						/* Сеть не работает. */
"Network is unreachable",				/* Сеть недоступна. */
"Network dropped connection on reset",	/* Сеть сбросила соединение при перезагрузке. */
"Software caused connection abort", 	/* Программный разрыв соединения. */
"Connection reset by peer",				/* Соединение сброшено другой стороной. */
"No buffer space available",			/* Нет буферов. */
"Socket is already connected",			/* Сокет уже соединён. */
"Socket is not connected",				/* Сокет не соединён. */
"Cannot send after socket shutdown",	/* Не могу послать данные после размыкания. */
"Too many references: can't splice",	/* Слишком много ссылок. */
"Connection timed out",					/* Таймаут на соединении. */
"Connection refused",					/* В соединении отказано. */
"Too many levels of symbolic links",	/* Слишком много уровней символических ссылок. */
"File name too long",					/* Слишком длинное имя файла. */
"Host is down",							/* Хост не работает. */
"No route to host"						/* Нет маршрута к хосту. */
};
void init( char **argv )
{
	WSADATA wsadata;
	( program_name = strrchr( argv[ 0 ], '\\' ) ) ?
		program_name++ : ( program_name = argv[ 0 ] );
	WSAStartup( MAKEWORD( 2, 2 ), &wsadata );
}
/* inet_aton - версия inet_aton для SVr4 и Windows. */
int inet_aton( char *cp, struct in_addr *pin )
{
   int rc;
	rc = inet_addr( cp );
	if ( rc == -1 && strcmp( cp, "255.255.255.255" ) )
		return 0;
	pin->s_addr = rc;
	return 1;
}
/* gettimeofday - для tselect. */
int gettimeofday( struct timeval *tvp, struct timezone *tzp )
{
	struct _timeb tb;
	_ftime( &tb );
	if ( tvp )
	{
		tvp->tv_sec = tb.time;
		tvp->tv_usec = tb.millitm * 1000;
	}
	if ( tzp )
	{
		tzp->tz_minuteswest = tb.timezone;
		tzp->tz_dsttime = tb.dstflag;
	}
}
/* strerror - версия, включающая коды ошибок Winsock. */
char *strerror( int err )
{
	if ( err >= 0 && err < sys_nerr )
		return sys_errlist[ err ];
	else if ( err >= MINBSDSOCKERR && err < MAXBSDSOCKERR )
		return bsdsocketerrs[ err - MINBSDSOCKERR ];
	else if ( err == WSASYSNOTREADY )
		return "Network subsystem is unusable";	/* Сетевая подсистема неработоспособна. */
	else if ( err == WSAVERNOTSUPPORTED )
		return "This version of Winsock not supported";	/* Эта версия Winsock не пожжерживается. */
	else if ( err == WSANOTINITIALISED )
			return "Winsock not initialized";	/* Winsock не инициализированна. */
	else
		return "Unknown error";	/* Неизвестная ошибка. */
}