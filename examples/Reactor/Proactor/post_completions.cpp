// $Id$
// ============================================================================
//
// = FILENAME
//     post_completions.cpp
//
// = DESCRITPTION
//     This program demonstrates how to post fake completions to The
//     Proactor. It also shows the how to specify the particular
//     real-time signals to post completions. The Real-time signal
//     based completion strategy is implemented with
//     ACE_POSIX_SIG_PROACTOR.
//     (So, it can be used only if ACE_HAS_AIO_CALLS is defined and
//      ACE_POSIX_AIOCB_PROACTOR is not defined)
//     Since it is faking results, you have to pay by knowing and
//     using platform-specific implementation objects for Asynchronous
//     Result classes.
//     This example shows using an arbitrary result class for faking
//     completions. You can also use the predefined Result classes for
//     faking. The factory methods in the Proactor class create the
//     Result objects.
//
// = COMPILATION
//     make
//
// = RUN
//     ./post_completions
//
// = AUTHOR
//     Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// =====================================================================

#include "ace/Proactor.h"
#include "ace/Synch.h"
#include "ace/Task.h"
#include "ace/WIN32_Proactor.h"
#include "ace/POSIX_Proactor.h"
#include "ace/Atomic_Op.h"
#include "ace/Thread_Mutex.h"

// Keep track of how many completions are still expected.
static ACE_Atomic_Op <ACE_SYNCH_MUTEX, size_t> Completions_To_Go;


#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) || \
     defined (ACE_HAS_AIO_CALLS)
// This only works on Win32 platforms and on Unix platforms supporting
// POSIX aio calls.

#if defined (ACE_HAS_AIO_CALLS)
#define RESULT_CLASS ACE_POSIX_Asynch_Result
#elif defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
#define RESULT_CLASS ACE_WIN32_Asynch_Result
#endif /* ACE_HAS_AIO_CALLS */

class My_Result : public RESULT_CLASS
{
  // = TITLE
  //
  //     Result Object that we will post to the Proactor.
  //
  // = DESCRIPTION
  //

public:
  My_Result (ACE_Handler &handler,
             const void *act,
             int signal_number,
             size_t sequence_number)
    : RESULT_CLASS (handler,
                    act,
                    ACE_INVALID_HANDLE,
                    0, // Offset
                    0, // OffsetHigh
                    0, // Priority
                    signal_number),
      sequence_number_ (sequence_number)
    {}
  // Constructor.

  virtual ~My_Result (void)
    {}
  // Destructor.

  void complete (size_t,
                 int success,
                 const void *completion_key,
                 u_long error)
    // This is the method that will be called by the Proactor for
    // dispatching the completion. This method generally calls one of
    // the call back hood methods defined in the ACE_Handler
    // class. But, we will just handle the completions here.
    {
      this->success_ = success;
      this->completion_key_ = completion_key;
      this->error_ = error;

      size_t to_go = --Completions_To_Go;

      // Print the completion details.
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) Completion sequence number %d, success : %d, error : %d, signal_number : %d, %u more to go\n",
                  this->sequence_number_,
                  this->success_,
                  this->error_,
                  this->signal_number (),
                  to_go));

      // Sleep for a while.
      ACE_OS::sleep (4);
    }

private:
  size_t sequence_number_;
  // Sequence number for the result object.
};

class My_Handler : public  ACE_Handler
{
  // = TITLE
  //
  //     Handler class for faked completions.
  //
  // = DESCRIPTION
  //

public:
  My_Handler (void) {}
  // Constructor.

  virtual ~My_Handler (void) {}
  // Destructor.
};

class My_Task: public ACE_Task <ACE_NULL_SYNCH>
{
  // = TITLE
  //
  //     Contains thread functions which execute event loops. Each
  //     thread waits for a different signal.
  //
public:
  My_Task (void) {}
  // Constructor.

  virtual ~My_Task (void) {}
  // Destructor.

  int open (void *proactor)
    {
      // Store the proactor.
      this->proactor_ = (ACE_Proactor *) proactor;

      // Activate the Task.
      this->activate (THR_NEW_LWP, 5);
      return 0;
    }

  int svc (void)
    {
      // Handle events for 13 seconds.
      ACE_Time_Value run_time (13);

      ACE_DEBUG ((LM_DEBUG, "(%t):Starting svc routine\n"));

      if (this->proactor_->handle_events (run_time) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "(%t):%p.\n", "Worker::svc"), -1);

      ACE_DEBUG ((LM_DEBUG, "(%t) work complete\n"));

      return 0;
    }

private:
  ACE_Proactor *proactor_;
  // Proactor for this task.
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  ACE_DEBUG ((LM_DEBUG,
              "(%P | %t):Test starts \n"));

  // = Get two POSIX_SIG_Proactors, one with SIGRTMIN and one with
  //   SIGRTMAX.

  ACE_Proactor  proactor1;
  // Proactor1. SIGRTMIN Proactor. (default).

  // = Proactor2. SIGRTMAX Proactor.
#if defined (ACE_HAS_AIO_CALLS) && !defined (ACE_POSIX_AIOCB_PROACTOR)

  ACE_DEBUG ((LM_DEBUG, "Using ACE_POSIX_SIG_Proactor\n"));

  sigset_t signal_set;
  // Signal set that we want to mask.

  // Clear the signal set.
  if (sigemptyset (&signal_set) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:%p\n",
                       "sigemptyset failed"),
                      1);

  // Add the SIGRTMAX to the signal set.
  if (sigaddset (&signal_set, ACE_SIGRTMAX) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:%p\n",
                       "sigaddset failed"),
                      1);

  // Make the POSIX Proactor.
  ACE_POSIX_SIG_Proactor posix_proactor (signal_set);
  // Get the Proactor interface out of it.
  ACE_Proactor proactor2 (&posix_proactor);
#else /* ACE_HAS_AIO_CALLS && !ACE_POSIX_AIOCB_PROACTOR */
  ACE_Proactor proactor2;
#endif /* ACE_HAS_AIO_CALLS && !ACE_POSIX_AIOCB_PROACTOR */

  // = Create Tasks. One pool of threads to handle completions on
  //   SIGRTMIN and the other one to handle completions on SIGRTMAX.
  My_Task task1, task2;
  task1.open (&proactor1);
  task2.open (&proactor2);

  // Handler for completions.
  My_Handler handler;

  // = Create a few MyResult objects and post them to Proactor.
  const size_t NrCompletions (10);
  My_Result *result_objects [NrCompletions];
  int signal_number = ACE_SIGRTMAX;
  size_t ri;

  Completions_To_Go = NrCompletions;

  // Creation.
  for (ri = 0; ri < NrCompletions; ri++)
    {
      // Use RTMIN and RTMAX proactor alternatively, to post
      // completions.
      if (ri % 2)
        signal_number = ACE_SIGRTMIN;
      else
        signal_number = ACE_SIGRTMAX;
      // Create the result.
      ACE_NEW_RETURN (result_objects [ri],
                      My_Result (handler,
                                 0,
                                 signal_number,
                                 ri),
                      1);
    }
  ACE_OS::sleep(5);
  // Post all the result objects.
  ACE_Proactor *proactor;
  for (ri = 0; ri < NrCompletions; ri++)
    {
      // Use RTMIN and RTMAX Proactor alternatively, to post
      // completions.
      if (ri % 2)
        proactor = &proactor1;
      else
        proactor = &proactor2;
      if (result_objects [ri]->post_completion (proactor->implementation ())
          == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Test failed\n"),
                          1);
    }

  ACE_Thread_Manager::instance ()->wait ();

  int status = 0;
  size_t to_go = Completions_To_Go.value ();
  if (size_t (0) != to_go)
    {
      ACE_ERROR ((LM_ERROR,
                  "Fail! Expected all completions to finish but %u to go\n",
                  to_go));
      status = 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%P | %t):Test ends\n"));
  return status;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Task <ACE_NULL_SYNCH>;
template class ACE_Atomic_Op <ACE_Thread_Mutex, size_t>;
template class ACE_Atomic_Op_Ex <ACE_Thread_Mutex, size_t>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Task <ACE_NULL_SYNCH>
#pragma instantiate ACE_Atomic_Op <ACE_Thread_Mutex, size_t>
#pragma instantiate ACE_Atomic_Op_Ex <ACE_Thread_Mutex, size_t>
#endif /* ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA */

#else /* ACE_WIN32 && !ACE_HAS_WINCE || ACE_HAS_AIO_CALLS && !ACE_POSIX_AIOCB_PROACTOR*/

int
main (int, char *[])
{
  ACE_DEBUG ((LM_DEBUG,
              "This example cannot work with AIOCB_Proactor.\n"));
  return 1;
}

#endif /* ACE_WIN32 && !ACE_HAS_WINCE || ACE_HAS_AIO_CALLS && !ACE_POSIX_AIOCB_PROACTOR*/

