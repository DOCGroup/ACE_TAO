// $Id$

#include "thread_pool.h"

/* We need this header so that we can invoke handle_input() on the
   objects we dequeue.  */
#include "ace/Event_Handler.h"

/* All we do here is initialize our active thread counter.  */
Thread_Pool::Thread_Pool (void)
  : active_threads_ (0)
{
}

/* Our open() method is a thin disguise around the ACE_Task<>
   activate() method.  By hiding activate() in this way, the users of
   Thread_Pool don't have to worry about the thread configuration
   flags.  */
int
Thread_Pool::open (int pool_size)
{
  return this->activate (THR_NEW_LWP|THR_DETACHED, pool_size);
}

/* Closing the thread pool can be a tricky exercise.  I've decided to
   take an easy approach and simply enqueue a secret message for each
   thread we have active.  */
int
Thread_Pool::close (u_long flags)
{
  ACE_UNUSED_ARG(flags);

  /* Find out how many threads are currently active */
  int counter = active_threads_.value ();

  /* For each one of the active threads, enqueue a "null" event
    handler.  Below, we'll teach our svc() method that "null" means
    "shutdown".  */
  while (counter--)
    this->enqueue (0);

  /* As each svc() method exits, it will decrement the active thread
    counter.  We just wait here for it to reach zero.  Since we don't
    know how long it will take, we sleep for a quarter of a second
    between tries.  */
  while (active_threads_.value ())
    ACE_OS::sleep (ACE_Time_Value (0, 250000));

  return(0);
}

/* When an object wants to do work in the pool, it should call the
   enqueue() method.  We introduce the ACE_Message_Block here but,
   unfortunately, we seriously misuse it.  */
int
Thread_Pool::enqueue (ACE_Event_Handler *handler)
{
  /* An ACE_Message_Block is a chunk of data.  You put them into an
    ACE_Message_Queue.  ACE_Task<> has an ACE_Message_Queue built in.
    In fact, the parameter to ACE_Task<> is passed directly to
    ACE_Message_Queue.  If you look back at our header file you'll see
    that we used ACE_MT_SYNCH as the parameter indicating that we want
    MultiThread Synch safety.  This allows us to safely put
    ACE_Message_Block objects into the message queue in one thread and
    take them out in another.  */

  /* An ACE_Message_Block wants to have char* data.  We don't have
    that.  We could cast our ACE_Event_Handler* directly to a char*
    but I wanted to be more explicit.  Since casting pointers around
    is a dangerous thing, I've gone out of my way here to be very
    clear about what we're doing.

    First: Cast the handler pointer to a void pointer.  You can't do
    any useful work on a void pointer, so this is a clear message that
    we're making the pointer unusable.

    Next: Cast the void pointer to a char pointer that the ACE_Message_Block will accept.  */
  void *v_data = (void *) handler;
  char *c_data = (char *) v_data;

  ACE_Message_Block *mb;

  /* Construct a new ACE_Message_Block.  For efficiency, you might
    want to preallocate a stack of these and reuse them.  For
    simplicity, I'll just create what I need as I need it.  */
  ACE_NEW_RETURN (mb,
                  ACE_Message_Block (c_data),
                  -1);

  /* Our putq() method is a wrapper around one of the enqueue methods
    of the ACE_Message_Queue that we own.  Like all good methods, it
    returns -1 if it fails for some reason.  */
  if (this->putq (mb) == -1)
    {
      /* Another trait of the ACE_Message_Block objects is that they
        are reference counted.  Since they're designed to be passed
        around between various objects in several threads we can't
        just delete them whenever we feel like it.  The release()
        method is similar to the destroy() method we've used
        elsewhere.  It watches the reference count and will delete the
        object when possible.  */
      mb->release ();
      return -1;
    }

  return 0;
}

/* The "guard" concept is very powerful and used throughout
   multi-threaded applications.  A guard normally does some operation
   on an object at construction and the "opposite" operation at
   destruction.  For instance, when you guard a mutex (lock) object,
   the guard will acquire the lock on construction and release it on
   destruction.  In this way, your method can simply let the guard go
   out of scope and know that the lock is released.

   Guards aren't only useful for locks however.  In this application
   I've created two guard objects for quite a different purpose.  */

/* The Counter_Guard is constructed with a reference to the thread
   pool's active thread counter.  The guard increments the counter
   when it is created and decrements it at destruction.  By creating
   one of these in svc(), I know that the counter will be decremented
   no matter how or where svc() returns.  */
class Counter_Guard
{
public:
  Counter_Guard (Thread_Pool::counter_t &counter)
    : counter_ (counter)
  {
    ++counter_;
  }

  ~Counter_Guard (void)
  {
    --counter_;
  }

protected:
  Thread_Pool::counter_t &counter_;
};

/* My Message_Block_Guard is also a little non-traditional.  It
   doesn't do anything in the constructor but it's destructor ensures
   that the message block's release() method is called.  This is a
   cheap way to prevent a memory leak if I need an additional exit
   point in svc().  */
class Message_Block_Guard
{
public:
  Message_Block_Guard (ACE_Message_Block *&mb)
    : mb_ (mb)
  {
  }

  ~Message_Block_Guard (void)
  {
    mb_->release ();
  }

protected:
  ACE_Message_Block *&mb_;
};

/* Now we come to the svc() method.  As I said, this is being executed
   in each thread of the Thread_Pool.  Here, we pull messages off of
   our built-in ACE_Message_Queue and cause them to do work.  */
int
Thread_Pool::svc (void)
{
  /* The getq() method takes a reference to a pointer.  So... we need
    a pointer to give it a reference to.  */
  ACE_Message_Block *mb;

  /* Create the guard for our active thread counter object.  No matter
    where we choose to return() from svc(), we now know that the
    counter will be decremented.  */
  Counter_Guard counter_guard (active_threads_);

  /* Get messages from the queue until we have a failure.  There's no
    real good reason for failure so if it happens, we leave
    immediately.  */
  while (this->getq (mb) != -1)
    {
      /* A successful getq() will cause "mb" to point to a valid
        refernce-counted ACE_Message_Block.  We use our guard object
        here so that we're sure to call the release() method of that
        message block and reduce it's reference count.  Once the count
        reaches zero, it will be deleted.  */
      Message_Block_Guard message_block_guard (mb);

      /* As noted before, the ACE_Message_Block stores it's data as a
        char*.  We pull that out here and later turn it into an
        ACE_Event_Handler* */
      char *c_data = mb->base ();

      /* We've chosen to use a "null" value as an indication to leave.
        If the data we got from the queue is not null then we have
        some work to do.  */
      if (c_data)
        {
          /* Once again, we go to great lengths to emphasize the fact
            that we're casting pointers around in rather impolite
            ways.  We could have cast the char* directly to an
            ACE_Event_Handler* but then folks might think that's an OK
            thing to do.

            (Note: The correct way to use an ACE_Message_Block is to
            write data into it.  What I should have done was create a
            message block big enough to hold an event handler pointer
            and then written the pointer value into the block.  When
            we got here, I would have to read that data back into a
            pointer.  While politically correct, it is also a lot of
            work.  If you're careful you can get away with casting
            pointers around.)  */
          void *v_data = (void *) c_data;

          ACE_Event_Handler *handler = (ACE_Event_Handler *) v_data;

          /* Now that we finally have an event handler pointer, invoke
            it's handle_input() method.  Since we don't know it's
            handle, we just give it a default.  That's OK because we
            know that we're not using the handle in the method anyway.  */
          if (handler->handle_input (ACE_INVALID_HANDLE) == -1)
            {
              /* Tell the handler that it's time to go home.  The
                "normal" method for shutting down a handler whose
                handler failed is to invoke handle_close().  This will
                take care of cleaning it up for us.  Notice how we use
                the handler's get_handle() method to populate it's
                "handle" parameter.  Convenient isn't it?  */
              handler->handle_close (handler->get_handle (), 0);

              /* Also notice that we don't exit the svc() method here!
                The first time I did this, I was exiting.  After a few
                clients disconnect you have an empty thread pool.
                Hard to do any more work after that...  */
            }
        }
      else
        /* If we get here, we were given a message block with "null"
           data.  That is our signal to leave, so we return(0) to
           leave gracefully.  */
          return 0;		// Ok, shutdown request

      // message_block_guard goes out of scope here and releases the
      // message_block instance.
    }

  return 0;
}

