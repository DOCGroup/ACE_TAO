// $Id$

#include "ace/PIP_Active_IO_Handler.h"

/// Constructor
ACE_PIP_Active_IO_Handler::ACE_PIP_Active_IO_Handler()
   : shutdown_(false)
{
   // acquire the shutdown lock so that when shutdown_svc is called,
   // the caller cannot return until shutdown has been completed and 
   // lock relinquished
   this->shutdown_lock_.acquire();
}

/// Closes all remote connections.
int ACE_PIP_Active_IO_Handler::handle_close (ACE_HANDLE handle, 
                                             ACE_Reactor_Mask close_mask)
{
   ACE_UNUSED_ARG(handle);
   switch(close_mask)
   {
      case ACE_Event_Handler::READ_MASK:
	 this->read_closed_ = true;
	 break;
      case ACE_Event_Handler::WRITE_MASK:
	 this->write_closed_ = true;
	 break;
   };

   if (read_closed_ && write_closed_)
   {
      // Close our end of the connection
      this->peer_.close_reader();
      this->peer_.close_writer();
      delete this;
      return -1;
   }

   return 0;
}

/// Enqueue a message to be sent
int ACE_PIP_Active_IO_Handler::put_message (ACE_PIP_Protocol_Message* message)
{
   int result = this->outgoing_message_queue_.enqueue(message);
   if (result > -1)
   {
      return result;
   }

   return -1;
}

int ACE_PIP_Active_IO_Handler::svc()
{
   int     result(0);
   ssize_t bytes_available(0);
   char    byte;

   // Run until told to shutdown
   while (!this->shutdown_)
   {
      // Peek to see if an incoming message available
      bytes_available = this->peer_.recv(&byte, 1, MSG_PEEK);
      if (bytes_available > 0)
      {
         this->handle_input();
      }

      // Handle outgoing messages
      result = this->handle_output();
      if (result == -2)
      {
        // Indicate to caller that the
        // handler is no longer active
        return -1;
      }

      bytes_available = 0;
   }

   return 0;
}

void ACE_PIP_Active_IO_Handler::shutdown_svc()
{
   this->shutdown_ = true;
   this->shutdown_lock_.acquire();

   this->handle_close(0, 
                      ACE_Event_Handler::READ_MASK | 
                      ACE_Event_Handler::WRITE_MASK);

}

int ACE_PIP_Active_IO_Handler::open(void*)
{
   int result = this->activate();
   if (result > 0)
   {
      return 0;
   }

   return -1;
}




