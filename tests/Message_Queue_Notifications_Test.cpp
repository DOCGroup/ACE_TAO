// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
// 
// = FILENAME
//    Message_Queue_Notification_Test.cpp
//
// = DESCRIPTION
//      This is a test to illustrate the notification mechanisms in
//      Message_Queue and its integration with Reactor.
//
//      Note the following things about this example:
//
//      1. Multiple threads are not required.
//      2. You do not have to explicitly notify the Reactor
//      3. This code will work the same with Reactor and ReactorEx
//      4. handle_input, handle_exception, handle_output are the only
//         callbacks supported by this mechanism
//      5. The notification mechanism need not notify the Reactor. You can
//         write your own strategy classes that can do whatever application
//         specific behavior you want.
//
// = AUTHOR
//    Irfan Pyarali
// 
// ============================================================================

#include "ace/Reactor.h"
#include "ace/Strategies.h"
#include "ace/Task.h"
#include "test_config.h"

static int iterations = 10;

class Message_Handler : public ACE_Task<ACE_NULL_SYNCH>
{
public:
  Message_Handler (ACE_Reactor &reactor);
  
  virtual int handle_input (ACE_HANDLE);
  virtual int handle_output (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  virtual int handle_exception (ACE_HANDLE fd = ACE_INVALID_HANDLE);

private:
  int process_message (void);
  void make_message (void);

  ACE_Reactor_Notification_Strategy notification_strategy_;  
};

Message_Handler::Message_Handler (ACE_Reactor &reactor)
  // First time handle_input will be called 
  : notification_strategy_ (&reactor,
			    this,
			    ACE_Event_Handler::READ_MASK)
{
  this->msg_queue ()->notification_strategy (&this->notification_strategy_);
  this->make_message ();
}

int
Message_Handler::handle_input (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG, "Message_Handler::handle_input\n"));

  // Next time handle_output will be called 
  this->notification_strategy_.mask (ACE_Event_Handler::WRITE_MASK);

  return process_message ();
}

int 
Message_Handler::handle_output (ACE_HANDLE fd)
{
  ACE_DEBUG ((LM_DEBUG, "Message_Handler::handle_output\n"));
  ACE_UNUSED_ARG (fd);  

  // Next time handle_exception will be called 
  this->notification_strategy_.mask (ACE_Event_Handler::EXCEPT_MASK);

  return process_message ();  
}

int 
Message_Handler::handle_exception (ACE_HANDLE fd)
{
  ACE_DEBUG ((LM_DEBUG, "Message_Handler::handle_exception\n"));
  ACE_UNUSED_ARG (fd);  

  // Next time handle_input will be called 
  this->notification_strategy_.mask (ACE_Event_Handler::READ_MASK);

  return process_message ();
}

int
Message_Handler::process_message (void)
{
  ACE_Message_Block *mb;

  if (this->getq (mb, (ACE_Time_Value *) &ACE_Time_Value::zero) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "dequeue_head"), -1);
  else
    {
      ACE_DEBUG ((LM_DEBUG, "message received = %s\n", mb->rd_ptr ()));
      delete mb;
    }

  this->make_message ();
  return 0;
}  

void
Message_Handler::make_message (void)
{
  if (--iterations > 0)
    {
      ACE_Message_Block *mb = new ACE_Message_Block ("hello");
      
      ACE_DEBUG ((LM_DEBUG, "sending message\n"));
      this->putq (mb);
    }
}
  

int
main (int, char *[])
{
  ACE_START_TEST ("Message_Queue_Notifications_Test");

#if defined (ACE_HAS_THREADS)
  ACE_Reactor reactor; 
  Message_Handler mh (reactor);

  while (iterations > 0)
    reactor.handle_events ();

#else
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
#endif /* ACE_HAS_THREADS */
  ACE_END_TEST;
  return 0;
}

