static void print_dg( char *dg, int len )
{
	struct ip *ip;
	struct icmp *icmp;
	struct hostent *hp;
	char *hname;
	int hl;
	static char *redirect_code[] =
	{
		"сеть", "хост",
		"тип сервиса и сеть", "тип сервиса и хост"
	};
	static char *timexceed_code[] =
	{
		"транзите", "сборке"
	};
	static char *param_code[] =
	{
		"Плохой IP-заголовок", "Нет обязательной опции"
	};
	ip = ( struct ip * )dg;
	if ( ip->ip_v != 4 )
	{
		error( 0, 0, "IP-датаграмма не версии 4\n" );
		return;
	}
	hl = ip->ip_hl << 2;	/* Длина IP-заголовка в байтах. */
	if ( len < hl + ICMP_MINLEN )
	{
		error( 0, 0, "short datagram (%d bytes) from %s\n",
			len, inet_ntoa( ip->ip_src ) );
		return;
	}
	hp = gethostbyaddr( ( char * )&ip->ip_src, 4, AF_INET );
	if ( hp == NULL )
		hname = "";
	else
		hname = hp->h_name;
	icmp = ( struct icmp * )( dg + hl );  /* ICMP-пакет. */
	printf( "ICMP %s (%d) от %s (%s)\n",
		get_type( icmp->icmp_type ),
		icmp->icmp_type, hname, inet_ntoa( ip->ip_src ) );
	if ( icmp->icmp_type == ICMP_UNREACH )
		print_unreachable( icmp );
	else if ( icmp->icmp_type == ICMP_REDIRECT )
		printf( "\tперенаправление на %s\n", icmp->icmp_code <= 3 ?
			redirect_code[ icmp->icmp_code ] : "некорректный код" );
	else if ( icmp->icmp_type == ICMP_TIMXCEED )
		printf( "\tTTL == 0 при %s\n", icmp->icmp_code <= 1 ?
		timexceed_code[ icmp->icmp_code ] : "некорректный код" );
	else if ( icmp->icmp_type == ICMP_PARAMPROB )
		printf( "\t%s\n", icmp->icmp_code <= 1 ?
			param_code[ icmp->icmp_code ] : "некорректный код" );
}