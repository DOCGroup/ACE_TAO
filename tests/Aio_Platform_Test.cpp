// $Id$
// ============================================================================
//
// = FILENAME
//    aio_platform_test.cpp
//
// =  DESCRITPTION
//     Testing the platform for POSIX Asynchronous I/O. If
//     ACE_HAS_AIO_CALLS is defined, we also test the POSIX <aio_>
//     calls with real time signals on. We dont test the things with
//     aio_suspend.  
//
// = AUTHOR
//    Programming for the Real World. Bill O. GallMeister.
//    Modified by Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// =====================================================================

#include "test_config.h"
#include "ace/OS.h"

ACE_RCSID(tests, Aio_Platform_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

#include "ace/Message_Block.h"

#if defined (ACE_HAS_AIO_CALLS)
static ACE_HANDLE file_handle = ACE_INVALID_HANDLE;
static ACE_Message_Block mb1 (BUFSIZ + 1);
static ACE_Message_Block mb2 (BUFSIZ + 1);
static aiocb aiocb1;
static aiocb aiocb2;
static sigset_t completion_signal;

// Function prototypes.
static int have_asynchio (void);
static int query_aio_completions (void);
static int issue_aio_calls (void);
static int setup_signal_delivery (void);
static int test_aio_calls (void);
#endif /* ACE_HAS_AIO_CALLS */

#if defined (_POSIX_ASYNCHRONOUS_IO)
static int do_sysconf (void)
{
  // Call sysconf to find out runtime values.
  
  errno = 0;
#if defined (_SC_LISTIO_AIO_MAX)
  ACE_DEBUG ((LM_DEBUG,
              "Runtime value of LISTIO_AIO_MAX is %d, errno = %d\n",
              ACE_OS::sysconf (_SC_LISTIO_AIO_MAX),
              errno));
#elif defined (_SC_AIO_LISTIO_MAX)
  ACE_DEBUG ((LM_DEBUG,
              "Runtime value of AIO_LISTIO_MAX is %d, errno = %d\n",
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
              "Runtime value of AIO_MAX is %d, errno = %d\n",
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
              "Runtime value of RTSIG_MAX %d, Errno = %d\n",
              ACE_OS::sysconf (_SC_RTSIG_MAX),
              errno));
#else /* Not _SC_RTSIG_MAX */
  ACE_ERROR ((LM_ERROR,
              "_SC_RTSIG_MAX does not exist on this platform\n"));
#endif /* _SC_RTSIG_MAX */

#if defined (_SC_SIGQUEUE_MAX)
  errno = 0;
  ACE_DEBUG ((LM_DEBUG,
              "Runtime value of SIGQUEUE_MAX %d, Errno = %d\n",
              ACE_OS::sysconf (_SC_SIGQUEUE_MAX),
              errno));
#else /* Not _SC_SIGQUEUE_MAX */
  ACE_ERROR ((LM_ERROR,
              "_SC_SIGQUEUE_MAX does not exist on this platform\n"));
#endif /*  _SC_SIGQUEUE_MAX */
  return 0;
}
#endif /* _POSIX_ASYNCHRONOUS_IO */

#if defined (ACE_HAS_AIO_CALLS)
static int
test_aio_calls (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "test_aio_calls: Errno : %d\n",
              errno));
  // Set up the input file.
  // Open file (in SEQUENTIAL_SCAN mode)
  file_handle = ACE_OS::open ("Aio_Platform_Test.cpp",
                              O_RDONLY);

  if (file_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ACE_OS::open"),
                      -1);

  if (setup_signal_delivery () < 0)
    return -1;

  if (issue_aio_calls () < 0)
    return -1;

  if (query_aio_completions () < 0)
    return -1;

  return 0;
}

static int
setup_signal_delivery (void)
{
  // Make the sigset_t consisting of the completion signal.
  if (sigemptyset (&completion_signal) < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:%p:Couldnt init the RT completion signal set\n"),
                      -1);

  if (sigaddset (&completion_signal,
                 SIGRTMIN) < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:%p:Couldnt init the RT completion signal set\n"),
                      -1);

  // Mask them.
  if (sigprocmask (SIG_BLOCK, &completion_signal, 0) < 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:%p:Couldnt maks the RT completion signals\n"),
                      -1);

  // Setting up the handler(!) for these signals.
  struct sigaction reaction;
  sigemptyset (&reaction.sa_mask);   // Nothing else to mask.
  reaction.sa_flags = SA_SIGINFO;    // Realtime flag.
#if defined (SA_SIGACTION)
  // Lynx says, it is better to set this bit to be portable.
  reaction.sa_flags &= SA_SIGACTION;
#endif /* SA_SIGACTION */
  reaction.sa_sigaction = 0;         // No handler.
  int sigaction_return = sigaction (SIGRTMIN,
                                    &reaction,
                                    0);
  if (sigaction_return == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:%p:Proactor couldnt do sigaction for the RT SIGNAL"),
                      -1);
  return 0;
}

static int
issue_aio_calls (void)
{
  // Setup AIOCB.
  aiocb1.aio_fildes = file_handle;
  aiocb1.aio_offset = 0;
  aiocb1.aio_buf = mb1.wr_ptr ();
  aiocb1.aio_nbytes = BUFSIZ;
  aiocb1.aio_reqprio = 0;
  aiocb1.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
  aiocb1.aio_sigevent.sigev_signo = SIGRTMIN;
  aiocb1.aio_sigevent.sigev_value.sival_ptr = (void *) &aiocb1;

  // Fire off the aio write.
  if (aio_read (&aiocb1) == -1)
    // Queueing failed.
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Erro:%p:Asynch_Read_Stream: aio_read queueing failed\n"),
                      -1);

  // Setup AIOCB.
  aiocb2.aio_fildes = file_handle;
  aiocb2.aio_offset = BUFSIZ + 1;
  aiocb2.aio_buf = mb2.wr_ptr ();
  aiocb2.aio_nbytes = BUFSIZ;
  aiocb2.aio_reqprio = 0;
  aiocb2.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
  aiocb2.aio_sigevent.sigev_signo = SIGRTMIN;
  aiocb2.aio_sigevent.sigev_value.sival_ptr = (void *) &aiocb2;

  // Fire off the aio write.
  if (aio_read (&aiocb2) == -1)
    // Queueing failed.
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Erro:%p:Asynch_Read_Stream: aio_read queueing failed\n"),
                      -1);

  return 0;
}

static int
query_aio_completions (void)
{
  for (size_t number_of_compleions = 0;
       number_of_compleions < 2;
       number_of_compleions ++)
    {
      // Wait for <milli_seconds> amount of time.  @@ Assigning
      // <milli_seconds> to tv_sec.
      timespec timeout;
      timeout.tv_sec = ACE_INFINITE;
      timeout.tv_nsec = 0;

      // To get back the signal info.
      siginfo_t sig_info;

      // Await the RT completion signal.
      int sig_return = sigtimedwait (&completion_signal,
                                     &sig_info,
                                     &timeout);

      // Error case.
      // If failure is coz of timeout, then return *0* but set
      // errno appropriately. This is what the WinNT proactor
      // does.
      if (sig_return == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error:%p:Error waiting for RT completion signals\n"),
                          0);

      // RT completion signals returned.
      if (sig_return != SIGRTMIN)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unexpected signal (%d) has been received while waiting for RT Completion Signals\n",
                           sig_return),
                          -1);

      // @@ Debugging.
      ACE_DEBUG ((LM_DEBUG,
                  "Sig number found in the sig_info block : %d\n",
                  sig_info.si_signo));

      // Is the signo returned consistent?
      if (sig_info.si_signo != sig_return)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Inconsistent signal number (%d) in the signal info block\n",
                           sig_info.si_signo),
                          -1);

      // @@ Debugging.
      ACE_DEBUG ((LM_DEBUG,
                  "Signal code for this signal delivery : %d\n",
                  sig_info.si_code));

      // Is the signal code an aio completion one?
      if ((sig_info.si_code != SI_ASYNCIO) &&
          (sig_info.si_code != SI_QUEUE))
        ACE_ERROR_RETURN ((LM_DEBUG,
                           "Unexpected signal code (%d) returned on completion querying\n",
                           sig_info.si_code),
                          0);

      // Retrive the aiocb.
      aiocb* aiocb_ptr = (aiocb *) sig_info.si_value.sival_ptr;

      // Analyze error and return values. Return values are
      // actually <errno>'s associated with the <aio_> call
      // corresponding to aiocb_ptr.
      int error_code = aio_error (aiocb_ptr);
      if (error_code == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p:Invalid control block was sent to <aio_error> for compleion querying\n"),
                          -1);

      if (error_code != 0)
        // Error occurred in the <aio_>call. Return the errno
        // corresponding to that <aio_> call.
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p:An AIO call has failed\n"),
                          error_code);

      // No error occured in the AIO operation.
      int nbytes = aio_return (aiocb_ptr);
      if (nbytes == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p:Invalid control block was send to <aio_return>\n"),
                          -1);
      if (number_of_compleions == 0)
        // Print the buffer.
        ACE_DEBUG ((LM_DEBUG,
                    "\n Number of bytes transferred : %d\n The buffer : %s \n",
                    nbytes,
                    mb1.rd_ptr ()));
      else
        // Print the buffer.
        ACE_DEBUG ((LM_DEBUG,
                    "\n Number of bytes transferred : %d\n The buffer : %s \n",
                    nbytes,
                    mb2.rd_ptr ()));
    }

  return 0;
}
#endif /* ACE_HAS_AIO_CALLS */

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
              "AIO might *not* be supported on some paths\n"));
#endif /* _POSIX_ASYNC_IO */

  // System defined POSIX Values.
  ACE_DEBUG ((LM_DEBUG,
              "System claims to have  POSIX_ASYNCHRONOUS_IO\n"));
  ACE_DEBUG ((LM_DEBUG,
              "_POSIX_AIO_LISTIO_MAX = %d\n",
              _POSIX_AIO_LISTIO_MAX));
  ACE_DEBUG ((LM_DEBUG,
              "_POSIX_AIO_MAX = %d\n",
              _POSIX_AIO_MAX));

  // @@ Debugging.
  ACE_DEBUG ((LM_DEBUG,
              "Before do_sysconf : Errno %d\n",
              errno));
  
  // Check and print the run time values.
  do_sysconf ();

  // @@ Debugging.
  ACE_DEBUG ((LM_DEBUG,
              "After do_sysconf: Errno : %d\n", errno));

#if defined (ACE_HAS_AIO_CALLS)
  // Test the aio calls. Issue two <aio_read>s. Assing SIGRTMIN as the
  // notification signal. Mask these signal from delivery. Receive
  // this signal by doing <sigtimedwait>.
  test_aio_calls ();
#endif /* ACE_HAS_AIO_CALLS */

  return 0;

#else /* Not _POSIX_ASYNCHRONOUS_IO */
  ACE_DEBUG ((LM_DEBUG,
              "No support._POSIX_ASYNCHRONOUS_IO itself is not defined\n"));
  return -1;
#endif /* _POSIX_ASYNCHRONOUS_IO */
}

int
main (int, char *[])
{
  ACE_START_TEST ("Aio_Platform_Test");

  have_asynchio ();

  ACE_END_TEST;
  return 0;
}
