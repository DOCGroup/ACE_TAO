
// $Id$

// Task.cxx
//
// Tutorial regarding a way to use ACE_Stream.
//
// written by bob mcwhirter (bob@netwrench.com)
//
//

#include <ace/Message_Block.h>

#include "Task.h"

Task::Task(const char * nameOfTask,
	   int numberOfThreads)
  : d_numberOfThreads(numberOfThreads),
    d_barrier(numberOfThreads)
{
  // Just initialize our name, number of threads, and barrier.

  ACE_OS::strcpy(d_nameOfTask, nameOfTask);
}

Task::~Task(void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s Task::~Task() -- once per Task\n", d_nameOfTask));
}

int Task::open(void *arg) 
{
  ACE_UNUSED_ARG(arg);

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s Task::open() -- once per Task\n", d_nameOfTask));
  
  // call ACE_Task::activate() to spawn the threads using
  // our Task::svc() as the function to be run.

  // FMM -- Frequently Made Mistake --
  //  
  // If you specify the flag THR_DETACHED when activating the
  // Task, you will get an assert() violation during close(),
  // since the Task waits for all of its threads to rejoin.
  // 

  return this->activate(THR_NEW_LWP, d_numberOfThreads);
}

int Task::put(ACE_Message_Block *message,
	      ACE_Time_Value *timeout)
{
  // ACE_Stream uses the put() method of Tasks to send messages.
  // This defaultly does nothing.  Here we link our put() method
  // directly to our putq() method, so that Messages put() to us
  // will appear in the Message_Queue that is checked by the
  // service threads.
  
  return this->putq(message, timeout);
}

int Task::close(u_long flags)
{

  // When the Stream closes the Module, the Module then close()'s the Task
  // and passing a value of (1) as the flag.

  // When a service thread exits, it calls close() with a value that is not
  // (1).

  // We use this fact to tell the difference between closing a service thread,
  // and closing the main Task itself.

  if (flags == 1) {

    // The Module has asked to close the main Task.

    ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s Task::close() -- flags == 1 -- once per Task\n", d_nameOfTask));

    // We create a Message_Block...

    ACE_Message_Block *hangupBlock = new ACE_Message_Block();

    // And make it of the type MB_HANGUP.  

    hangupBlock->msg_type(ACE_Message_Block::MB_HANGUP);

    // We then send this Block into the Message_Queue to be seen by the 
    // service threads.

    // Once again we duplicate() the Block as send it off...
    
    if (this->putq(hangupBlock->duplicate()) == -1) {
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Task::close() putq"), -1);
    }
    
    // ..and we're free to release() our copy of it.

    hangupBlock->release();

    // Now, all we have to do is wait() for the service threads to all 
    // exit.  This is where using THR_DETACHED in the activate() method
    // will come back to haunt you.

    // The Stream waits until this returns before attempting to remove
    // the next Module/Task group in the Stream.  This allows for an
    // orderly shutting down of the Stream.

    return this->wait();


  } else {

    ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s Task::close() -- flags != 1 -- once per servicing thread\n", d_nameOfTask));

    // This is where we can clean up any mess left over by each service thread.
    // In this Task, there is nothing to do.

  }

  return 0;

}

int Task::svc(void)
{

  // This is the function that our service threads run once they are spawned.

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s Task::svc() -- once per servicing thread\n", d_nameOfTask));

  // First, we wait until all of our peer service threads have arrived
  // at this point also.

  d_barrier.wait();

  ACE_Message_Block *messageBlock;

  while (1) {

    // And now we loop almost infinitely.

    // getq() will block until a Message_Block is available to be read,
    // or an error occurs.

    if ( this->getq(messageBlock, 0) == -1) {
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Task::svc() getq"), -1);
    }

    if (messageBlock->msg_type() == ACE_Message_Block::MB_HANGUP) {
      
      // If the Message_Block is of type MB_HANGUP, then we're being asked
      // to shut down nicely.

      ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s Task::svc() -- HANGUP block received\n", d_nameOfTask));

      // So, we duplicate the Block, and put it back into the Message_Queue,
      // in case there are some more peer service threads still running.

      if (this->putq(messageBlock->duplicate()) == -1) {
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Task::svc() putq"), -1);
      }

      // We release our copy of the Block.
      messageBlock->release();

      // And we break out of the nearly infinitely loop, and
      // head towards close() ourselves.
      break;
    }

    // If we're here, then we've received a Message_Block that was 
    // not informing us to quit, so we're assuming it's a valid
    // meaningful Block.

    ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s Task::svc() -- Normal block received\n", d_nameOfTask));

    // We grab the read-pointer from the Block, and display it through a DEBUG statement.

    ACE_DEBUG ((LM_DEBUG, "(%P|%t) %s Task::svc() -- %s\n", d_nameOfTask, messageBlock->rd_ptr() ));

    // We pretend that this takes to time to process the Block.
    // If you're on a fast machine, you might have to raise this
    // value to actually witness different threads handling
    // blocks for each Task.

    ACE_OS::sleep (ACE_Time_Value (0, 250));

    // Since we're part of a Stream, we duplicate the Block, and 
    // send it on to the next Task.

    if (put_next(messageBlock->duplicate()) == -1) {
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Task::svc() put_next"), -1);
    }
    
    // And then we release our copy of it.

    messageBlock->release();

  }

  return 0;

}


const char * Task::nameOfTask(void) const
{
  return d_nameOfTask;
}
