#include <cstdlib>

#include "ace/OS_main.h"
#include "ace/OS_NS_stdio.h"

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  double sum {};
  double count {};
  int input {};
  char buf[BUFSIZ] {};

  while (ACE_OS::fgets (buf, sizeof (buf), stdin) != nullptr)
    {
      input = std::atoi (buf);
      sum += input;
      count++;
    }

  ACE_OS::printf ("sum of input is: %f\n", sum);
  ACE_OS::printf ("number of inputs is: %f\n", count);
  ACE_OS::printf ("average of input is: %f\n", sum / count);

  return 0;
}
