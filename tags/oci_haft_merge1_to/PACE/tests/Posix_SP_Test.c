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

#if PACE_LYNXOS != 0x300
#include "pace/stdio.h"
#include "pace/unistd.h"
#include "pace/time.h"
#include "pace/sys/utsname.h"

const char * success = "SUCCEEDED";
const char * failure = "***FAILED***";

#if defined (PACE_VXWORKS) && PACE_VXWORKS != 0
#include "vxworks_stub.c"
#endif /* VXWORKS */

void
check_sysconf ()
{
  long retval;
  int index;
#if PACE_HAS_POSIX == PACE_LYNXOS
  const int confnamessize = 30;
#else
  const int confnamessize = 38;
#endif /* PACE_HAS_POSIX != PACE_LYNXOS */

  int confnames[] = {_SC_AIO_LISTIO_MAX,
                     _SC_AIO_MAX,
                     _SC_AIO_PRIO_DELTA_MAX,
                     _SC_ARG_MAX,
                     _SC_CHILD_MAX,
                     _SC_CLK_TCK,
                     _SC_DELAYTIMER_MAX,
#if PACE_HAS_POSIX != PACE_LYNXOS
                     _SC_GETGR_R_SIZE_MAX,
                     _SC_GETPW_R_SIZE_MAX,
                     _SC_LOGIN_NAME_MAX,
#endif /* PACE_HAS_POSIX != PACE_LYNXOS */
                     _SC_MQ_OPEN_MAX,
                     _SC_MQ_PRIO_MAX,
                     _SC_NGROUPS_MAX,
                     _SC_OPEN_MAX,
                     _SC_PAGESIZE,
                     _SC_RTSIG_MAX,
                     _SC_SEM_NSEMS_MAX,
                     _SC_SEM_VALUE_MAX,
                     _SC_SIGQUEUE_MAX,
                     _SC_STREAM_MAX,
#if PACE_HAS_POSIX != PACE_LYNXOS
                     _SC_THREAD_DESTRUCTOR_ITERATIONS,
                     _SC_THREAD_KEYS_MAX,
                     _SC_THREAD_STACK_MIN,
                     _SC_THREAD_THREADS_MAX,
#endif /* PACE_HAS_POSIX != PACE_LYNXOS */
                     _SC_TIMER_MAX,
#if PACE_HAS_POSIX != PACE_LYNXOS
                     _SC_TTY_NAME_MAX,
#endif /* PACE_HAS_POSIX != PACE_LYNXOS */
                     _SC_TZNAME_MAX,
                     _SC_ASYNCHRONOUS_IO,
                     _SC_FSYNC,
                     _SC_JOB_CONTROL,
                     _SC_MAPPED_FILES,
                     _SC_MEMLOCK,
                     _SC_MEMLOCK_RANGE,
                     _SC_MEMORY_PROTECTION,
                     _SC_MESSAGE_PASSING,
                     _SC_PRIORITIZED_IO,
                     _SC_PRIORITY_SCHEDULING,
                     _SC_REALTIME_SIGNALS};
  char * confstrs[] = {"_SC_AIO_LISTIO_MAX",
                       "_SC_AIO_MAX",
                       "_SC_AIO_PRIO_DELTA_MAX",
                       "_SC_ARG_MAX",
                       "_SC_CHILD_MAX",
                       "_SC_CLK_TCK",
                       "_SC_DELAYTIMER_MAX",
#if PACE_HAS_POSIX != PACE_LYNXOS
                       "_SC_GETGR_R_SIZE_MAX",
                       "_SC_GETPW_R_SIZE_MAX",
                       "_SC_LOGIN_NAME_MAX",
#endif /* PACE_HAS_POSIX != PACE_LYNXOS */
                       "_SC_MQ_OPEN_MAX",
                       "_SC_MQ_PRIO_MAX",
                       "_SC_NGROUPS_MAX",
                       "_SC_OPEN_MAX",
                       "_SC_PAGESIZE",
                       "_SC_RTSIG_MAX",
                       "_SC_SEM_NSEMS_MAX",
                       "_SC_SEM_VALUE_MAX",
                       "_SC_SIGQUEUE_MAX",
                       "_SC_STREAM_MAX",
#if PACE_HAS_POSIX != PACE_LYNXOS
                       "_SC_THREAD_DESTRUCTOR_ITERATIONS",
                       "_SC_THREAD_KEYS_MAX",
                       "_SC_THREAD_STACK_MIN",
                       "_SC_THREAD_THREADS_MAX",
#endif /* PACE_HAS_POSIX != PACE_LYNXOS */
                       "_SC_TIMER_MAX",
#if PACE_HAS_POSIX != PACE_LYNXOS
                       "_SC_TTY_NAME_MAX",
#endif /* PACE_HAS_POSIX != PACE_LYNXOS */
                       "_SC_TZNAME_MAX",
                       "_SC_ASYNCHRONOUS_IO",
                       "_SC_FSYNC",
                       "_SC_JOB_CONTROL",
                       "_SC_MAPPED_FILES",
                       "_SC_MEMLOCK",
                       "_SC_MEMLOCK_RANGE",
                       "_SC_MEMORY_PROTECTION",
                       "_SC_MESSAGE_PASSING",
                       "_SC_PRIORITIZED_IO",
                       "_SC_PRIORITY_SCHEDULING",
                       "_SC_REALTIME_SIGNALS"};

  PACE_UNUSED_ARG (confstrs);

  /* Success indicated by compilation for this function. */
  /* printf("pace_sysconf %s\n", success); */
  for (index = 0; index < confnamessize; index++) 
    {
      /* Call pace_sysconf() */
      errno = 0;
      retval = pace_sysconf(confnames[index]);

      /* Return value of -1 indicates an invalid name or no support
         for the functionality associated with the name. POSIX-
         conformant platforms may not support all the names. However,
         using the names above should not cause compilation errors.
         */
      /* printf("pace_sysconf: name == %s\n", confstrs[index]); */
      if (retval == -1)
        {
          /* printf("retval == %ld\n", retval); */
          if (errno == 0)
            {
              /* printf("Unsupported name [errno did not change]\n"); */
            }
          else
            {
              printf("invalid name [errno did not change]\n");
            }
        }
      else
        {
          /* printf("pace_sysconf: value == %ld\n", retval); */
        }
    }

  /* printf("\n"); */
}

void
check_time ()
{
  long retval;
  time_t local_time;

  /* Call pace_time() with time_t * arg */
  local_time = 0;
  errno = 0;
  retval = pace_time(&local_time);

  /* printf("pace_time %s\n", success); */
  if (retval == -1)
    {
      printf("errno == %d\n", errno);
    }
  else
    {
      /* printf("pace_time (with time_t* arg) == %ld\n", local_time); */
    }

  /* Call pace_time() with NULL arg */
  errno = 0;
  retval = pace_time(0);

  if (retval == -1)
    {
      printf("errno == %d\n", errno);
    }
  else
    {
      /* printf("pace_time (with NULL arg) == %ld\n\n", retval); */
    }
}

void
check_uname ()
{
  long retval;
  struct utsname name;

  /* Call pace_uname() */
  errno = 0;
  retval = pace_uname(&name);
  /* printf("pace_uname %s\n", success); */

  if (retval < 0)
    {
      printf("pace_uname error: %d\n", errno);
    }
  else
    {
      /* printf("pace_uname: uname.sysname == %s\n", name.sysname); */
      /* printf("pace_uname: uname.nodename == %s\n", name.nodename); */
      /* printf("pace_uname: uname.release == %s\n", name.release); */
      /* printf("pace_uname: uname.version == %s\n", name.version); */
      /* printf("pace_uname: uname.machine == %s\n", name.machine); */
    }
}

int
main (int argc, char **argv)
{
  /* Check the PACE calls to the sysconf POSIX function. */
  check_sysconf();

  /* Check the PACE calls to the time POSIX function. */
  check_time();

  /* Check the PACE calls to the uname POSIX function. */
  check_uname();

  PACE_UNUSED_ARG (argc);
  PACE_UNUSED_ARG (argv);
  return 0;
}
#else
int
main (int argc, char **argv)
{
  printf("PACE does not support LynxOS 3.0.0.\n");
}
#endif /* PACE_LYNXOS == 0x300 */
