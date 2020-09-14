#include <sys/shm.h>
#include <sys/sem.h>
#define MUTEX_KEY	0x534d4253	/* SMBS */
#define SM_KEY	0x534d424d	/* SMBM */
#define lock_buf()	if ( semop( mutex, &lkbuf, 1 ) < 0 ) \
					error( 1, errno, "ошибка вызова semop" )
#define unlock_buf()	if ( semop( mutex, &unlkbuf, 1 ) < 0 ) \
					error( 1, errno, "ошибка вызова semop" )
int mutex;
struct sembuf lkbuf;
struct sembuf unlkbuf;
void init_smb( int init_freelist )
{
	union semun arg;
	int smid;
	int i;
	int rc;
	lkbuf.sem_op = -1;
	lkbuf.sem_flg = SEM_UNDO;
	unlkbuf.sem_op = 1;
	unlkbuf.sem_flg = SEM_UNDO;
	mutex = semget( MUTEX_KEY, 1,
		IPC_EXCL | IPC_CREAT | SEM_R | SEM_A );
	if ( mutex >= 0 )
	{
		arg.val = 1;
		rc = semctl( mutex, 0, SETVAL, arg );
		if ( rc < 0 )
			error( 1, errno, "semctl failed" );
	}	
	else if ( errno == EEXIST )
	{
		mutex = semget( MUTEX_KEY, 1, SEM_R | SEM_A );
		if ( mutex < 0 )
			error( 1, errno, "ошибка вызова semctl" );
	}
	else
		error( 1, errno, "ошибка вызова semctl" );
	smid = shmget( SM_KEY, NSMB * sizeof( smb_t ) + sizeof( int ),
		SHM_R | SHM_W | IPC_CREAT );	
	if ( smid < 0 )
		error( 1, errno, "ошибка вызова shmget" );
	smbarray = ( smb_t * )shmat( smid, NULL, 0 );
	if ( smbarray == ( void * )-1 )
		error( 1, errno, "ошибка вызова shmat" );
	if ( init_freelist )
	{
		for ( i = 0; i < NSMB - 1; i++ )
			smbarray[ i ].nexti = i + 1;
		smbarray[ NSMB - 1 ].nexti = -1;
		FREE_LIST = 0;
	}
}