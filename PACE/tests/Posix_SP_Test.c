/* $Id$ -*- C -*- */

/* =====================================================================
 *
 * = FILENAME
 *    Posix_SP_Test.c
 *
 * =  DESCRIPTION
 *     Testing the platform for POSIX_SINGLE_PROCESS unit of
 *     functionality. This consists of calls to sysconf, time, and
 *     uname.
 *     This is not meant to be an exhaustive test at this point but more
 *     a sanity check that PACE works (at least somewhat).
 *
 * = AUTHOR
 *    Joe Hoffert <joeh@cs.wustl.edu>
 *
 * =====================================================================
 */

#include "pace/stdio.h"
#include "pace/unistd.h"
#include "pace/time.h"
#include "pace/sys/utsname.h"

const char * success = "SUCCEEDED";
const char * failure = "***FAILED***";

int
main (int argc, char **argv)
{
  long retval;
  time_t local_time;
  struct utsname name;
  int confname = _SC_AIO_LISTIO_MAX;

  /* Call pace_sysconf() */
  errno = 0;
  retval = pace_sysconf(confname);

  /* Return value of -1 indicates an invalid name or no support
     for the functionality associated with the name. In either
     case, POSIX platforms should not return -1 for the name
     above.
   */
  if (retval == -1)
    {
      printf("pace_sysconf %s\n", failure);
      printf("retval == %ld\n", retval);
      if (errno == 0)
        {
          printf("Unsupported name [errno did not change]");
        }
      printf("\n");
      return -1;
    }

  printf("pace_sysconf %s\n", success);
  printf("pace_sysconf: name == %d\n", confname);
  printf("pace_sysconf: value == %ld\n\n", retval);

  /* Call pace_time() */
  local_time = 0;
  errno = 0;
  retval = pace_time(&local_time);

  if (retval == -1)
    {
      printf("pace_time %s\n", failure);
      printf("errno == %d\n\n", errno);
      return -1;
    }

  printf("pace_time %s\n", success);
  printf("pace_time == %ld\n\n", local_time);

  /* Call pace_uname() */
  retval = pace_uname(&name);

  if (retval < 0)
    {
      printf("pace_uname %s\n\n", failure);
      return -1;
    }

  printf("pace_uname %s\n", success);
  printf("pace_uname: uname.sysname == %s\n\n", name.sysname);

  PACE_UNUSED_ARG (argc);
  PACE_UNUSED_ARG (argv);
  return 0;
}
