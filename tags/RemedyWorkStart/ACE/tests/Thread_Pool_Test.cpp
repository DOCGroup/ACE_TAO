// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Thread_Pool_Test.cpp
//
// = DESCRIPTION
//     This test program illustrates how the <ACE_Task>
//     synchronization mechanisms work in conjunction with the
//     <ACE_Thread_Manager>.  If the <manual> flag is set input comes
//     from stdin until the user enters a return -- otherwise, the
//     input is generated automatically.  All worker threads shutdown
//     when (1) they receive a message block of length 0 or (2) the
//     queue is deactivated.
//
// = AUTHOR
//    Karlheinz Dorn <Karlheinz.Dorn@med.siemens.de>,
//    Douglas C. Schmidt <schmidt@cs.wustl.edu>, and
//    Prashant Jain <pjain@cs.wustl.edu>
//
// ============================================================================

#include "test_config.h"
#include "ace/Task.h"

#if defined (ACE_HAS_THREADS)
#include "ace/Lock_Adapter_T.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"

// Number of iterations to run the test.
static size_t n_iterations = 100;

// Controls whether the input is generated "manually" or automatically.
static int manual = 0;

class Thread_Pool : public ACE_Task<ACE_MT_SYNCH>
{
  // = TITLE
  //   Defines a thread pool abstraction based on the <ACE_Task>.
public:
  Thread_Pool (int n_threads);
  // Create the thread pool containing <n_threads>.

  ~Thread_Pool (void);
  // Destructor...

  int test_queue_deactivation_shutdown (void);
  // Activate the task's thread pool, produce the messages that are
  // consumed by the threads in the thread pool, and demonstate how to
  // shutdown using the <ACE_Message_Queue::deactivate> method.

  int test_empty_message_shutdown (void);
  // Activate the task's thread pool, produce the messages that are,
  // produce the messages that are consumed by the threads in the
  // thread pool, and demonstrate how to shutdown by enqueueing
  // "empty" messages into the queue.

  virtual int svc (void);
  // Iterate <n_iterations> time printing off a message and "waiting"
  // for all other threads to complete this iteration.

  virtual int put (ACE_Message_Block *mb,
                   ACE_Time_Value *tv = 0);
  // Allows the producer to pass messages to the <Thread_Pool>.

private:
  //FUZZ: disable check_for_lack_ACE_OS
  virtual int open (void * = 0);
  // Spawn the threads in the pool.

  virtual int close (u_long);
  // Close hook.
  //FUZZ: enable check_for_lack_ACE_OS

  ACE_Lock_Adapter<ACE_Thread_Mutex> lock_adapter_;
  // Serialize access to <ACE_Message_Block> reference count, which
  // will be decremented by multiple threads.

  int n_threads_;
  // Number of threads to spawn.
};

Thread_Pool::~Thread_Pool (void)
{
}

int
Thread_Pool::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) worker thread closing down\n")));
  return 0;
}

Thread_Pool::Thread_Pool (int n_threads)
  : n_threads_ (n_threads)
{
}

// Simply enqueue the Message_Block into the end of the queue.

int
Thread_Pool::put (ACE_Message_Block *mb,
                  ACE_Time_Value *tv)
{
  return this->putq (mb, tv);
}

// Iterate <n_iterations> printing off a message and "waiting" for all
// other threads to complete this iteration.

int
Thread_Pool::svc (void)
{
  // Keep looping, reading a message out of the queue, until we get a
  // message with a length == 0, which signals us to quit.

  for (int count = 1; ; count++)
    {
      ACE_Message_Block *mb = 0;

      int result = this->getq (mb);

      ACE_TEST_ASSERT (result != -1 || errno == ESHUTDOWN);

      if (result == -1 && errno == ESHUTDOWN)
        {
          // The queue has been deactivated, so let's bail out.
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%t) in iteration %d, queue len = %d, ")
                      ACE_TEXT ("queue deactivated, exiting\n"),
                      count,
                      this->msg_queue ()->message_count ()));

          break;
        }

      size_t length = mb->length ();

      if (length > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) in iteration %d, queue len = %d, ")
                    ACE_TEXT ("length = %d, text = \"%*s\"\n"),
                    count,
                    this->msg_queue ()->message_count (),
                    length,
                    length - 1,
                    mb->rd_ptr ()));

      // We're responsible for deallocating this.
      mb->release ();

      if (length == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%t) in iteration %d, queue len = %d, ")
                      ACE_TEXT ("got \"empty\" message, exiting\n"),
                      count,
                      this->msg_queue ()->message_count ()));
          break;
        }
    }

  // Note that the <ACE_Task::svc_run> method automatically removes us
  // from the <ACE_Thread_Manager> when the thread exits.
  return 0;
}

int
Thread_Pool::open (void *)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) producer start, dumping the Thread_Pool\n")));
  this->dump ();

  // Create a pool of worker threads.
  if (this->activate (THR_NEW_LWP,
                      this->n_threads_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("activate failed")),
                      -1);
  return 0;
}

// Activate the task's thread pool, produce the messages that are
// consumed by the threads in the thread pool, and demonstate how to
// shutdown using the <ACE_Message_Queue::deactivate> method.

int
Thread_Pool::test_queue_deactivation_shutdown (void)
{
  if (this->open () == -1)
    return -1;

  ACE_Message_Block *mb = 0;

  // Run the main loop that generates messages and enqueues them into
  // the pool of threads managed by <ACE_Task>.

  for (size_t count = 0;
       ;
       count++)
    {
      ssize_t n = 0;

      // Allocate a new message.
      ACE_NEW_RETURN (mb,
                      ACE_Message_Block (BUFSIZ,
                                         ACE_Message_Block::MB_DATA,
                                         0,
                                         0,
                                         0,
                                         &this->lock_adapter_),
                      -1);

      if (manual)
        {
#if !defined (ACE_HAS_WINCE)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%t) enter a new message for ")
                      ACE_TEXT ("the task pool...")));
          n = ACE_OS::read (ACE_STDIN,
                            mb->wr_ptr (),
                            mb->size ());
#endif  // ACE_HAS_WINCE
        }
      else
        {
          static size_t count = 0;

          ACE_OS::sprintf (reinterpret_cast<ACE_TCHAR *> (mb->wr_ptr ()),
                           ACE_SIZE_T_FORMAT_SPECIFIER,
                           count);
          n = ACE_OS::strlen (mb->rd_ptr ());

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
          mb->wr_ptr (n * sizeof (ACE_TCHAR));

          // Pass the message to the Thread_Pool.
          if (this->put (mb) == -1)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT (" (%t) %p\n"),
                        ACE_TEXT ("put")));
        }
      else
        {
          // Release the <Message_Block> since we're shutting down and
          // don't need it anymore.

          mb->release ();
          // Deactivate the message queue and return.
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("\n(%t) deactivating queue for %d threads, ")
                      ACE_TEXT ("dump of task:\n"),
                      this->thr_count ()));
          this->dump ();

          // Deactivate the queue.
          return this->msg_queue ()->deactivate ();
        }
    }
}

// Activate the task's thread pool, produce the messages that are,
// produce the messages that are consumed by the threads in the thread
// pool, and demonstrate how to shutdown by enqueueing "empty"
// messages into the queue.

int
Thread_Pool::test_empty_message_shutdown (void)
{
  if (this->open () == -1)
    return -1;

  ACE_Message_Block *mb = 0;

  // Run the main loop that generates messages and enqueues them into
  // the pool of threads managed by <ACE_Task>.

  for (size_t count = 0;
       ;
       count++)
    {
      ssize_t n = 0;

      // Allocate a new message.
      ACE_NEW_RETURN (mb,
                      ACE_Message_Block (BUFSIZ,
                                         ACE_Message_Block::MB_DATA,
                                         0,
                                         0,
                                         0,
                                         &this->lock_adapter_),
                      -1);

      if (manual)
        {
#if !defined (ACE_HAS_WINCE)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%t) enter a new message for ")
                      ACE_TEXT ("the task pool...")));
          n = ACE_OS::read (ACE_STDIN,
                            mb->wr_ptr (),
                            mb->size ());
#endif  // ACE_HAS_WINCE
        }
      else
        {
          static size_t count = 0;

          ACE_OS::sprintf (reinterpret_cast<ACE_TCHAR *> (mb->wr_ptr ()),
                           ACE_SIZE_T_FORMAT_SPECIFIER,
                           count);
          n = ACE_OS::strlen (mb->rd_ptr ());

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
          mb->wr_ptr (n * sizeof (ACE_TCHAR));

          // Pass the message to the Thread_Pool.
          if (this->put (mb) == -1)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT (" (%t) %p\n"),
                        ACE_TEXT ("put")));
        }
      else
        {
          // Send a shutdown message to the waiting threads and return.
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("\n(%t) sending shutdown message to %d threads, ")
                      ACE_TEXT ("dump of task:\n"),
                      this->thr_count ()));
          this->dump ();

          size_t i = 0;

          // Enqueue an empty message to flag each consumer thread to
          // inform it to shutdown.
          for (i = this->thr_count ();
               i > 0;
               i--)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%t) end of input, ")
                          ACE_TEXT ("enqueueing \"empty\" message %d\n"),
                          i));

              // Note the use of reference counting to avoid copying
              // the message contents.
              ACE_Message_Block *dup = mb->duplicate ();

              if (this->put (dup) == -1)
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT (" (%t) %p\n"),
                            ACE_TEXT ("put")));
            }

          mb->release ();

          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("\n(%t) end loop, dump of task:\n")));
          this->dump ();

          return 0;
        }
    }
}

#endif /* ACE_HAS_THREADS */

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Thread_Pool_Test"));

#if defined (ACE_HAS_THREADS)
  int n_threads = ACE_MAX_THREADS;

  // Create the worker tasks.
  Thread_Pool thread_pool (n_threads);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) running test with %d threads\n"),
              n_threads));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) starting empty message shutdown test\n")));

  // Activate the task's thread pool, produce the messages that are,
  // produce the messages that are consumed by the threads in the
  // thread pool, and demonstrate how to shutdown by enqueueing
  // "empty" messages into the queue.
  if (thread_pool.test_empty_message_shutdown () == -1)
    return 1;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) waiting for worker tasks to finish...\n")));
  // Wait for all the threads to reach their exit point, at which
  // point the barrier in the destructor of the <ACE_Task> portion of
  // <Thread_Pool> will return.
  if (thread_pool.wait () == -1)
    return 1;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) starting queue deactivation shutdown test\n")));

  // Activate the task's thread pool, produce the messages that are
  // consumed by the threads in the thread pool, and demonstate how to
  // shutdown using the <ACE_Message_Queue::deactivate> method.
  if (thread_pool.test_queue_deactivation_shutdown () == -1)
    return 1;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) waiting for worker tasks to finish...\n")));
  // Wait for all the threads to reach their exit point, at which
  // point the barrier in the destructor of the <ACE_Task> portion of
  // <Thread_Pool> will return.
  if (thread_pool.wait () == -1)
    return 1;

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) all worker tasks destroyed, exiting test...\n")));
#else
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}
