// $Id$

#include "task.h"
#include "block.h"

/* Set our housekeeping pointer to NULL and tell the user we exist.  */
Task::Task (size_t n_threads)
  : barrier_ (n_threads),
    n_threads_ (n_threads)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Task ctor 0x%x\n",
              (void *) this));
}

/* Take care of cleanup & tell the user we're going away.  */
Task::~Task (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Task dtor 0x%x\n",
              (void *) this));

  /* Get our shutdown notification out of the queue and release it.  */
  ACE_Message_Block *message;

  /* Like the getq() in svc() below, this will block until a message
    arrives.  By blocking, we know that the destruction will be paused
    until the last thread is done with the message block.  */
  this->getq (message);
  message->release ();
}

/* Open the object to do work.  Next, we activate the Task into the
  number of requested threads.  */
int
Task::open (void *unused)
{
  ACE_UNUSED_ARG (unused);

  return this->activate (THR_NEW_LWP,
                         n_threads_);
}

/* Tell the user we're closing and invoke the baseclass' close() to
  take care of things.  */
int
Task::close (u_long flags)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Task close 0x%x\n",
              (void *) this));
  return inherited::close (flags);
}

/* Our svc() method waits for work on the queue and then processes
  that work.  */
int
Task::svc (void)
{
  /* This will cause all of the threads to wait on this line until all
    have invoked this method.  The net result is that no thread in the
    Task will get a shot at the queue until all of the threads are
    active.  There's no real need to do this but it's an easy intro
    into the use of ACE_Barrier.  */
  this->barrier_.wait ();

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Task 0x%x starts in thread %d\n",
              (void *) this,
              ACE_Thread::self ()));

  /* Remember that get() needs a reference to a pointer.  To save
    stack thrashing we'll go ahead and create a pointer outside of the
    almost- infinite loop.  */
  ACE_Message_Block *message;

  for (;;)
    {
      /* Get a message from the queue.  Note that getq() will block
        until a message shows up.  That makes us very
        processor-friendly.  */
      if (this->getq (message) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "getq"),
                          -1);
      /* If we got the shutdown request, we need to go away.  */
      if (message->msg_type () == ACE_Message_Block::MB_HANGUP)
        {
          /* Forward the request to any peer threads.  */
          this->putq (message);

          /* Leave the infinite loop so that the thread exits.  */
          break;
        }

      /* The message queue stores char* data.  We use rd_ptr() to get
        to the beginning of the data.  */
      const char *cp = message->rd_ptr ();

      /* Move the rd_ptr() past the data we read.  This isn't real
        useful here since we won't be reading any more from the block
        but it's a good habit to get into.  */
      message->rd_ptr (ACE_OS::strlen (cp));

      /* Display the block's address and data to the user.  */
      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Block 0x%x contains (%s)\n",
                  (void *) message,
                  cp));

      /* Pretend that it takes a while to process the data.  */
      ACE_OS::sleep (ACE_Time_Value (0, 5000));

      /* Release the message block.  Notice that we never delete a
        message block.  Blocks are reference counted & the release()
        method will take care of the delete when there are no more
        references to the data.  */
      message->release ();
    }

  return 0;
}
