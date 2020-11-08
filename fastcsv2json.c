/*
 * Copyright (C) 2020 Lucas Tsatiris 
 * 
 * email: lucast70@hotmail.com
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 */

/* 
   changelog:

   2020-11-08: initial release 0.1.0
   
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PROGRAM_NAME	"fastcsv2json"
#define VERSION_MAJOR	0
#define VERSION_MINOR	1
#define VERSION_PATCH	0

#define COLUMNS_UPPER_LIMIT			1024
#define COLUMNS_DEFAULT				 200
#define COLUMN_SIZE_UPPER_LIMIT		8192
#define COLUMN_SIZE_LOWER_LIMIT		  16
#define COLUMN_SIZE_DEFAULT		     500

size_t MAX_COLUMNS = COLUMNS_DEFAULT, 
       MAX_COLUMN_SIZE = COLUMN_SIZE_DEFAULT;
           
char **column_name, 
     **column;           

char delimiter[2] = ",";

/* 
 * strtoke () is an strtok (3) implementation but does not treat continuous
 * delimiters as one
 */
static char * 
strtoke(char *str, const char *delim)
{
  static char *start = NULL; /* stores string str for consecutive calls */
  char *token = NULL; /* found token */
  /* assign new start in case */
  if (str) start = str;
  /* check whether text to parse left */
  if (!start) return NULL;
  /* remember current start as found token */
  token = start;
  /* find next occurrence of delim */
  start = strpbrk(start, delim);
  /* replace delim with terminator and move start to follower */
  if (start) *start++ = '\0';
  /* done */
  return token;
}

/*
 * generate json output
 * 
 * returns: 0 for success, 1 for not enough memory
 */
int
generate_json ()
{
  char *linein = NULL, *lineout = NULL, *tok, *buff = NULL;	
  int counter, ncolumns = 0, len = 0, nlines = 0, result = 1,
      lisize = 0, losize = 0;
  
  /* allocate memory for column_names and column arrays of (char *) */
  column_name = (char **) calloc (MAX_COLUMNS, sizeof (char *));
  column = (char **) calloc (MAX_COLUMNS, sizeof (char *)); 
  if (column_name == NULL || column == NULL)
    goto generate_json_end;  
  
  /* pre-allocate memory for column names */
  for (counter = 0; counter < MAX_COLUMNS; counter ++)
  {
    column_name[counter] = (char *) calloc (MAX_COLUMN_SIZE, sizeof (char *));
    if (column_name[counter] == NULL)
      goto generate_json_end;	 
  }
  
  /* allocate memory for input and output lines */
  lisize = MAX_COLUMNS * (MAX_COLUMN_SIZE + 1) + 3; /* increase column size 
                                                       by delimiter size plus
                                                       3 bytes for cr, lf and 
                                                       null */
  linein = (char *) malloc (lisize);
  
  losize = MAX_COLUMNS * ((MAX_COLUMN_SIZE * 2) + 6) + 5; /* increase column 
                                                             size for double 
                                                             quotes, column and
													         comma in json 
													         output plus 5 for 
													         [,],{,} and NULL */
  lineout = (char *) malloc (losize);
  
  if (linein == NULL || lineout == NULL)
    goto generate_json_end;
  
  /* get the column names: headers on the first line */
  if (fgets (linein, lisize, stdin) == NULL)
    goto generate_json_end;
  
  /* clear cr and lf from the end of linein */
  len = strlen (linein);
  
  if (len > 1)
    if (linein[len - 1] == 10 || linein[len - 1] == 13)
      linein[len - 1] = 0;
  
  if (len > 2)
    if (linein[len - 2] == 10 || linein[len - 2] == 13)
      linein[len - 2] = 0;
  
  /* fill the column_names array */
  counter = 0;  
  tok = strtoke (linein, delimiter);
  while (tok != NULL)
  {
	strncpy (column_name[counter], tok, MAX_COLUMN_SIZE - 1);  
    tok = strtoke (NULL, delimiter);
    counter ++;
    if (counter == MAX_COLUMNS)
      tok = NULL;
  }  
  ncolumns = counter;
  
  for (counter = 0; counter < ncolumns; counter ++)
  {
    column[counter] = (char *) calloc (1, MAX_COLUMN_SIZE);
    if (column[counter] == NULL)
      goto generate_json_end;	   
  }
  
  /* read the data */
  fprintf (stdout, "[");
  while (fgets (linein, lisize, stdin) != NULL)
  {
	if (strlen (linein) > 0 && strpbrk (linein, delimiter) != NULL)
	{  
      if (nlines > 0)
      {
        strcat (lineout, "},");	  
        fprintf (stdout, "%s\n", lineout);
      }
      
      nlines ++; 
	 
	  /* clear cr and lf from the end of linein */
      len = strlen (linein);
  
      if (len > 1)
        if (linein[len - 1] == 10 || linein[len - 1] == 13)
          linein[len - 1] = 0;
  
      if (len > 2)
        if (linein[len - 2] == 10 || linein[len - 2] == 13)
          linein[len - 2] = 0;
	  
	  /* fill the cells */
      tok = strtoke (linein, delimiter);
      for (counter = 0; counter < ncolumns; counter ++)
      {
	    if (tok != NULL)	
	      strncpy (column[counter], tok, MAX_COLUMN_SIZE);  
	    else
	      strcpy (column[counter], "");  

        tok = strtoke (NULL, delimiter);
      }
    
      /* prepare json output */
      strcpy (lineout, "{");
      for (counter = 0; counter < ncolumns; counter ++)
      {
	    strcat (lineout, "\"");
	    strcat (lineout, column_name[counter]);
	    strcat (lineout, "\":\"");
	    strcat (lineout, column[counter]);
	    strcat (lineout, "\"");
	    if (counter < ncolumns - 1)
	      strcat (lineout, ",");
      }    
    }  
  }
  strcat (lineout, "}]");	  
  fprintf (stdout, "%s\n", lineout);
  
  result = 0;		/* succeeded */
  
generate_json_end:
  for (counter = 0; counter < MAX_COLUMNS; counter ++)
  {
	if (column_name != NULL)  
	  if (column_name[counter] != NULL) free (column_name[counter]);
	
	if (column != NULL)  
	  if (column[counter] != NULL) free (column[counter]);
  }	  
  
  if (column_name != NULL)
    free (column_name);
  
  if (column != NULL)
    free (column);  
  
  if (linein != NULL)
    free (linein);
  
  if (linein != NULL)
    free (lineout);
   
   if (buff != NULL)
    free (buff); 
  
  return result;		
}

/* help */
void
help ()
{
  fprintf (stderr, "Usage: fastcsv2json [OPTION]\n");	
  fprintf (stderr, "Convert csv to json array from STDIN to STDOUT\n\n"); 
  fprintf (stderr, "Options:\n\n");
  fprintf (stderr, "-c, --max-columns       maximum columns, default %d\n", COLUMNS_DEFAULT);
  fprintf (stderr, "-d, --delimiter         delimiter as pipe, comma, semicolumn, column or tab\n");
  fprintf (stderr, "-h, --help              give this help\n");
  fprintf (stderr, "-s, --max-column-size   maximum column size, default %d\n", COLUMN_SIZE_DEFAULT);
  fprintf (stderr, "-v, --version           version information, license and copyright\n\n");
  fprintf (stderr, "example: fastcsv2json -d pipe -c 10 -s 20 < myfile.csv > myfile.json\n\n");
}	

/* parse command line arguments */
int
parse_arguments (int argc, char *argv[])
{
  int result = 0, counter = 1;
  
  while (counter < argc)
  {
	/* help */  
	if (strcmp (argv[counter], "-h") == 0 ||
	    strcmp (argv[counter], "--help") == 0)
	{
	  help ();
	  return 1;	
    }	    
    else
    /* version */  
	if (strcmp (argv[counter], "-v") == 0 ||
	    strcmp (argv[counter], "--version") == 0)
	{
	  fprintf (stderr, 
	    "fastcsv2json %d.%d.%d\n", 
        VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);	
      fprintf (stderr, "Copyright (C) 2020 Lucas Tsatiris.\n\n");  
      fprintf (stderr, 
        "This is free software.  You may redistribute copies of it under the terms of\n");
      fprintf (stderr,   
        "the GNU General Public License <https://www.gnu.org/licenses/gpl.html>.\n");
      fprintf (stderr,   
        "There is NO WARRANTY, to the extent permitted by law.\n\n");
      fprintf (stderr,   
        "Written by Lucas Tsatiris.\n");  
	  return 1;	
    }	    
    else
    /* delimiter */
	if (strcmp (argv[counter], "-d") == 0 ||
	    strcmp (argv[counter], "--delimiter") == 0)
	{
	  counter ++;
	  if (counter < argc)
	  {
		if (strcmp (argv[counter], "pipe") == 0)
		  delimiter[0] = '|';
		else
		if (strcmp (argv[counter], "comma") == 0)
		  delimiter[0] = ',';
		else
		if (strcmp (argv[counter], "semicolumn") == 0) 
		  delimiter[0] = ';';      
		else
		if (strcmp (argv[counter], "column") == 0) 
		  delimiter[0] = ':';
		else  
		if (strcmp (argv[counter], "tab") == 0) 
		  delimiter[0] = 9;     
		else
		{
		  fprintf (stderr, "unknown delimiter\n");
		  return 1;  	
	    }	
      }
      else
      {
		fprintf (stderr, "no argument found\n");
		return 1;  
      }	  	
    }
    else
    /* MAX_COLUMN_SIZE */  
    if (strcmp (argv[counter], "-s") == 0 ||
	    strcmp (argv[counter], "--max-column-size") == 0)
	{
	  counter ++;
	  if (counter < argc)
	  {  
		MAX_COLUMN_SIZE = atoi(argv[counter]) + 1;
		if (MAX_COLUMN_SIZE < COLUMN_SIZE_LOWER_LIMIT)
		  MAX_COLUMN_SIZE = COLUMN_SIZE_LOWER_LIMIT; 
		if (MAX_COLUMN_SIZE > COLUMN_SIZE_UPPER_LIMIT)
		  MAX_COLUMN_SIZE = COLUMN_SIZE_UPPER_LIMIT;   	
      }	    
      else
      {
		fprintf (stderr, "no argument found\n");
		return 1;  
      }
    }
    else
    /* MAX_COLUMNS */  
    if (strcmp (argv[counter], "-c") == 0 ||
	    strcmp (argv[counter], "--max-columns") == 0)
	{
	  counter ++;
	  if (counter < argc)
	  {  
		MAX_COLUMNS = atoi(argv[counter]);
		if (MAX_COLUMNS < 1)
		  MAX_COLUMNS = 1; 
		if (MAX_COLUMNS > COLUMNS_UPPER_LIMIT)
		  MAX_COLUMNS = COLUMNS_UPPER_LIMIT;   	
      }	    
      else
      {
		fprintf (stderr, "no argument found\n");
		return 1;  
      }
    }
    else
    {
	  fprintf (stderr, "invalid argument: %s\n", argv[counter]);
	  return 1;  
    }	  
    counter ++;		      
  }	  
  return result;	
}

/* as it says */
int
main (int argc, char *argv[])
{
  if (parse_arguments (argc, argv) == 0)
    return generate_json ();
  
  return 1;  
}

