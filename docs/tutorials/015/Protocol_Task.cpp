
// $Id$

#include "Protocol_Task.h"

// Construct the object and remember the thread count.
Protocol_Task::Protocol_Task(void)
{
    ;
}

Protocol_Task::~Protocol_Task(void)
{
    ;
}

int Protocol_Task::open(void *arg) 
{
  ACE_UNUSED_ARG(arg);

  return(0);
}

int Protocol_Task::close(u_long flags)
{
    return 0;
}

/* When a message is put() onto the task, it's time to process() some data.
*/
int Protocol_Task::put(ACE_Message_Block *message,ACE_Time_Value *timeout)
{
    return this->process(message,timeout);
}

/* Return an error since we don't want the task to ever be activated.
 */
int Protocol_Task::svc(void)
{
    return -1;
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
