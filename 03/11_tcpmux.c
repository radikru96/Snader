void reaper( int sig )
{
	int waitstatus;
	while ( waitpid( -1, &waitstatus, WNOHANG ) > 0 ) {;}
}