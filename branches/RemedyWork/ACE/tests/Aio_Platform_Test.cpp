// $Id$

// ============================================================================
//
// = FILENAME
//    aio_platform_test.cpp
//
// =  DESCRITPTION
//     Testing the platform for POSIX Asynchronous I/O. Basically
//     prints the predefined constants and also checks for their run
//     time values. If this test succeeds further tests at
//     $ACE_ROOT/examples/Reactor/Proactor can be used to test the
//     features further.
//
// = AUTHOR
//    Programming for the Real World. Bill O. GallMeister.  Modified
//    by Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// =====================================================================

#include "test_config.h"
#include "ace/OS_NS_unistd.h"

#if defined (_POSIX_ASYNCHRONOUS_IO)
static int do_sysconf (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "Doing <sysconf> calls to know the run-time values of POSIX feature limits\n"));

  // Call sysconf to find out runtime values.
  errno = 0;
#if defined (_SC_LISTIO_AIO_MAX)
  ACE_DEBUG ((LM_DEBUG,
              "Runtime value of LISTIO_AIO_MAX is %d, errno = %d, Minimum is 2\n",
              ACE_OS::sysconf (_SC_LISTIO_AIO_MAX),
              errno));
#elif defined (_SC_AIO_LISTIO_MAX)
  ACE_DEBUG ((LM_DEBUG,
              "Runtime value of AIO_LISTIO_MAX is %d, errno = %d, Minimum is 2\n",
              ACE_OS::sysconf (_SC_AIO_LISTIO_MAX),
              errno));
#else
  ACE_ERROR ((LM_ERROR,
              "_SC_LISTIO_AIO_MAX or _SC_AIO_LISTIO_MAX"
              " do not exist on this platform\n"));
#endif /* _SC_LISTIO_AIO_MAX */

#if defined (_SC_AIO_MAX)
  errno = 0;
  ACE_DEBUG ((LM_DEBUG,
              "Runtime value of AIO_MAX is %d, errno = %d, Minimum is 1\n",
              ACE_OS::sysconf (_SC_AIO_MAX),
              errno));
#else
  ACE_ERROR ((LM_ERROR,
              "_SC_AIO_MAX does not exist on this platform\n"));
#endif /* _SC_AIO_MAX */

#if defined (_SC_ASYNCHRONOUS_IO)
  errno = 0;
  ACE_DEBUG ((LM_DEBUG,
              "Runtime value of _POSIX_ASYNCHRONOUS_IO is %d, errno = %d\n",
              ACE_OS::sysconf (_SC_ASYNCHRONOUS_IO),
              errno));
#else /* Not _SC_ASYNCHRONOUS_IO */
  ACE_ERROR ((LM_ERROR,
              "_SC_ASYNCHRONOUS_IO does not exist on this platform\n"));
#endif /* _SC_ASYNCHRONOUS_IO */

#if defined (_SC_REALTIME_SIGNALS)
  errno = 0;
  ACE_DEBUG ((LM_DEBUG,
              "Runtime value of _POSIX_REALTIME_SIGNALS is %d, errno = %d\n",
              ACE_OS::sysconf (_SC_REALTIME_SIGNALS),
              errno));
#else /* Not _SC_REALTIME_SIGNALS */
  ACE_ERROR ((LM_ERROR,
              "_SC_REALTIME_SIGNALS does not exist on this platform\n"));
#endif /* _SC_REALTIME_SIGNALS */


#if defined (_SC_RTSIG_MAX)
  errno = 0;
  ACE_DEBUG ((LM_DEBUG,
              "Runtime value of RTSIG_MAX %d, Errno = %d, Minimum is 8\n",
              ACE_OS::sysconf (_SC_RTSIG_MAX),
              errno));
#else /* Not _SC_RTSIG_MAX */
  ACE_ERROR ((LM_ERROR,
              "_SC_RTSIG_MAX does not exist on this platform\n"));
#endif /* _SC_RTSIG_MAX */

#if defined (_SC_SIGQUEUE_MAX)
  errno = 0;
  ACE_DEBUG ((LM_DEBUG,
              "Runtime value of SIGQUEUE_MAX %d, Errno = %d, Minimum is 32\n",
              ACE_OS::sysconf (_SC_SIGQUEUE_MAX),
              errno));
#else /* Not _SC_SIGQUEUE_MAX */
  ACE_ERROR ((LM_ERROR,
              "_SC_SIGQUEUE_MAX does not exist on this platform\n"));
#endif /*  _SC_SIGQUEUE_MAX */
  return 0;
}
#endif /* _POSIX_ASYNCHRONOUS_IO */

static int
have_asynchio (void)
{
#if defined (_POSIX_ASYNCHRONOUS_IO)
#if defined (_POSIX_ASYNC_IO)
#if _POSIX_ASYNC_IO == -1
  ACE_DEBUG ((LM_DEBUG,
              "_POSIX_ASYNC_IO = -1.. ASYNCH IO NOT supported at all\n"));
  return -1;
#else /* Not _POSIX_ASYNC_IO == -1 */
  ACE_DEBUG ((LM_DEBUG,
              "_POSIX_ASYNC_IO = %d\n ASYNCH IO is supported FULLY\n",
              _POSIX_ASYNC_IO));
#endif /* _POSIX_ASYNC_IO == -1 */

#else  /* Not defined  _POSIX_ASYNC_IO */
  ACE_ERROR ((LM_DEBUG,
              "_POSIX_ASYNC_IO is not defined.\n"));
  ACE_DEBUG ((LM_DEBUG,
              "AIO might *not* be supported on all the paths\n"));
#endif /* _POSIX_ASYNC_IO */

  // System defined POSIX Values.
  ACE_DEBUG ((LM_DEBUG,
              "System claims to have  POSIX_ASYNCHRONOUS_IO\n"));

#if defined(_POSIX_AIO_LISTIO_MAX)
  ACE_DEBUG ((LM_DEBUG,
              "Number of operations in one listio: "
              "Minimum value is 2: "
              "_POSIX_AIO_LISTIO_MAX = %d\n",
              _POSIX_AIO_LISTIO_MAX));
#else
  ACE_DEBUG ((LM_DEBUG,
             "No value for _POSIX_AIO_LISTIO_MAX\n"));
#endif

#if defined(_POSIX_AIO_MAX)
  ACE_DEBUG ((LM_DEBUG,
              "Number of simultaneous asynchronous I/Os: "
              "Minimum is 1: "
              "_POSIX_AIO_MAX = %d\n",
              _POSIX_AIO_MAX));
#else
  ACE_DEBUG ((LM_DEBUG,
             "No value for _POSIX_AIO_MAX\n"));
#endif

  // @@ Debugging.
  ACE_DEBUG ((LM_DEBUG,
              "Before do_sysconf : Errno %d\n",
              errno));

  // Check and print the run time values.
  do_sysconf ();

  // @@ Debugging.
  ACE_DEBUG ((LM_DEBUG,
              "After do_sysconf: Errno : %d\n", errno));

  return 0;

#else /* Not _POSIX_ASYNCHRONOUS_IO */
  ACE_DEBUG ((LM_DEBUG,
              "No support._POSIX_ASYNCHRONOUS_IO itself is not defined\n"));
  return -1;
#endif /* _POSIX_ASYNCHRONOUS_IO */
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Aio_Platform_Test"));

  // Test the #defined and constants and runtime values.
  errno = 0;
  if (have_asynchio () == 0)
    ACE_DEBUG ((LM_DEBUG,
                "Basic test successful"
                "Check the run time values of the predefined constants\n"
                "ACE_HAS_AIO_CALLS can be defined for this platform\n"
                "Further tests at $ACE_ROOT/examples/Reactor/Proactor\n"));
  else
    ACE_ERROR ((LM_INFO,
                "AIO not supported on this platform\n"));

  ACE_END_TEST;

  return 0;
}
