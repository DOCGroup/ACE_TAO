
// $Id$

#include "task.h"
#include "block.h"
#include "work.h"

/*
   Construct the Task with zero or more subtasks.  If subtasks are requested,
   we assign our next_ pointer to the first of those and let it worry about
   any remaining subtasks. 
 */
Task::Task (int sub_tasks)
        : barrier_ (0)
         ,next_ (0)
{
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Task ctor 0x%x\n", (void *) this));
    if (sub_tasks)
    {
        next_ = new Task (--sub_tasks);
    }
}

/*
   Delete our barrier object and any subtasks we may have. 
 */
Task::~Task (void)
{
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Task dtor 0x%x\n", (void *) this));

    delete barrier_;
    delete next_;
}

/*
   Open our thread pool with the requested number of threads.  If subtasks are
   enabled, they inherit the thread-pool size.  Make sure that the subtasks can 
   be opened before we open our own threadpool. 
 */
int Task::start (int threads)
{
    if (next_)
    {
        if (next_->start (threads) == -1)
        {
            return -1;
        }
    }

    barrier_ = new ACE_Barrier (threads);
    return this->activate (THR_NEW_LWP, threads);
}

/*
   Close ourselves and any subtasks.  This just prints a message so that we can 
   assure ourselves things are cleaned up correctly. 
 */
int Task::close (u_long flags)
{
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Task close 0x%x\n", (void *) this));
    if (next_)
    {
        next_->close (flags);
    }

    return (0);
}

/*
   Wait for all of the threads in our pool to exit and then wait for any
   subtasks.  When called from the front of the task chain, this won't return
   until all thread pools in the chain have exited. 
 */
int Task::wait (void)
{
    inherited::wait ();
    if (next_)
    {
        next_->wait ();
    }
    return (0);
}

/*
   Like the thread-pools before, this is where all of the work is done. 
 */
int Task::svc (void)
{
        // Wait for all threads to get this far before continuing.
    this->barrier_->wait ();

    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Task 0x%x starts in thread %u\n", (void *) this, ACE_Thread::self ()));

        // getq() wants an ACE_Message_Block so we'll start out with one
        // of those.  We could do some casting (or even auto-casting) to
        // avoid the extra variable but I prefer to be clear about our actions.
    ACE_Message_Block *message;
  
        // What we really put into the queue was our Message_Block.
        // After we get the message from the queue, we'll cast it to this 
        // so that we know how to work on it.
    Message_Block *message_block;
  
        // And, of course, our Message_Block contains our Data_Block
        // instead of the typical ACE_Data_Block
    Data_Block *data_block;
  
      // Even though we put Work objects into the queue, we take them
      // out using the baseclass pointer.  This allows us to create new 
      // derivatives without having to change this svc() method.
    Unit_Of_Work *work;

    while (1)
    {
            // Get the ACE_Message_Block
        if (this->getq (message) == -1)
        {
            ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "getq"), -1);
        }

            // "Convert" it to our Message_Block
        message_block = (Message_Block *) message;

            // Get the ACE_Data_Block and "convert" to Data_Block in one step.
        data_block = (Data_Block *) (message_block->data_block ());

            // Get the unit of work from the data block
        work = data_block->data ();

            // Show the object's instance value and "type name"
        work->who_am_i ();
        work->what_am_i ();

            // If there is a hangup we need to tell our pool-peers as
            // well as any subtasks.
        if (message_block->msg_type () == ACE_Message_Block::MB_HANGUP)
        {
                // duplicate()ing the message block will increment the
                // reference counts on the data blocks.  This allows us
                // to safely release() the message block.  The rule of
                // thumb is that if you pass a message block to a new
                // owner, duplicate() it.  Then you can release() when
                // you're done and not worry about memory leaks.
            if (this->putq (message_block->duplicate ()) == -1)
            {
                ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "putq"), -1);
            }

                // If we have a subtask, duplicate() the message block
                // again and pass it to that task's queue
            if (next_ && next_->putq (message_block->duplicate ()) == -1)
            {
                ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "putq"), -1);
            }

                // We're now done with our copy of the block, so we can
                // release it.  Our peers/subtasks have their own message 
                // block to access the shared data blocks.
            message_block->release ();

            break;
        }

            // If this isn't a hangup/shutdown message then we tell the
            // unit of work to process() for a while.
        work->process ();

        if (next_)
        {
                // If we have subtasks, we pass the block on to them.  Notice
                // that I don't bother to duplicate() the block since I won't 
                // release it in this case.  I could have invoked
                // duplicate() in the puq() and then release()
                // afterwards.  Either is acceptable.
            if (next_->putq (message_block) == -1)
                ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "putq"), -1);
        }
        else
        {
                // If we don't have subtasks then invoke fini() to tell
                // the unit of work that we won't be invoking process()
                // any more.  Then release() the block.  This release()
                // would not change if we duplicate()ed in the above conditional
            work->fini ();
            message_block->release ();
        }

            // Pretend that the work takes some time...
        ACE_OS::sleep (ACE_Time_Value (0, 250));
    }

    return (0);
}
