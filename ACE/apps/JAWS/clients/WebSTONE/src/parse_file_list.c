/**************************************************************************
 *
 *  Copyright (C) 1995 Silicon Graphics, Inc.
 *
 *  These coded instructions, statements, and computer programs were
 *  developed by SGI for public use.  If any changes are made to this code
 *  please try to get the changes back to the author.  Feel free to make
 *  modifications and changes to the code and release it.
 *
 **************************************************************************/
#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#ifndef WIN32
#include <netdb.h>
#include <sys/param.h>
#endif /* WIN32 */

#include <sys/types.h>

#ifndef WIN32
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#else
#include <windows.h>
#include <process.h>
#include <time.h>
#include <winsock.h>
#endif /* WIN32 */

#include <ctype.h>

#include "sysdep.h"
#include "bench.h"


/*
 * count_file_list()
 * given a filename, return a guess at the number of pages
 */
int
count_file_list(const char *url_list_file)
{
  FILE *fp;
  char a_line[BUFSIZ];
  long int num_of_pages = 0;

  fp = fopen(url_list_file, "r");

  if (fp == 0)
    {
      D_PRINTF("Error %d opening filelist %s: %s\n",
               errno, url_list_file, strerror(errno) );
      return(returnerr("Error %d opening filelist %s: %s\n",
             errno, url_list_file, strerror(errno)));
    }

  D_PRINTF( "Opened file, about to count\n" );
  /* Count non-empty URLs after stripping comments. */
  while (fgets(a_line, sizeof(a_line), fp) != 0)
    {
      char *textvalue = a_line;
      char *comment = strchr(textvalue, '#');

      if (strchr(a_line, '\n') == 0 && fgetc(fp) != EOF)
        {
          fclose(fp);
          errexit("Filelist line exceeds maximum length\n");
        }

      if (comment != 0)
        {
          *comment = '\0';
        }

      while (isspace((unsigned char)*textvalue))
        {
          ++textvalue;
        }

      if (*textvalue == '\0')
        {
          continue;
        }

      if (num_of_pages >= MAXNUMOFPAGES)
        {
          fclose(fp);
          errexit("Filelist exceeds the maximum of %d pages\n",
                  MAXNUMOFPAGES);
        }

      ++num_of_pages;
      D_PRINTF( "Found %ld pages\n", num_of_pages );
    }

  if (ferror(fp))
    {
      int error = errno;
      fclose(fp);
      return returnerr("Error reading filelist %s: %s\n",
                       url_list_file, strerror(error));
    }

  if (fclose(fp) != 0)
    {
      return returnerr("Error closing filelist %s: %s\n",
                       url_list_file, strerror(errno));
    }

  return (int)num_of_pages;
} /* end count_file_list() */

/*
 * parse_file_list()
 * given a filename, a pointer to a page list, and pointers to integers
 * for the number of pages and the number of files, parse its contents.
 */
void
parse_file_list (const char *url_list_file, page_list_t *page_list,
                 long int *num_of_pages, long int *num_of_files)
{
  FILE        *fp;
  int         loadnum;
  char        a_line[BUFSIZ];
  char        tempbuf[URL_SIZE + 1];
  int         numfiles = 1, numpages = 0;
  page_list_t *pp;

  extern int haveproxyserver;

  fp = fopen(url_list_file, "r");

  if (fp == 0)
    {
      errexit("Error %d opening filelist: %s\n", errno, strerror(errno));
    }
  else
    {
      /*
       * GRAB A LINE. FORMAT IS: URL WEIGHTINGFACTOR
       * EXAMPLE: http://www/file.html 1
       */
      D_PRINTF( "File is open.\n" );
      while(fgets(a_line, BUFSIZ, fp) != 0)
        {
          char *textvalue = a_line;
          int returnval;

          /* check for comments */
          if (strchr(textvalue, '#') != 0)
            {
              /* throw out anything after a '#' */
              D_PRINTF( "Stripping comment from line: %s\n", textvalue );
              returnval = strcspn(textvalue, "#");
              textvalue[returnval] = '\0';
              D_PRINTF( "Found first # at %d\n", returnval );
              if (returnval == 0)
                continue;
            }

          if (strchr(a_line, '\n') == 0 && fgetc(fp) != EOF)
            {
              errexit("Filelist line exceeds maximum length\n");
            }

          if (numpages >= *num_of_pages)
          errexit("Out of space in parse_file_list()\n");

          pp = &page_list[numpages];

          D_PRINTF( "Processing page %ld\n", numpages );
          loadnum = 0;

          if (textvalue != 0)
            {
              /* is there more? */
              /* check for weighting factor */
              D_PRINTF( "Setting page values from: %s\n", textvalue );
              returnval = sscanf(textvalue, "%1024s%d", tempbuf, &loadnum);
              if (returnval < 1)
                continue;
              if (strlen(tempbuf) >= URL_SIZE)
                errexit("Filelist URL exceeds maximum length\n");
              D_PRINTF("Scan for weighting returns %d, %d\n",
                       returnval, loadnum);
              if (returnval == EOF || loadnum <= 0)
                {
                  pp->load_num = 1;
                }
              else
                {
                  pp->load_num = loadnum;
                }
              D_PRINTF("Setting load=%d for line: %s\n",
                       pp->load_num, textvalue);

              /* placeholder for grouping multiple files onto one page */
              pp->num_of_files = 1;
              filenum = 0;

              textvalue = tempbuf;
              D_PRINTF( "Line is now: %s\n", textvalue );

              /*
              * if we've got a proxy server, we'll assume that the
              * remaining text is a valid URL, and stuff it into
              * page_list[numpages].filename[filenum]
              * Otherwise, we'll have to parse it out.
              */

              if (haveproxyserver)
                {
                  pp->servername[filenum] = 0;
                  pp->port_number[filenum] = 0;
                  strcpy(pp->filename[filenum], textvalue);
                }
              else /* no proxy server, so we have to parse it out... */
                {
                  char hostbuf[URL_SIZE + 1];
                  char pathbuf[URL_SIZE + 1];

                  /* try http://server(:port)/file */
                  D_PRINTF( "Trying http://server(:port)/filename\n" );
                  returnval = sscanf(textvalue,
                                     "http://%1024[^/]%1024s",
                                     hostbuf, pathbuf);
                  /* check server string for :port */
                  if (returnval == 2)
                    {
                      char serverbuf[URL_SIZE + 1];

                      if (strlen(hostbuf) >= URL_SIZE ||
                          strlen(pathbuf) >= URL_SIZE)
                        errexit("Filelist URL component exceeds maximum length\n");

                      D_PRINTF( "Setting filename %s\n", pathbuf );
                      strcpy(pp->filename[filenum], pathbuf);

                      D_PRINTF( "Checking %s for :portnumber\n", hostbuf );
                      returnval = sscanf(hostbuf, "%1024[^:]:%d", serverbuf,
                                         &pp->port_number[filenum]);

                      if (returnval < 2)
                        {
                          pp->port_number[filenum] = 80;
                        }
                      if (returnval < 1)
                        {
                          errexit("Invalid server in filelist URL\n");
                        }
                      else
                        {
                          D_PRINTF("Port number %d, setting server %s\n",
                                   pp->port_number[filenum], serverbuf);

                          if (strlen(serverbuf) >= URL_SIZE)
                            errexit("Filelist server exceeds maximum length\n");
                          strcpy(pp->servername[filenum], serverbuf);
                        }

                      D_PRINTF("Server %s, port number %d\n",
                               pp->servername[filenum],
                               pp->port_number[filenum]);
                    }
                  else /* no good - try straight filename */
                    {
                      pp->port_number[filenum] = 80;
                      D_PRINTF( "Trying filename, returnval=%d\n", returnval );
                      pp->servername[filenum] = 0;
                      D_PRINTF("Server %s, port number %d\n",
                               pp->servername[filenum], pp->port_number[filenum]);
                      strcpy(pathbuf, textvalue);
                      D_PRINTF( "Scan returned filename %s\n", pathbuf );

                      strcpy(pp->filename[filenum], pathbuf);
                    } /* end of parsing */
                } /* end if haveproxyserver */

              D_PRINTF("Done parsing line\n");
              D_PRINTF("Got server %s, port %d, file %s, returnval %d\n",
                       pp->servername[filenum],
                       pp->port_number[filenum],
                       pp->filename[filenum],
                       returnval);
            } /* end if textvalue not NULL */

          ++numpages;
        } /* end while not EOF */
      if (numpages < 1)
        {
          returnerr("No files are specified by filelist\n");
        }
    } /* end if file ok */
    fclose(fp);
    D_PRINTF("Returning %ld pages and %ld files\n",
             numpages, numfiles );

    *num_of_pages = numpages;
    *num_of_files = numfiles;
}
/* end parse_file_list */

long int
load_percent(page_list_t *page_list, long int number_of_pages)
{
  int i;
  long int index_number = 0;

  for (i = 0; i < number_of_pages; ++i)
    {
      index_number += page_list[i].load_num;
    }

  D_PRINTF( "load_percent returning %d\n", (index_number) );
  return(index_number);
}

