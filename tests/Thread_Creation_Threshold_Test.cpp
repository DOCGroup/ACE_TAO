
//=============================================================================
/**
 *  @file    Thread_Creation_Threshold_Test.cpp
 *
 *  $Id$
 *
 *   This test program stresses how many threads can be
 *   consecutively (not simultaneously) created on a platform.
 *   Rather than testing exhaustively, it establishes a
 *   semi-arbitrary upper limit (MAX_THREAD)of threads.  The limit
 *   is only partly arbitrary because it was chosen as a value that
 *   exceeded an observed upper limit on the values that Solaris 9
 *   will accept as arguments to thr_concurrency(), used by
 *   ACE_OS::thr_create(THR_NEW_LWP).
 *
 *
 *  @author Chris Cleeland <cleeland@ociweb.com>
 */
//=============================================================================


#include "test_config.h"
#include "ace/Task.h"



#if defined (ACE_HAS_THREADS)
#include "ace/Lock_Adapter_T.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"

// Number of iterations to run the test.
#if defined (ACE_VXWORKS)
const int THREADS_PER_POOL = 50;
const unsigned long MAX_THREAD = 500;
#else
const int THREADS_PER_POOL = 1000;
const unsigned long MAX_THREAD = 100000;
#endif

namespace
{
  // Change this to 'true' if you want lots of debugging messages in the log
  const bool PRINT_DEBUG_MSGS = true;
}

/**
 * @class Thread_Pool
 *
 * @brief Defines a thread pool abstraction based on the <ACE_Task>.
 */
class Thread_Pool : public ACE_Task<ACE_MT_SYNCH>
{
public:
  /// Create the thread pool containing <n_threads>.
  Thread_Pool (int n_threads);

  /// Destructor...
  ~Thread_Pool (void);

  /// Iterate <n_iterations> time printing off a message and "waiting"
  /// for all other threads to complete this iteration.
  virtual int svc (void);

  /// Start the threads in the pool.
  void start();

  unsigned long total_threads()
    {
      return this->total_activated_threads_.value();
    }

  /// Number of threads to spawn.
  int n_threads_;

  /// Returns true iff failed_ == false.
  bool operator! ();

private:
  //FUZZ: disable check_for_lack_ACE_OS
  /// Spawn the threads in the pool.
  virtual int open (void * = 0);

  /// Close hook.
  ///FUZZ: enable check_for_lack_ACE_OS
  virtual int close (u_long);

  /// Total number of threads activated through this thread pool ever.
  ACE_Atomic_Op<ACE_Thread_Mutex, unsigned long> total_activated_threads_;

  /// Flag set only in worker threads to indicate whether they should print
  /// debug messages.
  bool doprint_;

  /// Flag indicating that start() failed.
  bool failed_;
};

bool
Thread_Pool::operator!()
{
  return ! this->failed_;
}

Thread_Pool::~Thread_Pool (void)
{
}

int
Thread_Pool::close (u_long)
{
  if (this->doprint_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) worker thread closing down\n")));
  return 0;
}

Thread_Pool::Thread_Pool (int n_threads)
  : n_threads_ (n_threads)
  , total_activated_threads_ (0)
  , doprint_ (false)
  , failed_ (false)
{
}

void
Thread_Pool::start ()
{
  this->failed_ = false;
  if (this->open () == -1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%P|%t) thread activation ")
                  ACE_TEXT ("failed after %u threads\n"),
                  this->total_threads()));
      this->failed_ = true;
    }
}

int
Thread_Pool::svc (void)
{
  unsigned long t = ++this->total_activated_threads_;
  if (PRINT_DEBUG_MSGS) // change this to 'true' for debugging
    this->doprint_ = (t > 65530);

  if (this->doprint_)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT("  (%P|%t) thread started.\n")));

  // Note that the <ACE_Task::svc_run> method automatically removes us
  // from the <ACE_Thread_Manager> when the thread exits.
  return 0;
}

namespace {
  void no_op() { }
}

int
Thread_Pool::open (void *)
{
  if (PRINT_DEBUG_MSGS)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%P|%t) pool start %d threads...\n"),
                this->n_threads_));

  if (this->total_activated_threads_ >= (65534 - this->n_threads_))
    no_op ();

  // Create a pool of worker threads.
  if (this->activate (THR_NEW_LWP,
                      this->n_threads_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("activate failed")),
                      -1);

  return 0;
}

#endif /* ACE_HAS_THREADS */


int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Thread_Creation_Threshold_Test"));

#if defined (ACE_HAS_THREADS)
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) will try to start and kill up")
              ACE_TEXT (" to %u threads sequentially\n"),
              MAX_THREAD));
  int initial_pool_size = 50;

  // Create the worker tasks.
  Thread_Pool thread_pool (initial_pool_size);

  while (!thread_pool && thread_pool.total_threads() < MAX_THREAD)
    {
      // Activate the task's thread pool, produce the messages that are,
      // produce the messages that are consumed by the threads in the
      // thread pool, and demonstrate how to shutdown by enqueueing
      // "empty" messages into the queue.
      thread_pool.start ();

      // Wait for all the threads to reach their exit point, at which
      // point the barrier in the destructor of the <ACE_Task> portion of
      // <Thread_Pool> will return.
      if (thread_pool.wait () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("wait")), 1);
        }

      if (PRINT_DEBUG_MSGS)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%u total threads\n"),
                    thread_pool.total_threads()));

      int& n_threads = thread_pool.n_threads_;
      const unsigned long THREAD_THRESHOLD = 63336;
      if (thread_pool.total_threads() >= (THREAD_THRESHOLD - n_threads))
        n_threads = 1;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%u total threads successfully started and died;")
              ACE_TEXT (" expected %u.\n"),
              thread_pool.total_threads (),
              MAX_THREAD));
#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
