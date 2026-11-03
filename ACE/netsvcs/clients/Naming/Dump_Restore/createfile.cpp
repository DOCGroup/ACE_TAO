#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/ACE.h"


int
ACE_TMAIN(int, ACE_TCHAR *argv[])
{
  FILE * const infile = ACE_OS::fopen (argv[1], "r");
  char buf[BUFSIZ];

  if (infile == nullptr)
    return -1;

  FILE * const outfile = ACE_OS::fopen (argv[2], "w");
  if (outfile == nullptr)
    return -1;

  int count = 0;
  while (ACE_OS::fgets (buf, BUFSIZ, infile))
    {
      buf[ACE_OS::strlen(buf) - 1] = '\0';
      ACE_OS::fputs (buf, outfile);
      if (count % 2 == 0)
        ACE_OS::fputs (" ", outfile);
      else
        ACE_OS::fputs ("\n", outfile);
      count++;
    }

  ACE_OS::fclose (outfile);
  ACE_OS::fclose (infile);
  return 0;
}
