#ifndef __SKEL_H__
#define __SKEL_H__
/* версия Winsock. */
#include <windows.h>
#include <winsock2.h>
struct timezone
{
	long tz_minuteswest;
	long tz_dsttime;
};
typedef unsigned int u_int32_t;
#define EMSGSIZE			WSAEMSGSIZE
#define INIT()				init( argv );
#define EXIT(s)				do { WSACleanup(); exit( ( s ) ); } \
							while ( 0 )
#define CLOSE(s)			if ( closesocket( s ) ) \
							error( 1, errno, "ошибка вызова close")
#define errno				( GetLastError() )
#define set_errno(e)		SetLastError( ( e ) )
#define isvalidsock(s)		( ( s ) != SOCKET_ERROR )
#define bzero(b,n)	memset	( ( b ), 0, ( n ) )
#define sleep(t)			Sleep( ( t ) * 1000 )
#define WINDOWS
#endif  /* __SKEL_H__ */