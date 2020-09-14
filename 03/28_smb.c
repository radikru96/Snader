#include "etcp.h"
#define FREE_LIST		smbarray[ NSMB ].nexti
typedef union
{
	int nexti;
	char buf[ SMBUFSZ ];
} smb_t;
smb_t *smbarray;
void *smballoc( void )
{
	smb_t *bp;
	lock_buf();
	if ( FREE_LIST < 0 )
	error( 1, 0, "больше нет буферов в разделяемой памяти\n" );
	bp = smbarray + FREE_LIST;
	FREE_LIST  = bp->nexti;
	unlock_buf();
	return bp;
}
void smbfree( void *b )
{
	smb_t *bp;
	bp = b;
	lock_buf();
	bp->nexti = FREE_LIST;
	FREE_LIST  = bp - smbarray;
	unlock_buf();
}