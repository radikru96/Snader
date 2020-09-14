static void parsetab( void )
{
	FILE *fp;
	servtab_t *stp = service_table;
	char *cp;
	int i;
	int lineno;
	char line[ MAXLINE ];
	fp = fopen( CONFIG, "r" );
	if ( fp == NULL )
		error( 1, errno, "не могу открыть %s", CONFIG );
	lineno = 0;
	while ( fgets( line, sizeof( line ), fp ) != NULL )
	{	
		lineno++;
		if ( line[ strlen( line ) - 1 ] != '\n' )
			error( 1, 0, "строка %d слишком длинная\n", lineno );
		if ( stp >= service_table + NSERVTAB )
			error( 1, 0, "слишком много строк в tcpmux.conf\n" );
		cp = strchr( line, '#' );
		if ( cp != NULL )
			*cp = '\0';
		cp = strtok( line, " \t\n" );
		if ( cp == NULL )
			continue;
		if ( *cp == '+' )
		{
			stp->flag = TRUE;
			cp++;
			if ( *cp == '\0' || strchr( " \t\n", *cp ) != NULL )
				error( 1, 0, "строка %d: пробел после '+'\n",
					lineno );
		}
		stp->service = strdup( cp );
		if ( stp->service == NULL )
			error( 1, 0, "не хватило памяти\n" );
		cp = strtok( NULL, " \t\n" );
		if ( cp == NULL )
			error( 1, 0, "строка %d: не задан путь (%s)\n",
				lineno, stp->service );
		stp->path = strdup( cp );
		if ( stp->path == NULL )
			error( 1, 0, "не хватиоло памяти\n" );
		for ( i = 0; i < MAXARGS; i++ )
		{
			cp = strtok( NULL, " \t\n" );
			if ( cp == NULL )
				break;
			stp->args[ i ] = strdup( cp );
			if ( stp->args[ i ] == NULL )
				error( 1, 0, "не хватиоло памяти\n" );
		}
		if ( i >= MAXARGS && strtok( NULL, " \t\n" ) != NULL )
		error( 1, 0, "строка %d: слишком много аргументов (%s)\n",
			lineno, stp->service );
		stp->args[ i ] = NULL;
		stp++;
	}
	stp->service = NULL;
	fclose ( fp );
}