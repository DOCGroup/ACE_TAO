
// $Id$

#include "Protocol_Task.h"

Protocol_Task::Protocol_Task( int _thr_count )
        : desired_thr_count_(_thr_count)
{
}

Protocol_Task::~Protocol_Task(void)
{
}

int Protocol_Task::open(void *arg) 
{
  ACE_UNUSED_ARG(arg);

  if( desired_thr_count_ )
  {
      return this->activate(THR_NEW_LWP, desired_thr_count_);
  }

  return(0);
}

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

int Protocol_Task::put(ACE_Message_Block *message,ACE_Time_Value *timeout)
{
    if( is_active() )
    {
        return this->putq(message,timeout);
    }

    return this->process(message,timeout);
}

int Protocol_Task::svc(void)
{

    ACE_Message_Block * message;
    
    while (1) {

        if ( this->getq(message, 0) == -1) {
            ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Protocol_Task::svc() getq"), -1);
        }

        ACE_DEBUG ((LM_DEBUG, "(%P|%t) Protocol_Task::svc() got message\n"));
        
        if (message->msg_type() == ACE_Message_Block::MB_HANGUP) {
 
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) Protocol_Task::svc() -- HANGUP block received\n"));

            if (this->putq(message->duplicate()) == -1) {
                ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "Protocol_Task::svc() putq"), -1);
            }
            
            break;
        }

        if( this->process(message->duplicate(),0) == -1 )
        {
            break;
        }
        
        message->release();
    }

    message->release();
    
    return(0);
}

int Protocol_Task::process(ACE_Message_Block * message, ACE_Time_Value *timeout)
{
    if( this->is_writer() )
    {
        return this->send(message,timeout);
    }
    
    return this->recv(message,timeout);
}

