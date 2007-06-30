// $Id$
#include "ace/OS.h"

int
main (void)
{
  double sum = 0;
  double count = 0;
  int input;
  char buf[BUFSIZ];

  while (ACE_OS::fgets (buf, sizeof (buf), stdin) != 0)
    {
      input = ACE_OS::atoi (buf);
      sum += input;
      count++;
    }

  ACE_OS::printf ("sum of input is: %f\n", sum);
  ACE_OS::printf ("number of inputs is: %f\n", count);
  ACE_OS::printf ("average of input is: %f\n", sum / count);

  return 0;
}
