// $Id$

#include "ace/PIP_Active_IO_Handler.h"


#include <iostream>
/// Constructor
ACE_PIP_Active_IO_Handler::ACE_PIP_Active_IO_Handler()
  : shutdown_(false)
{
   // acquire the shutdown lock so that when shutdown_svc is called,
   // the caller cannot return until shutdown has been completed and 
   // lock relinquished
   shutdown_lock_.acquire();
}

/// Closes all remote connections.
int ACE_PIP_Active_IO_Handler::handle_close (ACE_HANDLE handle, ACE_Reactor_Mask close_mask)
{
   int result(0);
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
      delete this;
      return -1;
   }
   
   return 0; 
}


/// Enqueue a message to be sent
int ACE_PIP_Active_IO_Handler::put_message (ACE_PIP_Protocol_Message* message)
{
   outgoing_message_queue_.enqueue(message);
}

int ACE_PIP_Active_IO_Handler::svc()
{
   int result(0);
   ssize_t bytes_available(0);
   char byte;

   // run until we're told to quit
   while (!shutdown_)
   {
      // peek to see if incoming message available
      bytes_available = peer_.recv(&byte, 1, MSG_PEEK);
      if (bytes_available > 0)
      {
         handle_input();
      }

      // handle outgoing message
      result = handle_output();
      if (result == -2)
      {
         // indicate to caller that the
         // handler is no longer active
         return -1;
      }

      bytes_available = 0;
   }
   

   return 0;
}

void ACE_PIP_Active_IO_Handler::shutdown_svc()
{
  shutdown_ = true;
  shutdown_lock_.acquire();

  handle_close(0, ACE_Event_Handler::READ_MASK | ACE_Event_Handler::WRITE_MASK);
  
}

int ACE_PIP_Active_IO_Handler::open(void*)
{
  std::cout << "activate" << std::endl;
   this->activate();
}




