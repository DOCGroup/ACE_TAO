// $Id$

// This test program illustrates that the Win32
// <WaitForMultipleObjects> function can be called in multiple
// threads, all of which wait on the same set of HANDLEs.  Note that
// the dispatching of the threads should be relatively "fair" (i.e.,
// everyone gets a chance to process the various HANDLEs as they
// become active).  Thanks to Ari Erev <Ari_Erev@comverse.com> for
// suggesting this and providing the initial code.

#include "ace/Task.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_main.h"



#if defined (ACE_WIN32)

// Number of threads.
static const int THREAD_COUNT = 5;

// Number of iterations.
static const int MAX_ITERATIONS = 100;

class WFMO_Test : public ACE_Task <ACE_NULL_SYNCH>
{
public:
  //FUZZ: disable check_for_lack_ACE_OS
  virtual int open (void *);
  //FUZZ: enable check_for_lack_ACE_OS

  virtual int svc (void);

  // Use two handles here..
  ACE_sema_t sema_handles_[2];
  int semaphore_count_;
};

static WFMO_Test wfmo_test;

int
WFMO_Test::open (void *arg)
{
  int thread_count = int (arg);
  int result = this->activate (0, thread_count);

  ACE_ASSERT (result != -1);
  return 0;
}

int
WFMO_Test::svc (void)
{
  while(1)
    {
      int result = ::WaitForMultipleObjects (2, this->sema_handles_,
                                             FALSE, INFINITE);
      if (result == WAIT_OBJECT_0)
        {
          // Signal the other semaphore just to see if we can get
          // another thread to wakeup.
          result = ACE_OS::sema_post (&sema_handles_[1]);
          ACE_ASSERT (result != -1);
        }
      else if (result == WAIT_OBJECT_0 + 1)
        ;
      else
        {
          ACE_ERROR ((LM_ERROR, "Error in WaitForMultipleObejcts\n"));
          ACE_OS::exit (0);
        }

      // semaphore_count_ will be displayed by the "main" thread.
      // It's value must be 2.  Note that although this is a shared
      // resource it's not protected via a mutex because the ++
      // operation on Intel is atomic.

      semaphore_count_++;
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) thread has been signaled.\n"));

      // Yield this thread so that the other one(s) have a chance to
      // run.
      ACE_OS::thr_yield ();
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int thread_count = THREAD_COUNT;

  if (argc > 1)
    thread_count = ACE_OS::atoi (argv[1]);

  wfmo_test.open ((void *) thread_count);

  // Initialize the semaphores.
  int result = ACE_OS::sema_init (&wfmo_test.sema_handles_[0], thread_count + 5);
  ACE_ASSERT (result != -1);

  result = ACE_OS::sema_init (&wfmo_test.sema_handles_[1], thread_count + 5);
  ACE_ASSERT (result != -1);

  for (int i = 0; i < MAX_ITERATIONS; i++)
    {
      wfmo_test.semaphore_count_ = 0;

      result = ACE_OS::sema_post (&wfmo_test.sema_handles_[0]);
      ACE_ASSERT (result != -1);

      // No real synchronization here. Just sleep enough so that at
      // least one (or two threads) run as a result of the semaphore.
      ACE_OS::sleep (1);

      // Add one for the other thread that was signaled.
      ACE_DEBUG ((LM_DEBUG,
                  "semaphore_count_ = %d  (should have been %d).\n",
                  wfmo_test.semaphore_count_,
                  2)); // Two semaphores should have been released.
    }

  ACE_OS::exit (0);

  return 0;
}
#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_DEBUG ((LM_DEBUG, "this test only runs on Win32\n"));
}
#endif /* ACE_WIN32 */
