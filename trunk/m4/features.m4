dnl -------------------------------------------------------------------------
dnl       $Id$
dnl 
dnl       features.m4
dnl
dnl       ACE M4 include file which contains ACE specific M4 macros
dnl       that determine availablility of certain OS features for ACE.
dnl 
dnl -------------------------------------------------------------------------

dnl  Copyright (C) 1998, 1999  Ossama Othman
dnl
dnl  All Rights Reserved
dnl
dnl This library is free software; you can redistribute it and/or
dnl modify it under the current ACE distribution terms.
dnl 
dnl This library is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

dnl Asynchronous IO check
dnl Use this macro to determine if asynchronous IO is working on a
dnl given platform.
dnl Usage: ACE_CHECK_ASYNCH_IO
AC_DEFUN(ACE_CHECK_ASYNCH_IO, dnl
[
 AC_REQUIRE([AC_PROG_CXX])
 AC_REQUIRE([AC_PROG_CXXCPP])
 AC_REQUIRE([AC_LANG_CPLUSPLUS])

 dnl In case a library with the asynchronous libraries is found but
 dnl the asynchronous IO support is not functional then save a copy
 dnl of the list of libraries before the asynch IO function library
 dnl is added to the list so that we can revert the list to its
 dnl pre-asynch-IO check state.
 ace_save_LIBS="$LIBS"

 dnl Asynchronous IO library check
 dnl Some platforms, such as Solaris puts aio_read in -lposix4, for example.
 dnl In some cases, the thread library must be linked to in addition to the
 dnl real-time support library.  As such, make sure these checks are done
 dnl after the thread library checks.
 ace_has_aio_funcs=yes
  AC_CHECK_FUNC(aio_read, , dnl
   AC_CHECK_LIB(aio, aio_read, ,
     AC_CHECK_LIB(posix4, aio_read, , 
       AC_CHECK_LIB(rt, aio_read, , ace_has_aio_funcs=no))))

if test "$ace_has_aio_funcs" = yes; then
  ACE_CACHE_CHECK(for working asynchronous IO,
    ace_cv_feature_aio_calls,
    [
     AC_TRY_RUN(
       [
#ifndef ACE_LACKS_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include <aio.h>

class Test_Aio
{
public:
  Test_Aio (void);
  // Default constructor.

  int init (void);
  // Initting the output file and the buffer.

  int do_aio (void);
  // Doing the testing stuff.

  ~Test_Aio (void);
  // Destructor.
private:
  int out_fd_;
  // Output file descriptor.

  struct aiocb *aiocb_write_;
  // For writing to the file.

  struct aiocb *aiocb_read_;
  // Reading stuff from the file.

  char *buffer_write_;
  // The buffer to be written to the out_fd.

  char *buffer_read_;
  // The buffer to be read back from the file.
};

Test_Aio::Test_Aio (void)
  : aiocb_write_ (new struct aiocb),
    aiocb_read_ (new struct aiocb),
    buffer_write_ (0),
    buffer_read_ (0)
{
}

Test_Aio::~Test_Aio (void)
{
  delete aiocb_write_;
  delete aiocb_read_;
  delete buffer_write_;
  delete buffer_read_;
}

// Init the output file and init the buffer.
int
Test_Aio::init (void)
{
  // Open the output file.
  this->out_fd_ = open ("conftest.log", O_RDWR | O_CREAT | O_TRUNC, 0666);
  if (this->out_fd_ == 0)
    {
      return -1;
    }

  // Init the buffers.
  this->buffer_write_ = strdup ("Welcome to the world of AIO... AIO Rules !!!");
  this->buffer_read_ = new char [strlen (this->buffer_write_)];
}

// Set the necessary things for the AIO stuff.
// Write the buffer asynchly.hmm Disable signals.
// Go on aio_suspend. Wait for completion.
// Print out the result.
int
Test_Aio::do_aio (void)
{
  // = Write to the file.

  // Setup AIOCB.
  this->aiocb_write_->aio_fildes = this->out_fd_;
  this->aiocb_write_->aio_offset = 0;
  this->aiocb_write_->aio_buf = this->buffer_write_;
  this->aiocb_write_->aio_nbytes = strlen (this->buffer_write_);
  this->aiocb_write_->aio_reqprio = 0;
  this->aiocb_write_->aio_sigevent.sigev_notify = SIGEV_NONE;
  //this->this->aiocb_.aio_sigevent.sigev_signo = SIGRTMAX;
  this->aiocb_write_->aio_sigevent.sigev_value.sival_ptr =
    (void *) this->aiocb_write_;

  // Fire off the aio write.
  if (aio_write (this->aiocb_write_) != 0)
    {
      perror ("aio_write");
      return -1;
    }

  // = Read from that file.

  // Setup AIOCB.
  this->aiocb_read_->aio_fildes = this->out_fd_;
  this->aiocb_read_->aio_offset = 0;
  this->aiocb_read_->aio_buf = this->buffer_read_;
  this->aiocb_read_->aio_nbytes = strlen (this->buffer_write_);
  this->aiocb_read_->aio_reqprio = 0;
  this->aiocb_read_->aio_sigevent.sigev_notify = SIGEV_NONE;
  //this->this->aiocb_.aio_sigevent.sigev_signo = SIGRTMAX;
  this->aiocb_read_->aio_sigevent.sigev_value.sival_ptr =
    (void *) this->aiocb_read_;

  // Fire off the aio write. If it doesnt get queued, carry on to get
  // the completion for the first one.
  if (aio_read (this->aiocb_read_) < 0)
    perror ("aio_read");
  
  // Wait for the completion on aio_suspend.
  struct aiocb *list_aiocb[2];
  list_aiocb [0] = this->aiocb_write_;
  list_aiocb [1] = this->aiocb_read_;

  // Do suspend till all the aiocbs in the list are done.
  int done = 0;
  int return_val = 0;
  while (!done)
    {
      return_val = aio_suspend (list_aiocb,
                                2,
                                0);

      // Analyze return and error values.
      if (aio_error (list_aiocb [0]) != EINPROGRESS)
        {
          if (aio_return (list_aiocb [0]) == -1)
            {
              perror ("aio_return");
              return -1;
            }
          else
            {
              // Successful. Store the pointer somewhere and make the
              // entry NULL in the list.
              this->aiocb_write_ = list_aiocb [0];
              list_aiocb [0] = 0;
            }
        }

      if (aio_error (list_aiocb [1]) != EINPROGRESS)
        {
          if (aio_return (list_aiocb [1]) == -1)
            {
              perror ("aio_return");
              return -1;
            }
          else
            {
              // Successful. Store the pointer somewhere and make the
              // entry NULL in the list.
              this->aiocb_read_ = list_aiocb [1];
              list_aiocb [1] = 0;
            }
        }

      // Is it done?
      if ((list_aiocb [0] == 0) && (list_aiocb [1] == 0))
        done = 1;
    }
  
  return 0;
}

int
main (int argc, char *argv[])
{
  Test_Aio test_aio;

  if (test_aio.init () != 0)
    {
      return -1;
    }
  
  if (test_aio.do_aio () != 0)
    {
      return -1;
    }

  return 0;
}
       ],
       [
        dnl Now try another test
        AC_TRY_RUN(
          [
#ifndef ACE_LACKS_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#ifndef ACE_LACKS_SYS_TYPES_H
# include <sys/types.h>
#endif
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

#include <limits.h>

#include <aio.h>

int file_handle = -1;
char mb1 [BUFSIZ + 1];
char mb2 [BUFSIZ + 1];
aiocb aiocb1, aiocb2;
sigset_t completion_signal;

/* Function prototypes. */
int setup_signal_delivery (void);
int issue_aio_calls (void);
int query_aio_completions (void);
int test_aio_calls (void);

int 
setup_signal_delivery (void)
{
  /* Make the sigset_t consisting of the completion signal. */
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
  
  /* Mask them. */
  if (sigprocmask (SIG_BLOCK, &completion_signal, 0) == -1)
    {
      perror ("Error:Couldnt maks the RT completion signals\n");
      return -1;
    }
  
  /* Setting up the handler(!) for these signals. */
  struct sigaction reaction;
  sigemptyset (&reaction.sa_mask);   /* Nothing else to mask. */
  reaction.sa_flags = SA_SIGINFO;    /* Realtime flag. */
#if defined (SA_SIGACTION)
  /* Lynx says, it is better to set this bit to be portable. */
  reaction.sa_flags &= SA_SIGACTION;
#endif /* SA_SIGACTION */      
  reaction.sa_sigaction = 0;         /* No handler. */
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
issue_aio_calls (void)
{
  /* Setup AIOCB. */
  aiocb1.aio_fildes = file_handle;
  aiocb1.aio_offset = 0;
  aiocb1.aio_buf = mb1;
  aiocb1.aio_nbytes = BUFSIZ;
  aiocb1.aio_reqprio = 0;
  aiocb1.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
  aiocb1.aio_sigevent.sigev_signo = SIGRTMIN;
  aiocb1.aio_sigevent.sigev_value.sival_ptr = (void *) &aiocb1; 
  
  /* Fire off the aio write. */
  if (aio_read (&aiocb1) == -1)
    {
      /* Queueing failed. */
      perror ("Error:Asynch_Read_Stream: aio_read queueing failed\n");
      return -1;
    }
  
  /* Setup AIOCB. */
  aiocb2.aio_fildes = file_handle;
  aiocb2.aio_offset = BUFSIZ + 1;
  aiocb2.aio_buf = mb2;
  aiocb2.aio_nbytes = BUFSIZ;
  aiocb2.aio_reqprio = 0;
  aiocb2.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
  aiocb2.aio_sigevent.sigev_signo = SIGRTMIN;
  aiocb2.aio_sigevent.sigev_value.sival_ptr = (void *) &aiocb2; 
  
  /* Fire off the aio write. */
  if (aio_read (&aiocb2) == -1)
    {
      /* Queueing failed. */
      perror ("Error:Asynch_Read_Stream: aio_read queueing failed\n");
      return -1;
    }
  return 0;
}

int
query_aio_completions (void)
{
  int result = 0;
  size_t number_of_compleions = 0;
  for (number_of_compleions = 0;
       number_of_compleions < 2;
       number_of_compleions ++)
    {
      /* Wait for <milli_seconds> amount of time. */
      /* @@ Assigning <milli_seconds> to tv_sec. */
      timespec timeout;
      timeout.tv_sec = INT_MAX;
      timeout.tv_nsec = 0;
  
      /* To get back the signal info. */
      siginfo_t sig_info;
      
      /* Await the RT completion signal. */
      int sig_return = sigtimedwait (&completion_signal,
                                     &sig_info,
                                     &timeout);
      
      /* Error case. */
      /* If failure is coz of timeout, then return *0* but set */
      /* errno appropriately. This is what the WinNT proactor */
      /* does. */
      if (sig_return == -1)
        {
          perror ("Error:Error waiting for RT completion signals\n");
          return -1;
        }
      
      /* RT completion signals returned. */
      if (sig_return != SIGRTMIN)
        {
          printf ("Unexpected signal (%d) has been received while waiting for RT Completion Signals\n",
                  sig_return);
          return -1;
        }
      
      /* @@ Debugging. */
      printf ("Sig number found in the sig_info block : %d\n",
              sig_info.si_signo);
  
      /* Is the signo returned consistent? */
      if (sig_info.si_signo != sig_return)
        {
          printf ("Inconsistent signal number (%d) in the signal info block\n",
                  sig_info.si_signo);
          return -1;
        }
  
      /* @@ Debugging. */
      printf ("Signal code for this signal delivery : %d\n",
              sig_info.si_code);
  
      /* Is the signal code an aio completion one? */
      if ((sig_info.si_code != SI_ASYNCIO) &&
          (sig_info.si_code != SI_QUEUE))
        {
          printf ("Unexpected signal code (%d) returned on completion querying\n",
                  sig_info.si_code);
          return -1;
        }
  
      /* Retrive the aiocb. */
      aiocb* aiocb_ptr = (aiocb *) sig_info.si_value.sival_ptr;
      
      /* Analyze error and return values. Return values are */
      /* actually <errno>'s associated with the <aio_> call */
      /* corresponding to aiocb_ptr. */
      int error_code = aio_error (aiocb_ptr);
      if (error_code == -1)
        {
          perror ("Error:Invalid control block was sent to <aio_error> for compleion querying\n");
          return -1;
        }
  
      if (error_code != 0)
        {
          /* Error occurred in the <aio_>call. Return the errno */
          /* corresponding to that <aio_> call. */
          printf ("Error:An AIO call has failed:Error code = %d\n",
                  error_code);
          return -1;
        }

      /* No error occured in the AIO operation. */
      int nbytes = aio_return (aiocb_ptr);
      if (nbytes == -1)
        {
          perror ("Error:Invalid control block was send to <aio_return>\n");
          return -1;
        }
      
      if (number_of_compleions == 0)
        /* Print the buffer. */
        printf ("Number of bytes transferred : %d\n The buffer : %s \n",
                nbytes,
                mb1);
      else
        /* Print the buffer. */
        printf ("Number of bytes transferred : %d\n The buffer : %s \n",
                nbytes,
                mb2);
    }
  return 0;
}

int
test_aio_calls (void) 
{
  /* Set up the input file. */
  /* Open file (in SEQUENTIAL_SCAN mode) */
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

int
main (int, char *[])
{
  if (test_aio_calls () != 0)
    return -1;
  else
  return 0;
}
         ],
         [
          ace_cv_feature_aio_calls=yes
         ],
         [
          ace_cv_feature_aio_calls=no
         ],
         [
          dnl Don't bother doing anything for cross-compiling here
          dnl since the outer run-time test will prevent this
          dnl inner run-time test from ever running when cross-compiling.
          dnl We just put something in here to prevent autoconf
          dnl from complaining.
          ace_just_a_place_holder=ignoreme
         ])
       ],
       [
        ace_cv_feature_aio_calls=no
       ],
       [
        dnl Asynchronous IO test for cross-compiled platforms
        dnl This test is weaker than the above run-time tests but it will
        dnl have to do.
        AC_TRY_COMPILE(
          [
#include <aio.h>
          ],
          [
           aiocb* aiocb_ptr (void);
          ],
          [
           ace_cv_feature_aio_calls=yes
          ],
          [
           ace_cv_feature_aio_calls=no
          ])
       ])
    ], AC_DEFINE(ACE_HAS_AIO_CALLS), LIBS="$ace_save_LIBS")
fi dnl test "$ace_has_aio_funcs" = yes
])
