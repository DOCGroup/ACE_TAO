/* $Id$ -*- C -*- */

/* =====================================================================
 *
 * = FILENAME
 *    Posix_SP_Test.c
 *
 * =  DESCRIPTION
 *     Testing the platform for POSIX_SINGLE_PROCESS unit of
 *     functionality. This consists of calls to execl, execle, execlp,
 *     execv, execve, execvp, fork, getenv, getpid, getppid, sleep,
 *     times, wait, waitpid, and _exit.
 *     This is not meant to be an exhaustive test at this point but more
 *     a sanity check that PACE works (at least somewhat) as advertised.
 *
 * = AUTHOR
 *    Joe Hoffert <joeh@cs.wustl.edu>
 *
 * =====================================================================
 */

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

  /* Call pace_sysconf() */
  retval = pace_sysconf(_SC_2_C_BIND);

  if (retval != _POSIX2_C_BIND)
    {
      printf("pace_sysconf %s\n", failure);
      printf("retval == %ld\n", retval);
      return -1;
    }

  printf("pace_sysconf %s\n", success);

  /* Call pace_time() */
  local_time = 0;
  errno = 0;
  retval = pace_time(&local_time);

  if (retval == -1)
    {
      printf("pace_time %s\n", failure);
      printf("errno == %d\n", errno);
      return -1;
    }

  printf("pace_time %s\n", success);

  /* Call pace_uname() */
  retval = pace_uname(&name);

  if (retval <= -1)
    {
      printf("pace_uname %s\n", failure);
      return -1;
    }

  printf("pace_uname %s\n", success);

  return 0;
}
