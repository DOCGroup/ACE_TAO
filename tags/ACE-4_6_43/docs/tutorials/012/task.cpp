
// $Id$

#include "task.h"
#include "block.h"
#include "work.h"

/*
  Boring default constructor.  Be sure our barrier_ is initialized in
  case we get destructed before opened.
*/
Task::Task (void)
: barrier_ (0)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Task ctor 0x%x\n", (void *) this));
}

/*
  You'll see in the svc() method that when we get a shutdown request,
  we always putq() it back into our message queue.  The last thread in 
  the pool will do this also and result in there always being one
  shutdown request left in the queue when we get here.  Just to be
  polite, we'll go ahead and get that message and release it.

  We also delete the barrier_ object we used to synch the svc()
  methods.
*/
Task::~Task (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Task dtor 0x%x\n", (void *) this));

  ACE_Message_Block *message;
  this->getq (message);
  message->release ();

  delete barrier_;
}

/*
  The ACE_Barrier needs to know how many threads it will be working
  for.  For that reason, we have to put off it's construction until we 
  get here.  We then pass the thread count through to our base class'
  activate().
*/
int Task::start (int threads)
{
  barrier_ = new ACE_Barrier (threads);
  return this->activate (THR_NEW_LWP, threads);
}

/*
  We don't really do anything here but I wanted to provide a message
  in the output.
*/
int Task::close (u_long flags)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Task close 0x%x\n", (void *) this));
  return inherited::close (flags);
}

/*
  Now the svc() method where everything interesting happens.
*/
int Task::svc (void)
{
     /*
       All of the threads will block here until the last thread
       arrives.  They will all then be free to begin doing work.
     */
  this->barrier_->wait ();

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Task 0x%x starts in thread %u\n", (void *) this, ACE_Thread::self ()));

   // Where we getq() the message
  ACE_Message_Block *message;
   // What we really put into the queue is a Message_Block, so we'll
   // cast the 'message' to 'message_block' after getting it.  I'm
   // going through some extra steps here just to be explicit
  Message_Block * message_block;
   // The baseclass of the work object we put into the queue.  Notice
   // that we can use this and not bother with the Work object at all.
  Unit_Of_Work * unit_of_work;

  while (1)
  {
       // Get the message...
    if (this->getq (message) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "getq"), -1);
    }

     // Is it a shutdown request?
    if (message->msg_type () == ACE_Message_Block::MB_HANGUP)
    {
         // Send the shutdown to all of our pool peers
      this->putq (message);
      break;
    }

     // Cast the pointer to our specialized Message_Block.  We could
     // have done this at the getq() call but I wanted to be explicit
     // about what we're doing here
    message_block = (Message_Block*)message;

     /*
       Since we left alone the ACE_Data_Block used by the
       Message_Block we have chosen to use it to send arbitrary data
       as well.
     */
    const char *cp = message_block->rd_ptr ();
     // Don't forget to skip the NULL we inserted
    message_block->rd_ptr (strlen (cp) + 1);

     /*
       Get the Unit_Of_Work pointer out of our specialized
       Message_Block.  Since the methods of interest are virtual, we
       don't have to know what kind of work we're to do.
     */
    unit_of_work = message_block->data();

    /*
       Invoke a couple of method calls on the object we constructed. 
     */
    unit_of_work->who_am_i ();
    unit_of_work->what_am_i ();

    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Block 0x%x contains (%s)\n", (void *) message, cp));

     /*
       Pretend that the work takes a little time to process.  This
       prevents one thread from getting all of the action.  In a real 
       system you wouldn't need to do this since the work really
       would take time to complete.
     */
    ACE_OS::sleep (ACE_Time_Value (0, 5000));

     /*
       Release the message block and allow the unit of work to also go 
       away.
     */
    message->release ();
  }

  return (0);
}
