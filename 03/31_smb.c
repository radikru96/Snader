#define FILENAME	"./smbfile"
#define lock_buf()	if ( WaitForSingleObject( mutex, INFINITE )\
				 != WAIT_OBJECT_0 ) \
				error( 1, errno, "ошибка вызова lock_buf " )
#define unlock_buf()	if ( !ReleaseMutex( mutex ) )\
				error( 1, errno, "ошибка вызова unlock_buf" )
HANDLE mutex;
void init_smb( int init_freelist )
{
	HANDLE hfile;
	HANDLE hmap;
	int i;
	mutex = CreateMutex( NULL, FALSE, "smbmutex" );
	if ( mutex == NULL )
		error( 1, errno, "ошибка вызова CreateMutex" );
	hfile = CreateFile( FILENAME,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if ( hfile == INVALID_HANDLE_VALUE )
		error( 1, errno, "ошибка вызова CreateFile" );
	hmap = CreateFileMapping( hfile, NULL, PAGE_READWRITE,
		0, NSMB * sizeof( smb_t ) + sizeof( int ), "smbarray" );
	smbarray = MapViewOfFile( hmap, FILE_MAP_WRITE, 0, 0, 0 );
	if ( smbarray == NULL )
		error( 1, errno, "ошибка вызова MapViewOfFile" );

	if ( init_freelist )
	{
		for ( i = 0; i < NSMB - 1; i++ )
			smbarray[ i ].nexti = i + 1;
		smbarray[ NSMB - 1 ].nexti = -1;
		FREE_LIST = 0;
	}
}