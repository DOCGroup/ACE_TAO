//=============================================================================
/**
 *  @file    Aio_Platform_Test_C.cpp
 *
 *  $Id$
 *
 *  Testing the platform for POSIX Asynchronous I/O. This is the C
 *  version of the  $ACE_ROOT/tests/Aio_Platform_Test.cpp. Useful
 *  to send bug reports.
 *
 *  @author Programming for the Real World. Bill O. GallMeister. Modified by Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */
//=============================================================================



#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include <limits.h>

#include <aio.h>

int do_sysconf (void);
int have_asynchio (void);

static int file_handle = -1;
char mb1 [BUFSIZ + 1];
char mb2 [BUFSIZ + 1];
aiocb aiocb1, aiocb2;
sigset_t completion_signal;

// For testing the <aio> stuff.
int test_aio_calls (void);
int issue_aio_calls (void);
int query_aio_completions (void);
int setup_signal_delivery (void);
int do_sysconf (void);
int have_asynchio (void);

int
do_sysconf (void)
{
  // Call sysconf to find out runtime values.
  errno = 0;
#if defined (_SC_LISTIO_AIO_MAX)
  printf ("Runtime value of LISTIO_AIO_MAX is %d, errno = %d\n",
          sysconf(_SC_LISTIO_AIO_MAX),
          errno);
#else
  printf ("Runtime value of AIO_LISTIO_MAX is %d, errno = %d\n",
          sysconf(_SC_AIO_LISTIO_MAX),
          errno);
#endif

  errno = 0;
  printf ("Runtime value of AIO_MAX is %d, errno = %d\n",
          sysconf (_SC_AIO_MAX),
          errno);

  errno = 0;
  printf ("Runtime value of _POSIX_ASYNCHRONOUS_IO is %d, errno = %d\n",
          sysconf (_SC_ASYNCHRONOUS_IO),
          errno);

  errno = 0;
  printf ("Runtime value of _POSIX_REALTIME_SIGNALS is %d, errno = %d\n",
          sysconf (_SC_REALTIME_SIGNALS),
          errno);

  errno = 0;
  printf ("Runtime value of RTSIG_MAX %d, Errno = %d\n",
          sysconf (_SC_RTSIG_MAX),
          errno);

  errno = 0;
  printf ("Runtime value of SIGQUEUE_MAX %d, Errno = %d\n",
          sysconf (_SC_SIGQUEUE_MAX),
          errno);
  return 0;
}

int
have_asynchio (void)
{
#if defined (_POSIX_ASYNCHRONOUS_IO)
  // POSIX Asynch IO is present in this system.
#if defined (_POSIX_ASYNC_IO)
  // If this is defined and it is not -1, POSIX_ASYNCH is supported
  // everywhere in the system.
#if _POSIX_ASYNC_IO == -1
  printf ("_POSIX_ASYNC_IO = -1.. ASYNCH IO NOT supported at all\n");
  return -1;
#else /* Not _POSIX_ASYNC_IO == -1 */
  printf ("_POSIX_ASYNC_IO = %d\n ASYNCH IO is supported FULLY\n",
          _POSIX_ASYNC_IO);
#endif /* _POSIX_ASYNC_IO == -1 */

#else  /* Not defined  _POSIX_ASYNC_IO */
  printf ("_POSIX_ASYNC_IO is not defined.\n");
  printf ("AIO might *not* be supported on some paths\n");
#endif /* _POSIX_ASYNC_IO */

  // System defined POSIX Values.
  printf ("System claims to have  POSIX_ASYNCHRONOUS_IO\n");

  printf ("_POSIX_AIO_LISTIO_MAX = %d\n", _POSIX_AIO_LISTIO_MAX);
  printf ("_POSIX_AIO_MAX = %d\n", _POSIX_AIO_MAX);

  // Check and print the run time values.
  do_sysconf ();

  return 0;

#else /* Not _POSIX_ASYNCHRONOUS_IO */
  printf ("No support._POSIX_ASYNCHRONOUS_IO itself is not defined\n");
  return -1;
#endif /* _POSIX_ASYNCHRONOUS_IO */
}

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  if (have_asynchio () == 0)
    printf ("Test successful\n");
  else
    printf ("Test not successful\n");
  return 0;
}
