/* Driver program for the Gen_Perf hash function generator Copyright
// $Id$

   (C) 1989 Free Software Foundation, Inc.  written by Douglas
   C. Schmidt (schmidt@ics.uci.edu)

This file is part of GNU GPERF.

GNU GPERF is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 1, or (at your option)
any later version.

GNU GPERF is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU GPERF; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA 02111, USA.  */

// Simple driver program for the Gen_Perf.hash function generator.
// All the hard work is done in class Gen_Perf and its class methods.

#include "Options.h"
#include "Gen_Perf.h"

int
main (int argc, char *argv[])
{

  struct tm *tm;
  time_t     clock; 

  time (&clock);
  tm = localtime (&clock);
  printf ("/* starting time is %d:%02d:%02d */\n", tm->tm_hour, tm->tm_min, tm->tm_sec);

#if defined(RLIMIT_STACK) && LARGE_STACK_ARRAYS
  /* Get rid of any avoidable limit on stack size.  */
  {
    struct rlimit rlim;

    /* Set the stack limit huge so that alloca does not fail. */
    getrlimit (RLIMIT_STACK, &rlim);
    rlim.rlim_cur = rlim.rlim_max;
    setrlimit (RLIMIT_STACK, &rlim);
  }
#endif /* RLIMIT_STACK */

  /* Sets the Options. */
  option (argc, argv);          

  // Initializes the key word list.
  Gen_Perf table;

  // Generates and prints the Gen_Perf hash table.  Don't use exit
  // here, it skips the destructors.
  int status = table.generate ();     

  time (&clock);
  tm = localtime (&clock);
  printf ("/* ending time is %d:%02d:%02d */\n", tm->tm_hour, tm->tm_min, tm->tm_sec);
  return status;
}
