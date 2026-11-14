#include <cstdio>
#include <cstdlib>

#include "ace/OS_main.h"

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  double sum {};
  double count {};
  char buf[BUFSIZ] {};

  while (std::fgets (buf, sizeof (buf), stdin) != nullptr)
    {
      int const input = std::atoi (buf);
      sum += input;
      ++count;
    }

  std::printf ("sum of input is: %f\n", sum);
  std::printf ("number of inputs is: %f\n", count);
  std::printf ("average of input is: %f\n", sum / count);

  return 0;
}
