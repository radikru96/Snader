�����������������������������������������������������������wincompat.c
 1 #include <sys/timeb.h>
 2 #include "etcp.h"
 3 #include <winsock2.h>
 4 #define MINBSDSOCKERR			( WSAEWOULDBLOCK )
 5 #define MAXBSDSOCKERR			( MINBSDSOCKERR + \
 6									( sizeof( bsdsocketerrs ) / \
 7									sizeof( bsdsocketerrs[ 0 ] ) ) )
 8 extern int sys_nerr;
 9 extern char *sys_errlist[];
10 extern char *program_name;
11 static char *bsdsocketerrs[] =
12 {
13	"Resource temporarily unavailable",	/* ������ �������� ����������. */
14	"Operation now in progress",			/* �������� ������ �����������. */
15	"Operation already in progress",	/* �������� ��� �����������. */
16	"Socket operation on non-socket",		/* �������� ������ �� ��� �������. */
17	"Destination address required",		/* ����� ����� ����������. */
18	"Message too long",						/* ������� ������� ���������. */
19	"Protocol wrong type for socket",		/* �������� ��� ��������� ��� ������. */
20	"Bad protocol option",					/* ������������ ����� ���������. */
21	"Protocol not supported",				/* �������� �� ��������������. */
22	"Socket type not supported",			/* ��� ������ �� ��������������. */
23	"Operation not supported",				/* �������� �� ��������������. */
24	"Protocol family not supported",	/* ��������� ���������� �� */
											/* ��������������. */
25	"Address family not supported by protocol family", /* �������� ��������� */
								/* �� �������������� ���������� ����������*/	26	"Address already in use",				/* ����� ��� ������������. */
27	"Can�t assign requested address",	/* �� ���� �������� ������������� */
											/* �����. */
28	"Network is down",						/* ���� �� ��������. */
29	"Network is unreachable",				/* ���� ����������. */
30	"Network dropped connection on reset", /* ���� �������� ���������� */
											/* ��� ������������. */
31	"Software caused connection abort", 	/* ����������� ������ ����������. */
32	"Connection reset by peer",			/* ���������� �������� ������ */
											/* ��������. */
33	"No buffer space available",			/* ��� �������. */
34	"Socket is already connected",			/* ����� ��� ��������. */
35	"Socket is not connected",				/* ����� �� ��������. */
36	"Cannot send after socket shutdown",	/* �� ���� ������� ������ ����� */
											/* ����������. */
37	"Too many references: can�t splice", /* ������� ����� ������. */
38	"Connection timed out",					/* ������� �� ����������. */
39	"Connection refused",					/* � ���������� ��������. */
40	"Too many levels of symbolic links",	/* ������� ����� ������� */
											/* ������������� ������. */
41	"File name too long",					/* ������� ������� ��� �����. */
42	"Host is down",							/* ���� �� ��������. */
43	"No route to host"						/* ��� �������� � �����. */
44 };
45 void init( char **argv )
46 {
47		WSADATA wsadata;
48		( program_name = strrchr( argv[ 0 ], '\\' ) ) ?
49			program_name++ : ( program_name = argv[ 0 ] );
50		WSAStartup( MAKEWORD( 2, 2 ), &wsadata );
51	}
52 /* inet_aton - ������ inet_aton ��� SVr4 � Windows. */
53 int inet_aton( char *cp, struct in_addr *pin )
54 {
55    int rc;
56		rc = inet_addr( cp );
57		if ( rc == -1 && strcmp( cp, "255.255.255.255" ) )
58			return 0;
59		pin->s_addr = rc;
60		return 1;
61 }
62 /* gettimeofday - ��� tselect. */
63 int gettimeofday( struct timeval *tvp, struct timezone *tzp )
64 {
65		struct _timeb tb;
66		_ftime( &tb );
67		if ( tvp )
68		{
69			tvp->tv_sec = tb.time;
70			tvp->tv_usec = tb.millitm * 1000;
71		}
72		if ( tzp )
73		{
74			tzp->tz_minuteswest = tb.timezone;
75			tzp->tz_dsttime = tb.dstflag;
76		}
77 }
78 /* strerror - ������, ���������� ���� ������ Winsock. */
79 char *strerror( int err )
80 {
81		if ( err >= 0 && err < sys_nerr )
82			return sys_errlist[ err ];
83		else if ( err >= MINBSDSOCKERR && err < MAXBSDSOCKERR )
84			return bsdsocketerrs[ err - MINBSDSOCKERR ];
85		else if ( err == WSASYSNOTREADY )
86			return "Network subsystem is unusable";
						/* �0������ ���������� ����������������. */
87		else if ( err == WSAVERNOTSUPPORTED )
88			return "This version of Winsock not supported";
						/* ��� ������ Winsock �� ��������������. */
89		else if ( err == WSANOTINITIALISED )
90			return "Winsock not initialized";
						/* Winsock �� ����������������. */
91		else
92			return "Unknown error";
						/* ����������� ������. */
93 }
��������������������������������������������������������������wincompat.c