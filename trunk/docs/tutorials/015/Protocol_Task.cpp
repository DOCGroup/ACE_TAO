
// $Id$

#include "Protocol_Task.h"

// Construct the object and remember the thread count.
Protocol_Task::Protocol_Task( int _thr_count )
        : desired_thr_count_(_thr_count)
{
}

Protocol_Task::~Protocol_Task(void)
{
}

// Activate the object if necessary.
int Protocol_Task::open(void *arg) 
{
  ACE_UNUSED_ARG(arg);

  if( desired_thr_count_ )
  {
      return this->activate(THR_NEW_LWP, desired_thr_count_);
  }

  return(0);
}

/* When we're being closed by the ACE_Stream and we've got threads to
   worry about then we drop a hangup message onto the message queue so 
   that svc() will go away.  Except for the call to is_active(), this
   is lifted directly from Tutorial 14.
*/
int Protocol_Task::close(u_long flags)
{
    if (flags == 1 && is_active() )
    {
        ACE_Message_Block *hangupBlock = new ACE_Message_Block();

        hangupBlock->msg_type(ACE_Message_Block::MB_HANGUP);
    
        if (this->putq(hangupBlock->duplicate()) == -1) {
            ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Task::close() putq"), -1);
        }
    
        hangupBlock->release();

        return this->wait();
    }

    return 0;
}

/* The put() method has to make a decision.  If we've got threads then 
   put the unit of work onto the message queue for svc() to deal
   with.  If not then process() it directly.
*/
int Protocol_Task::put(ACE_Message_Block *message,ACE_Time_Value *timeout)
{
    if( is_active() )
    {
        return this->putq(message,timeout);
    }

    return this->process(message,timeout);
}

/* svc() is about what you would expect.  This is again lifted
   directly from Tutorial 14 but with a call to process() for handling 
   the logic instead of doing the work right here.
 */
int Protocol_Task::svc(void)
{
    ACE_Message_Block * message;
    
    while (1)
    {
            // Get a message
        if ( this->getq(message, 0) == -1) {
            ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Protocol_Task::svc() getq"), -1);
        }

        ACE_DEBUG ((LM_DEBUG, "(%P|%t) Protocol_Task::svc() got message\n"));

            // Check for hangup
        if (message->msg_type() == ACE_Message_Block::MB_HANGUP) {
 
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) Protocol_Task::svc() -- HANGUP block received\n"));

                // Hangup our thread-pool peers (if any)
            if (this->putq(message->duplicate()) == -1) {
                ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Protocol_Task::svc() putq"), -1);
            }

                // Leave svc()
            break;
        }

            // Do some work on the data.
        if( this->process(message->duplicate(),0) == -1 )
        {
            break;
        }

            // Give up the message block before we go get another.
        message->release();
    }

        // Give up the message block that caused us to exit the
        // while(1) loop.
    message->release();
    
    return(0);
}

/* There's nothing really magic about process().  We just decide if
   we're moving data upstream or downstream and invoke the appropriate 
   virtual function to handle it.
*/
int Protocol_Task::process(ACE_Message_Block * message, ACE_Time_Value *timeout)
{
    if( this->is_writer() )
    {
        return this->send(message,timeout);
    }
    
    return this->recv(message,timeout);
}

/* We must insist that derivatives provide a meaningful overload for
   these methods.  It's fairly common for ACE object methods to return 
   an error when an overload is expected but the method cannot be
   safely made pure virtual.
 */
 
int Protocol_Task::send(ACE_Message_Block *message,
                        ACE_Time_Value *timeout)
{
    return -1;
}

int Protocol_Task::recv(ACE_Message_Block * message,
                        ACE_Time_Value *timeout)
{
    return -1;
}
