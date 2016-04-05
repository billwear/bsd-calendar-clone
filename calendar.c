// calendar program - bill wear - fall-winter, 2014
//
// design parameters:
// 1. code my own calendar program, after the style of the bsd calendar utility
// 2. slowly add functionality related to emacs org-mode
// 3. learn a lot about coding and program design in the process
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int parse_date( char* date )
{
	// dates usually have numbers
	char keys[] = "0123456789";
	char tab[3], smonth[3];
	char* pch;
	int day, month, year;
	time_t currtime;
	struct tm* timeinfo;

	tab[0] = '/';
	tab[1] = '\n';
	tab[2] = 0;
	day = 32;
	month = 13;
	year = 100;

	// every date (right now) has a slash
	if( strchr( date, '/' ) == NULL )
		return(1);

	// if the date *doesn't* have numbers, ...
	if( strcspn( date, keys ) == strlen( date ))

		// ... is it the special "to do" keyword?
		if( strstr( date, "to/do" ) == NULL )
			return(1);
		else
			return(0);

	// pull off the month
	pch = strtok( date, tab );
	if( sscanf( pch, "%d", &month ) == 0 )
		return(2);
	if( month > 12 )
		return(3);
		
	// pull off the day
	pch = strtok( NULL, tab );
	if( pch == NULL )
		return(4);
	if( sscanf( pch, "%d", &day ) == 0 )
		return(5);
	if( day > 31 )
		return(6);

	// pull off the year
	pch = strtok( NULL, tab );
	if( pch == NULL )
	{
		year = -1;
	}
	else
	{
		if( sscanf( pch, "%d", &year ) == 0 )
			return(7);
		if( year > 99 )
			return(8);
	}

	// is this for today?
	time( &currtime );
	timeinfo = localtime( &currtime );

	if( year == -1 )
		year = timeinfo->tm_year-100;

	if( !(month == timeinfo->tm_mon+1 && day == timeinfo->tm_mday && year == timeinfo->tm_year-100) )
		return(9);

	// format the date output value
	switch(month)
	{
		case 1:
			sprintf( smonth, "Jan" );
			break;
		case 2:
			sprintf( smonth, "Feb" );
			break;
		case 3:
			sprintf( smonth, "Mar" );
			break;
		case 4:
			sprintf( smonth, "Apr" );
			break;
		case 5:
			sprintf( smonth, "May" );
			break;
		case 6:
			sprintf( smonth, "Jun" );
			break;
		case 7:
			sprintf( smonth, "Jul" );
			break;
		case 8:
			sprintf( smonth, "Aug" );
			break;
		case 9:
			sprintf( smonth, "Sep" );
			break;
		case 10:
			sprintf( smonth, "Oct" );
			break;
		case 11:
			sprintf( smonth, "Nov" );
			break;
		case 12:
			sprintf( smonth, "Dec" );
			break;
	}

	sprintf( date, "%s %2d", smonth, day );
		
	return(0);
}

int cmpfunc (const void * a, const void * b)
{
	return strncmp((const char *)a, (const char *)b, 6);
}

int open_calendar_file( FILE* fp )
{
	fp = fopen( "zork", "r" );
	if( fp == NULL )
	{
		perror("could not open file");
		return 1;
	}
}

int main( int argc, char* argv[] )
{
	// *phase 1: just read the file and stream it back, without filtering
	// *phase 2: clean up the output by taking control of newlines and tabs
	// *phase 3: dump any lines that don't have a tab in them
	// *phase 4: separate date and description and put them back together on output
	// *phase 5: don't print descriptions that don't go with a previous date
	// *phase 6: limit output to items that match today's date
	//  *phase 6a: reject dates that don't parse to mm/dd/yy
	//	 *phase 6a1: accept "to/do" as a valid date
	//  *phase 6b: convert inbound date to correct comparison format
	//  *phase 6c: decide if this item is for today's date
	// *phase 7: convert "mm/dd(/yy)" date to "Mmm dd" format for output
	//  phase 8: consolidate date-matching lines across the file
	//	 phase n: recognize other date formats
	

	// *** declarations
	char line[132], 
		date[32], 
		lastdate[32],
		descr[100], 
		tab[3];
	char lines[1000][132];
	char *pch;
	int i, j, k, datefound;
	FILE *fp;

	open_calendar_file( fp );
	// just setting up flags
	tab[0] = '\t';
	tab[1] = '\n';
	tab[2] = 0;
	datefound = 0;

	// read up to 1000 lines from the file
	for( j=0; j<1000; j++ )
	{
		if( fgets( line, 1024, fp ) == NULL )
		{
			break;
		}
		// ignore lines with no tabs
		if( strchr( line, '\t' ) == NULL )
			continue;

		// assume first token is a date
		pch = strtok( line, tab );
		strcpy( date, pch );
		
		// look for a second token
		pch = strtok( NULL, tab );
		if( pch == NULL )
		{
			// no second token? 
			if( datefound == 0 )
				continue;

			// build blank continuation line
			//  if the last date found applies
			strcpy( descr, date );
			strcpy( date, lastdate );
//			memset( date, ' ', 8 );
//			date[8] = 0;
		}
		else
		{
			// yes, second token
			// is this a valid date?
			if( parse_date( date ))
			{
				datefound = 0;
				continue;
			}
			strcpy( descr, pch);
			strcpy( lastdate, date );
			datefound = 1;
		}
	
		// store the calendar line

		sprintf(lines[j], 
			         "%-8s %s\n", 
			          date, descr);
	}
	qsort(lines, j, 132, cmpfunc);
	memset( lastdate, ' ', 32 ); 
	for( i=0; i<j; i++ )
	{
		if( !strncmp( lastdate, lines[i], 6 ))
		{
			for( k=0; k<6; k++ )
			{
				lines[i][k] = ' ';
			}
		}
		else
		{
			strncpy( lastdate, lines[i], 6 );
		} 
		if( strlen( lines[i] ) > 6)
		{
			printf("%s", lines[i]);
		}
	}
}
