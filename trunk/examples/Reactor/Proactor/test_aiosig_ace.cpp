// $Id$
// ============================================================================
//
// = FILENAME
//     test_aiosig_sig.cpp
//
// = DESCRITPTION
//     This program helps you to test the <aio_*> calls on a
//     platform. 
//     Before running this test, make sure the platform can
//     support POSIX <aio_> calls, using ACE_ROOT/tests/Aio_Plaform_Test.cpp
// 
//     This  program tests the Signal based completion approach which 
//     uses <sigtimedwait> for completion querying. 
//     If this test is successful, ACE_POSIX_SIG_PROACTOR
//     can be used on this platform.
// 
//     This program is a ACE version of the 
//     $ACE_ROOT/examples/Reactor/Proactor/test_aiosig.cpp, with
//     ACE_DEBUGs and Message_Blocks.
//
//     This test does the following:
//     Issue two <aio_read>s.
//     Assign SIGRTMIN as the notification signal.
//     Mask these signals from delivery.
//     Receive this signal by doing <sigtimedwait>.
//     Wait for two completions (two signals)
// 
// = COMPILATION
//     make 
//
// = RUN
//     ./test_aiosig_ace
//
// = AUTHOR
//     Programming for the Real World. Bill O. GallMeister.
//     Modified by Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// =====================================================================

#include "ace/Message_Block.h"

static ACE_HANDLE file_handle = ACE_INVALID_HANDLE;
static ACE_Message_Block mb1 (BUFSIZ + 1);
static ACE_Message_Block mb2 (BUFSIZ + 1);
static aiocb aiocb1;
static aiocb aiocb2;
static sigset_t completion_signal;

// Function prototypes.
static int setup_signal_delivery (void);
static int issue_aio_calls (void);
static int query_aio_completions (void);
static int test_aio_calls (void);
static void null_handler (int signal_number, siginfo_t *info, void *context);
static int setup_signal_handler (int signal_number);

static int
setup_signal_delivery (void)
{
  // = Mask all the signals.
  
  sigset_t full_set;
  
  // Get full set.
  if (sigfillset (&full_set) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:(%P | %t):%p\n",
                       "sigfillset failed"),
                      -1);
  
  // Mask them.
  if (pthread_sigmask (SIG_SETMASK, &full_set, 0) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:(%P | %t):%p\n",
                       "pthread_sigmask failed"),
                      -1);
  
  // = Make a mask with SIGRTMIN only. We use only that signal to
  //   issue <aio_>'s.
  
  if (sigemptyset (&completion_signal) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:%p:Couldnt init the RT completion signal set\n"),
                      -1);

  if (sigaddset (&completion_signal,
                 SIGRTMIN) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:%p:Couldnt init the RT completion signal set\n"),
                      -1);

  // Set up signal handler for this signal.
  return setup_signal_handler (SIGRTMIN);
}

static int
setup_signal_handler (int signal_number)
{
  // Setting up the handler(!) for these signals.
  struct sigaction reaction;
  sigemptyset (&reaction.sa_mask);   // Nothing else to mask.
  reaction.sa_flags = SA_SIGINFO;    // Realtime flag.
#if defined (SA_SIGACTION)
  // Lynx says, it is better to set this bit to be portable.
  reaction.sa_flags &= SA_SIGACTION;
#endif /* SA_SIGACTION */
  reaction.sa_sigaction = null_handler; // Null handler.
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
                          -1);

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
                          -1);

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

static int
test_aio_calls (void)
{
  // Set up the input file.
  // Open file (in SEQUENTIAL_SCAN mode)
  file_handle = ACE_OS::open ("test_aiosig_ace.cpp",
                              O_RDONLY);

  if (file_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ACE_OS::open"),
                      -1);

  if (setup_signal_delivery () == -1)
    return -1;

  if (issue_aio_calls () == -1)
    return -1;

  if (query_aio_completions () == -1)
    return -1;

  return 0;
}

static void
null_handler (int signal_number,
              siginfo_t */* info */,
              void * /* context */)
{
  ACE_ERROR ((LM_ERROR,
              "Error:%s:Signal number %d\n"
              "Mask all the RT signals for this thread",
              "ACE_POSIX_SIG_Proactor::null_handler called",
              signal_number));
}

int
main (int, char *[])
{
  if (test_aio_calls () == 0)
    printf ("RT SIG test successful:\n"
            "ACE_POSIX_SIG_PROACTOR should work in this platform\n");
  else
    printf ("RT SIG test failed:\n"
            "ACE_POSIX_SIG_PROACTOR may not work in this platform\n");
  return 0;
}
