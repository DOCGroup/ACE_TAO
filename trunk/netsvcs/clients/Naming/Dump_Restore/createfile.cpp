#include <stdio.h>
// $Id$

#include <string.h>
#include <math.h>

int 
main (int argc, char **argv)
{
  FILE *infile, *outfile;
  char buf[BUFSIZ];

  if ((infile = fopen (argv[1], "r")) == NULL)
    return -1;

  if ((outfile = fopen (argv[2], "w")) == NULL)
    return -1;

  int count = 0;
  while (::fgets (buf, BUFSIZ, infile))
    {
      buf[::strlen(buf) - 1] = '\0';
      fputs (buf, outfile);
      if (count % 2 == 0)
	fputs (" ", outfile);
      else
	fputs ("\n", outfile);
      count++;
    }
  fclose (outfile);
  fclose (infile);
}
