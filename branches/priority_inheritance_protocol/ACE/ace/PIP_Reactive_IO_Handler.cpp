// $Id$

#include "ace/OS_NS_sys_time.h"
#include "ace/PIP_Reactive_IO_Handler.h"
#include "ace/PIP_Invocation_Manager.h"

/// Constructor
ACE_PIP_Reactive_IO_Handler::ACE_PIP_Reactive_IO_Handler()
{
}

ACE_PIP_Reactive_IO_Handler::~ACE_PIP_Reactive_IO_Handler()
{
}

/// Closes all remote connections.
int ACE_PIP_Reactive_IO_Handler::handle_close (ACE_HANDLE handle, ACE_Reactor_Mask close_mask)
{
  ACE_UNUSED_ARG(handle);
  switch(close_mask)
   {
      case ACE_Event_Handler::READ_MASK:
	 read_closed_ = true;
	 break;
      case ACE_Event_Handler::WRITE_MASK:
	 write_closed_ = true;
	 break;
   };
   
   if (read_closed_ && write_closed_)
   {
      // Close our end of the connection
      peer_.close_reader();
      peer_.close_writer();

      // un-register with invocation manager so it doesn't 
      // try to use the handler for IO
      ACE_PIP_Invocation_Manager::instance()->unregister_IO_handler(this);

      delete this;
      return -1;
   }
   
   return 0; 
}


/// Enqueue a message to be sent
int ACE_PIP_Reactive_IO_Handler::put_message (ACE_PIP_Protocol_Message* message)
{
  big_lock_.acquire();
  outgoing_message_queue_.enqueue_head(message);
  big_lock_.release();

  // Register so Reactor tells us to send the message
  ACE_Reactor::instance()->register_handler(this, ACE_Event_Handler::WRITE_MASK);
  ACE_Reactor::instance()->register_handler(this, ACE_Event_Handler::READ_MASK);
  
  return 0;
}




