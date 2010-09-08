// $Id$

// This test program illustrates how the <ACE_Task> synchronization
// mechanisms work in conjunction with the <ACE_Thread_Manager>.  If
// the <manual> flag is set input comes from stdin until the user
// enters a return -- otherwise, the input is generated automatically.
// All worker threads shutdown when they receive a message block of
// length 0.
//
// This code is original based on a test program written by Karlheinz
// Dorn <Karlheinz.Dorn@med.siemens.de>.  It was modified to utilize
// more ACE features by Doug Schmidt <schmidt@cs.wustl.edu>.

#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_main.h"
#include "ace/Task.h"
#include "ace/Service_Config.h"



#if defined (ACE_HAS_THREADS)

// Default number of iterations to run the test.
static int n_iterations = 100;

// Controls whether the input is generated "manually" or automatically.
static int manual = 0;

class Thread_Pool : public ACE_Task<ACE_MT_SYNCH>
{
  // = TITLE
  //   Defines a thread pool abstraction based on the <ACE_Task>.
public:
  Thread_Pool (ACE_Thread_Manager *thr_mgr,
               int n_threads);
  // Constructor activates <n_threads> in the thread pool.

  ~Thread_Pool (void);
  // Destructor...

  virtual int svc (void);
  // Iterate <n_iterations> time printing off a message and "waiting"
  // for all other threads to complete this iteration.

  virtual int put (ACE_Message_Block *mb,
                   ACE_Time_Value *tv = 0);
  // This allows the producer to pass messages to the <Thread_Pool>.

private:
  //FUZZ: disable check_for_lack_ACE_OS
  virtual int close (u_long);
  // Close hook.
  //FUZZ: enable check_for_lack_ACE_OS
};

int
Thread_Pool::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) worker thread closing down\n"));
  return 0;
}

Thread_Pool::Thread_Pool (ACE_Thread_Manager *thr_mgr,
                          int n_threads)
  : ACE_Task<ACE_MT_SYNCH> (thr_mgr)
{
  // Create the pool of worker threads.
  if (this->activate (THR_NEW_LWP,
                      n_threads) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "activate failed"));
}

Thread_Pool::~Thread_Pool (void)
{
}

// Simply enqueue the Message_Block into the end of the queue.

int
Thread_Pool::put (ACE_Message_Block *mb,
                  ACE_Time_Value *tv)
{
  return this->putq (mb, tv);
}

// Iterate <n_iterations> time printing off a message and "waiting"
// for all other threads to complete this iteration.

int
Thread_Pool::svc (void)
{
  // Note that the <ACE_Task::svc_run> method automatically adds us to
  // the Thread_Manager when the thread begins.

  int count = 1;

  // Keep looping, reading a message out of the queue, until we get a
  // message with a length == 0, which signals us to quit.

  for (;; count++)
    {
      ACE_Message_Block *mb = 0;

      ACE_DEBUG ((LM_DEBUG,
                  "(%t) in iteration %d before getq ()\n",
                  count));

      if (this->getq (mb) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%t) in iteration %d, got result -1, exiting\n",
                      count));
          break;
        }

      size_t length = mb->length ();

      if (length > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "(%t) in iteration %d, length = %d, text = \"%*s\"\n",
                    count,
                    length,
                    length - 1,
                    mb->rd_ptr ()));

      // We're responsible for deallocating this.
      mb->release ();

      if (length == 0)
        {
          //FUZZ: disable check_for_NULL
          ACE_DEBUG ((LM_DEBUG,
                      "(%t) in iteration %d, got NULL message, exiting\n",
                      count));
          //FUZZ: enable check_for_NULL

          break;
        }
    }

  // Note that the <ACE_Task::svc_run> method automatically removes us
  // from the <ACE_Thread_Manager> when the thread exits.
  return 0;
}

static void
producer (Thread_Pool &thread_pool)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) producer start, generating data for the <Thread_Pool>\n"));
  // thread_pool.dump ();

  for (int n; ;)
    {
      // Allocate a new message.
      ACE_Message_Block *mb = 0;
      ACE_NEW (mb,
               ACE_Message_Block (BUFSIZ));

      if (manual)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%t) enter a new message for the task pool..."));
          n = ACE_OS::read (ACE_STDIN,
                            mb->rd_ptr (),
                            mb->size ());
        }
      else
        {
          static int count = 0;

          ACE_OS::sprintf (mb->rd_ptr (),
                           "%d\n",
                           count);
          n = ACE_Utils::truncate_cast<int> (ACE_OS::strlen (mb->rd_ptr ()));

          if (count == n_iterations)
            n = 1; // Indicate that we need to shut down.
          else
            count++;

          if (count == 0 || (count % 20 == 0))
            ACE_OS::sleep (1);
        }

      if (n > 1)
        {
          // Send a normal message to the waiting threads and continue
          // producing.
          mb->wr_ptr (n);

          // Pass the message to the Thread_Pool.
          if (thread_pool.put (mb) == -1)
                  ACE_ERROR ((LM_ERROR,
                              " (%t) %p\n",
                              "put"));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,
                            "\n(%t) start loop, dump of task:\n"));
          // thread_pool.dump ();

                // Send a shutdown message to the waiting threads and exit.
          for (size_t i = thread_pool.thr_count (); i > 0; i--)
            {
              //FUZZ: disable check_for_NULL
              ACE_DEBUG ((LM_DEBUG,
                          "(%t) EOF, enqueueing NULL block for thread = %d\n",
                          i));
              //FUZZ: enable check_for_NULL

              // Enqueue a NULL message to flag each consumer to
              // shutdown.
              ACE_Message_Block *mb = 0;
              ACE_NEW (mb,
                       ACE_Message_Block);
              if (thread_pool.put (mb) == -1)
                      ACE_ERROR ((LM_ERROR,
                                  " (%t) %p\n",
                                  "put"));
            }

          ACE_DEBUG ((LM_DEBUG,
                      "\n(%t) end loop\n"));
          // thread_pool.dump ();
          break;
        }
    }
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int n_threads = argc > 1 ? ACE_OS::atoi (argv[1]) : ACE_DEFAULT_THREADS;
  n_iterations = argc > 2 ? ACE_OS::atoi (argv[2]) : n_iterations;
  manual = argc > 3 ? 1 : 0;

  ACE_DEBUG ((LM_DEBUG,
              "(%t) argc = %d, threads = %d\n",
              argc,
              n_threads));

  // Create the worker tasks.
  Thread_Pool thread_pool (ACE_Thread_Manager::instance (),
                           n_threads);

  // Create work for the worker tasks to process in their own threads.
  producer (thread_pool);

  ACE_DEBUG ((LM_DEBUG,
              "(%t) waiting for threads to exit in Thread_Pool destructor...\n"));
  // Wait for all the threads to reach their exit point.
  if (thread_pool.wait () == -1)
    //FUZZ: disable check_for_lack_ACE_OS
    ACE_ERROR_RETURN ((LM_ERROR, "(%t) wait() failed\n"),
                      1);
    //FUZZ: enable check_for_lack_ACE_OS

  ACE_DEBUG ((LM_DEBUG,
              "(%t) destroying worker tasks and exiting...\n"));
  return 0;
}
#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR ((LM_ERROR,
              "threads not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */
