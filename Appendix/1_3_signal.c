typedef void sighndlr_t( int );
sighndlr_t *signal( int sig, sighndlr_t *hndlr )
{
	struct sigaction act;
	struct sigaction xact;
	act.sa_handler = hndlr;
	act.sa_flags = 0;
	sigemptyset( &act.sa_mask );
	if ( sigaction( sig, &act, &xact ) < 0 )
		return SIG_ERR;
	return xact.sa_handler;
}