// $Id: test.cpp 1.1	10/18/96

// Tests the generated perfect has function.
// The -v option prints diagnostics as to whether a word is in 
// the set or not.  Without -v the program is useful for timing.
  
#include <stdio.h>
#include <string.h>

static const int MAX_LEN = 80;

char *in_word_set (char *str, unsigned int len);

int 
main (int argc, char *argv[])
{
  int verbose = argc > 1 && strcmp (argv[1], "-v") ? 1 : 0;
  char buf[MAX_LEN];

  while (fgets (buf, sizeof buf, stdin) != 0) 
    if (in_word_set (buf, strlen (buf)) && verbose) 
      printf ("in word set %s\n", buf);
    else if (verbose) 
      printf ("NOT in word set %s\n", buf);

  return 0;
}
