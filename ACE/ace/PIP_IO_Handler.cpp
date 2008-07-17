// $Id$

#include "ace/Guard_T.h"
#include "ace/PIP_IO_Handler.h"
#include "ace/PIP_Invocation_Manager.h"
#include "ace/PIP_Dispatcher.h"

/// Constructor
ACE_PIP_IO_Handler::ACE_PIP_IO_Handler()
   : priority_set_(false)
   , destination_site_id_(0)
   , site_id_(0)
   , handler_id_(0)
   , millisecond_(0, 1000)
{
  // Temporarily assign the priority to be highest possible.
  // The first message received by the handler will be the priority
  this->priority(ACE_Event_Handler::HI_PRIORITY);
}

/// Destructor
ACE_PIP_IO_Handler::~ACE_PIP_IO_Handler( )
{
  // Tell the Invocation Manager to stop sending us messages
  ACE_PIP_Invocation_Manager::instance()->unregister_IO_handler(this);

  // Delete all outgoing messages
  ACE_PIP_Protocol_Message* message(0);
  while (!outgoing_message_queue_.is_empty())
  {
     outgoing_message_queue_.dequeue_tail(message);
     delete message;
  }
}

void ACE_PIP_IO_Handler::site_id(ACE_UINT32 site_id)
{
  site_id_ = site_id;
}

ACE_UINT32 ACE_PIP_IO_Handler::site_id() const
{
  return site_id_;
}

ACE_UINT32 ACE_PIP_IO_Handler::destination_site_id() const
{
   return destination_site_id_;
}

void ACE_PIP_IO_Handler::handler_id(ACE_UINT32 handler_id)
{
   handler_id_ = handler_id;
}

ACE_UINT32 ACE_PIP_IO_Handler::handler_id() const
{
  return handler_id_;
}

/// Initialize the priority of the handler, and inform the other end
/// of the priority
void ACE_PIP_IO_Handler::init(ACE_UINT32 site_id,
                              ACE_UINT32 destination_site_id,
                              ACE_UINT32 priority)
{
  this->priority(priority);
  site_id_ = site_id;
  destination_site_id_ = destination_site_id;

  // Inform other end of this connections priority
  peer_.send(&priority, sizeof(priority));

  // Inform other end of this end's site id
  peer_.send(&site_id,  sizeof(site_id));
  priority_set_ = true;

  // Register to receive outgoing messages
  ACE_PIP_Invocation_Manager::instance()->register_IO_handler(this);
}

void ACE_PIP_IO_Handler::extract_priority()
{
  ACE_UINT32 priority(0);
  if (peer_.recv(&priority, sizeof(priority)) == sizeof(priority))
  {
     this->priority(priority);
  }
  else
  {
     this->priority(ACE_Event_Handler::LO_PRIORITY);
  }

  // Receive the other end's site id
  if (peer_.recv(&destination_site_id_, sizeof(destination_site_id_)) != sizeof(destination_site_id_))
  {
     destination_site_id_ = 0;
  }
  
  priority_set_ = true;
}

/// Handles read event on socket.
int ACE_PIP_IO_Handler::handle_input (ACE_HANDLE fd)
{
   int result(0);
   int bytes_read(0);
   
   if (!priority_set_)
   {
      // incoming message is the priority of this connection
     extract_priority();
     ACE_PIP_Invocation_Manager::instance()->register_IO_handler(this);
   }
   else
   {
      // Read the next incoming message
      ACE_PIP_Protocol_Message* message = new ACE_PIP_Protocol_Message;
      bytes_read = message->deserialize(peer_);
      if (bytes_read > 0)
      {
         if (message->get_message_type() == ACE_PIP_Protocol_Message::ACCEL)
         {
            std::cout << "Accel Message Received" << std::endl;
         }

        ACE_PIP_Dispatcher::instance()->process_message(message);
      }     
      else if (bytes_read < 0)
      {
        // The connection is broken, so handler should be deleted
         delete message;
         result = -1;
      }
   }
   
   return result;
}


/// Handles output event on socket
int ACE_PIP_IO_Handler::handle_output (ACE_HANDLE fd)
{
   int bytes_sent(0);
  // determine if outgoing messages exist
  ACE_PIP_Protocol_Message* message(0);

  write_closed_ = false;
  big_lock_.acquire();
  if (outgoing_message_queue_.dequeue_tail(message) != -1)
  {
     if (message->get_message_type() == ACE_PIP_Protocol_Message::ACCEL)
     {
        std::cout << "Sending accel message" << std::endl;
     }
    bytes_sent = message->serialize(peer_);
     delete message;
     if (bytes_sent >= 0)
     {
       big_lock_.release();
       return 0;
     }
     else
     {
        write_closed_ = true;
        big_lock_.release();
        // indicate the outgoing connection is closed
        return -2;
     }
  }
  else
  {
     // indicate that there was no message to output
    
    big_lock_.release();
    return -1;
  }
}

ACE_INET_Addr ACE_PIP_IO_Handler::get_remote_address() const
{
  ACE_INET_Addr addr;
  peer_.get_remote_addr(addr);
  return addr;
}
