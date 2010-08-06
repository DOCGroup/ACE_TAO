#include <iostream>
#include <sstream>
#include <iterator>
#include <algorithm>
#include "Combination_T.h"
#include "Primary_Scheduler.h"

typedef std::list <Processor> PGROUP;

int main (int, char* [])
{
  unsigned int m = 6;
  PGROUP procs;
  PGROUP perm;
  for (unsigned int i = 1; i <= m; ++i)
    {
      std::stringstream ss;
      ss << "P" << i;
      procs.push_back (ss.str ());
    }

  do
    {
      std::copy (perm.begin (),
                 perm.end (),
                 std::ostream_iterator <std::string> (std::cout, " "));
      std::cout << std::endl;
    }
  while (next_combination (procs.begin (),
                           procs.end (),
                           perm.begin (),
                           perm.end ()));

  return 0;
}
