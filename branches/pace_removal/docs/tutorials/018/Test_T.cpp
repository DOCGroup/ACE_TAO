// $Id$

/* This is something new... Since we're included by the header, we
   have to provide a sentry to protect against recursive inclusion.
 */
#ifndef TEST_T_C
#define TEST_T_C

// Get our definition
#include "Test_T.h"

// We'll hard-code the thread count.  Mucking around with that isn't
// really the point of the exercise today...
#define TEST_THREAD_COUNT 5

/* Construction time...
   Initialize the baseclass, the name and the barrier.  Since the
   client will probably invoke run() next, we go ahead an announce our
   creation to make the output more readable.
 */
template <class MUTEX>
Test_T<MUTEX>::Test_T (const char *name)
  : ACE_Task<ACE_MT_SYNCH>(),
    name_ (name),
    barrier_ (TEST_THREAD_COUNT)
{
  ACE_DEBUG ((LM_INFO,
              "(%P|%t|%T)\tTest_T (%s) created\n",
              name));
}

/* Activate the threads and create some test data...
 */
template <class MUTEX> int
Test_T<MUTEX>::run (void)
{
  // Try to activate the set of threads that will test the mutex
  if (this->open () == -1)
    return -1;

  // Create a set of messages.  I chose twice the thread count so that
  // we can see how they get distributed.
  for (int i = 0; i < TEST_THREAD_COUNT*2; ++i)
    {
      // A message block big enough for a simple message.
      ACE_Message_Block *message;

      ACE_NEW_RETURN (message,
                      ACE_Message_Block (64),
                      -1);

      // Put some text into the message block so that we can know
      // what's going on when we get to svc()
      sprintf (message->wr_ptr (),
               "Message Number %d",
               i);
      message->wr_ptr (ACE_OS::strlen (message->rd_ptr ()) + 1);

      // Send the message to the thread pool
      if (this->send (message) == -1)
        break;
    }

  // Send a hangup to the thread pool so that we can exit.
  if (this->send () == -1)
    return -1;

  // Wait for all of the threads to exit and then return to the client.
  return this->wait ();
}

/* Send a message to the thread pool
 */
template <class MUTEX> int
Test_T<MUTEX>::send (ACE_Message_Block *message)
{
  // If no message was provided, create a hangup message.
  if (message == 0)
  {
    ACE_Message_Block::ACE_Message_Type mb_hangup =
        ACE_Message_Block::MB_HANGUP ;

    ACE_NEW_RETURN (message,
                    ACE_Message_Block (0, mb_hangup),
                    -1);
  }

  // Use the duplicate() method when sending the message.  For this
  // simple application, that may be overkill but it's a good habit.
  // duplicate() will increment the reference count so that each user
  // of the message can release() it when done.  The last user to call
  // release() will cause the data to be deleted.
  if (this->putq (message->duplicate ()) == -1)
    {
      // Error?  release() the message block and return failure.
      message->release ();
      return -1;
    }

  // release() the data to prevent memory leaks.
  message->release();

  return 0;
}

/* A farily typical open().  Just activate the set of threads and return.
 */
template <class MUTEX> int
Test_T<MUTEX>::open (void *arg)
{
  ACE_UNUSED_ARG(arg);
  return this->activate (THR_NEW_LWP,
                         TEST_THREAD_COUNT);
}

/* svc() is also fairly typical.  The new part is the use of the guard
   to simulate protection of shared resources.
 */
template <class MUTEX> int
Test_T<MUTEX>::svc (void)
{
  // Keep a simple thread identifier.  We could always use the
  // thread id but this is a nice, simple number.
  int my_number = ++thread_num_;

  ACE_DEBUG ((LM_INFO,
              "%d (%P|%t|%T)\tTest_T::svc() Entry\n",
              my_number));

  // Wait for all of threads to get started so that they all have a
  // fair shot at the message queue.  Comment this out and see how the
  // behaviour changes.  Does it surprise you?
  barrier_.wait ();

  ACE_Message_Block *message;
  int mcount = 0;

  // This would usually be an almost-infinite loop.  Instead, I've
  // governed it so that no single thread can get more than "thread
  // count" number of messages.  You'll see that with ACE_Mutex, this
  // is just about the only way to keep the first thread from getting
  // all the action.  Ths is obviously just for sake of the test since
  // you don't want your real-world app to exit after a fixed number
  // of messages!
  while (mcount < TEST_THREAD_COUNT)
    {
      // Get a message.  Since the message queue is already
      // thread-safe we don't have to guard it.  In fact, moving the
      // guard up above getq() will decrease your parallelization.
      if (getq (message) == -1)
        break;

      // Now we pretend that there are shared resources required to
      // process the data.  We grab the mutex through the guard and
      // "do work".  In a real application, you'll want to keep these
      // critical sections as small as possible since they will reduce
      // the usefulness of multi-threading.
      guard_t guard (mutex_);

      // Increase our message count for the debug output and the
      // governor.
      ++mcount;

      // Check for a hangup request...  Notice the use of release()
      // again to prevent leaks
      if (message->msg_type () == ACE_Message_Block::MB_HANGUP)
        {
          message->release ();
          break;
        }

      // Display the message so that we can see if things are working
      // the way we want.
      ACE_DEBUG ((LM_INFO,
                  "%d (%P|%t|%T)\tTest_T::svc() received message #%d (%s)\n",
                  my_number,
                  mcount,
                  message->rd_ptr ()));

      // Pretend that the work takes some time to complete.  Remember,
      // we're holding that lock during this time!
      ACE_OS::sleep (1);

      // No leaks...
      message->release ();
    }

  // Send a hangup to the other threads in the pool.  If we don't do
  // this then wait() will never exit since all of the other threads
  // are still blocked on getq().
  this->send ();

  return 0;
};

#endif /* TEST_T_C */
