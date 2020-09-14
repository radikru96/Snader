#include <stdio.h>
void main( void )
{
	int cnt = 0;
	char line[ 1024 ];
	/*
 	 * мы должны явно установить режим построчной буферизации, 
 	 * так как функции из библиотеки стандартного ввода/вывода 
 	 * не считают сокет терминалом. */
	setvbuf( stdout, NULL, _IOLBF, 0 );
	while ( fgets( line, sizeof( line ), stdin ) != NULL )
		printf( "%3i: %s", ++cnt, line );
}