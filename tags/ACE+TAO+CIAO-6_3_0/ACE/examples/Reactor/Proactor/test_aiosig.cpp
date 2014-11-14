//=============================================================================
/**
 *  @file     test_aiosig.cpp
 *
 *  $Id$
 *
 *  Check out test_aiosig_ace.cpp, the ACE'ified version of this
 *  program. This program may not be uptodate.
 *  CC -g -o test_aiosig -lrt test_aiosig.cpp
 *  ./test_aiosig
 *
 *  @author  Programming for the Real World. Bill O. GallMeister.  Modified by Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */
//=============================================================================


//FUZZ: disable check_for_lack_ACE_OS
//FUZZ: disable check_for_improper_main_declaration

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include <limits.h>
#include <pthread.h>

#include <aio.h>

int file_handle = -1;
char mb1 [BUFSIZ + 1];
char mb2 [BUFSIZ + 1];
aiocb aiocb1, aiocb2;
sigset_t completion_signal;

// Function prototypes.
int setup_signal_delivery (void);
int issue_aio_calls (void);
int query_aio_completions (void);
int test_aio_calls (void);
int setup_signal_handler (void);
int setup_signal_handler (int signal_number);

int
setup_signal_delivery (void)
{
  // Make the sigset_t consisting of the completion signal.
  if (sigemptyset (&completion_signal) == -1)
    {
      perror ("Error:Couldnt init the RT completion signal set\n");
      return -1;
    }

  if (sigaddset (&completion_signal, SIGRTMIN) == -1)
    {
      perror ("Error:Couldnt init the RT completion signal set\n");
      return -1;
    }

  // Mask them.
  if (pthread_sigmask (SIG_BLOCK, &completion_signal, 0) == -1)
    {
      perror ("Error:Couldnt maks the RT completion signals\n");
      return -1;
    }

  return setup_signal_handler (SIGRTMIN);
}

int
issue_aio_calls (void)
{
  // Setup AIOCB.
  aiocb1.aio_fildes = file_handle;
  aiocb1.aio_offset = 0;
  aiocb1.aio_buf = mb1;
  aiocb1.aio_nbytes = BUFSIZ;
  aiocb1.aio_reqprio = 0;
  aiocb1.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
  aiocb1.aio_sigevent.sigev_signo = SIGRTMIN;
  aiocb1.aio_sigevent.sigev_value.sival_ptr = (void *) &aiocb1;

  // Fire off the aio write.
  if (aio_read (&aiocb1) == -1)
    {
      // Queueing failed.
      perror ("Error:Asynch_Read_Stream: aio_read queueing failed\n");
      return -1;
    }

  // Setup AIOCB.
  aiocb2.aio_fildes = file_handle;
  aiocb2.aio_offset = BUFSIZ + 1;
  aiocb2.aio_buf = mb2;
  aiocb2.aio_nbytes = BUFSIZ;
  aiocb2.aio_reqprio = 0;
  aiocb2.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
  aiocb2.aio_sigevent.sigev_signo = SIGRTMIN;
  aiocb2.aio_sigevent.sigev_value.sival_ptr = (void *) &aiocb2;

  // Fire off the aio write.
  if (aio_read (&aiocb2) == -1)
    {
      // Queueing failed.
      perror ("Error:Asynch_Read_Stream: aio_read queueing failed\n");
      return -1;
    }
  return 0;
}

int
query_aio_completions (void)
{
  size_t number_of_compleions = 0;
  for (number_of_compleions = 0;
       number_of_compleions < 2;
       number_of_compleions ++)
    {
      // Wait for <milli_seconds> amount of time.
      // @@ Assigning <milli_seconds> to tv_sec.
      timespec timeout;
      timeout.tv_sec = INT_MAX;
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
        {
          perror ("Error:Error waiting for RT completion signals\n");
          return -1;
        }

      // RT completion signals returned.
      if (sig_return != SIGRTMIN)
        {
          printf ("Unexpected signal (%d) has been received while waiting for RT Completion Signals\n",
                  sig_return);
          return -1;
        }

      // @@ Debugging.
      printf ("Sig number found in the sig_info block : %d\n",
              sig_info.si_signo);

      // Is the signo returned consistent?
      if (sig_info.si_signo != sig_return)
        {
          printf ("Inconsistent signal number (%d) in the signal info block\n",
                  sig_info.si_signo);
          return -1;
        }

      // @@ Debugging.
      printf ("Signal code for this signal delivery : %d\n",
              sig_info.si_code);

      // Is the signal code an aio completion one?
      if ((sig_info.si_code != SI_ASYNCIO) &&
          (sig_info.si_code != SI_QUEUE))
        {
          printf ("Unexpected signal code (%d) returned on completion querying\n",
                  sig_info.si_code);
          return -1;
        }

      // Retrive the aiocb.
      aiocb* aiocb_ptr = (aiocb *) sig_info.si_value.sival_ptr;

      // Analyze error and return values. Return values are
      // actually <errno>'s associated with the <aio_> call
      // corresponding to aiocb_ptr.
      int error_code = aio_error (aiocb_ptr);
      if (error_code == -1)
        {
          perror ("Error:Invalid control block was sent to <aio_error> for compleion querying\n");
          return -1;
        }

      if (error_code != 0)
        {
          // Error occurred in the <aio_>call. Return the errno
          // corresponding to that <aio_> call.
          printf ("Error:An AIO call has failed:Error code = %d\n",
                  error_code);
          return -1;
        }

      // No error occured in the AIO operation.
      int nbytes = aio_return (aiocb_ptr);
      if (nbytes == -1)
        {
          perror ("Error:Invalid control block was send to <aio_return>\n");
          return -1;
        }

      if (number_of_compleions == 0)
        // Print the buffer.
        printf ("Number of bytes transferred : %d\n The buffer : %s\n",
                nbytes,
                mb1);
      else
        // Print the buffer.
        printf ("Number of bytes transferred : %d\n The buffer : %s\n",
                nbytes,
                mb2);
    }
  return 0;
}

int
test_aio_calls (void)
{
  // Set up the input file.
  // Open file (in SEQUENTIAL_SCAN mode)
  file_handle = open ("test_aiosig.cpp", O_RDONLY);

  if (file_handle == -1)
    {
      perror ("Error:Opening the inputfile");
      return -1;
    }

  if (setup_signal_delivery () < 0)
    return -1;

  if (issue_aio_calls () < 0)
    return -1;

  if (query_aio_completions () < 0)
    return -1;

  return 0;
}

void
null_handler (int         /* signal_number */,
              siginfo_t * /* info */,
              void *      /* context */)
{
}

int
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
  reaction.sa_sigaction = null_handler;     // Null handler.
  int sigaction_return = sigaction (SIGRTMIN,
                                    &reaction,
                                    0);
  if (sigaction_return == -1)
    {
      perror ("Error:Proactor couldnt do sigaction for the RT SIGNAL");
      return -1;
    }

  return 0;
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
