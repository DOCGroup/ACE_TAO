
#include "ace/PIP_Invocation_Manager.h"
#include "ace/PIP_IO_Handler.h"
#include <ace/PIP_Message_Handler.h>

#include <iostream>
ACE_PIP_Invocation_Manager* ACE_PIP_Invocation_Manager::invocation_manager_ = 0;
ACE_Mutex                   ACE_PIP_Invocation_Manager::instance_lock_;
bool                        ACE_PIP_Invocation_Manager::delete_manager_ = false;
ACE_UINT64                  ACE_PIP_Invocation_Manager::message_id_base_ = 0;
ACE_UINT32                  ACE_PIP_Invocation_Manager::site_id_ = 0;

/// Constructor
ACE_PIP_Invocation_Manager::ACE_PIP_Invocation_Manager()
   : message_counter_(0)
{
}

/// Destructor
ACE_PIP_Invocation_Manager::~ACE_PIP_Invocation_Manager()
{

}

/// Processes requests received from I/O handler
void ACE_PIP_Invocation_Manager::process_inbound_request(ACE_PIP_Protocol_Message* message)
{
  ACE_PIP_Data_Message* payload = 
    static_cast<ACE_PIP_Data_Message*>(message->release_next());
  
  ACE_UINT32 handler_id = payload->destination_handler_ID();  
  ACE_PIP_Message_Handler* handler(0);
  
  big_lock_.acquire();
  ACE_UINT64 message_id = message->message_id();
  if (object_id_handler_map_.find(handler_id, handler) == 0)
  {
     // look to see if there are any accelerations. If so, accelerate.
     // Map the message ID to a list of outgoing messages
     in_out_id_map_.bind(message_id,
                         std::list<ACE_UINT64>());

     // Keep a record of the message and its priority so
     // it can be accelerated if necessary
     Invocation_Data invocation_data;
     invocation_data.site_id = site_id_;
     invocation_data.priority = payload->message_priority();
     invocation_data_map_.bind(message_id, invocation_data);

     big_lock_.release();
     
     // Pass the message to the message handler, deleting
     // the corresponding struct
     handler->process_incoming_message(payload->release_block(), message_id);
     delete payload;
     delete message;
     
     // Once message processing has completed,
     // clean-up any message residue
     big_lock_.acquire();
     in_out_id_map_.unbind(message_id);   
  }
  
  big_lock_.release();
}

/// Processes request to be forwarded to another handler
int ACE_PIP_Invocation_Manager::process_outbound_request(ACE_Message_Block* message, 
                                                          ACE_UINT64 token,
                                                          ACE_Future<ACE_Message_Block*>*& response_holder)
{
  // Create a protocol message from the data block
   ACE_PIP_Protocol_Message* protocol_message(0);

   ACE_NEW_RETURN(protocol_message, ACE_PIP_Protocol_Message, -1);
   protocol_message->message_type(ACE_PIP_Protocol_Message::REQUEST);
   protocol_message->process_message_payload(message);
      
   ACE_PIP_Data_Message* data_message =
     static_cast<ACE_PIP_Data_Message*>(protocol_message->next());

   // determine if the message expects a reply. If so, create a future for it.
   if (data_message->reply_expected())
     {
       // create and store future
        ACE_NEW_RETURN(response_holder, ACE_Future<ACE_Message_Block*>, -1);
     }
   
   Invocation_Data remote_info;
   remote_info.response_holder = response_holder;
   remote_info.site_id = data_message->destination_site_ID();
   remote_info.priority = data_message->message_priority();
   
   // Associate this message with token. This enables acceleration forwarding. Only
   // Token == -1 indicates this is the start of a call chain
   
   big_lock_.acquire();
   ACE_UINT64 message_id = generate_message_id();
   invocation_data_map_.bind(message_id, remote_info);
   protocol_message->message_id(message_id);

   if (token != 0)
     {
       ACE_Hash_Map_Entry<ACE_UINT64, std::list<ACE_UINT64> >* entry(0);
       // map the originating message to the outgoing message
       // so that accelerations can be forwarded appropriately
       in_out_id_map_.find(token, entry);
       if (entry)
         {
           entry->item().push_back(message_id);
         }
     }

   // use the priority and address to determine which I/O handler to send to 
   // pass the message to the I/O handler
   PRIORITY_TO_IO_HANDLER_MAP* handler_map(0);
   site_to_handlers_map_.find(data_message->destination_site_ID(),
                              handler_map);
   
   ACE_PIP_IO_Handler* IO_handler(0);
   handler_map->find(data_message->message_priority(),
                     IO_handler);

   big_lock_.release();
   
   if (IO_handler)
   {
      IO_handler->put_message(protocol_message);
   }

   return 0;
}

ACE_PIP_Invocation_Manager* ACE_PIP_Invocation_Manager::instance()
{
  if (ACE_PIP_Invocation_Manager::invocation_manager_ == 0)
    {
      instance_lock_.acquire();
      
      if (ACE_PIP_Invocation_Manager::invocation_manager_ == 0)
        {
          ACE_NEW_RETURN (ACE_PIP_Invocation_Manager::invocation_manager_,
                          ACE_PIP_Invocation_Manager,
                          0);

          delete_manager_ = true;
        }

      instance_lock_.release();
    }
  
  return invocation_manager_;
}

/// Process response received from a handler
void ACE_PIP_Invocation_Manager::process_inbound_response(ACE_PIP_Protocol_Message* message)
{
   Invocation_Data remote_info;
  
   ACE_Guard<ACE_Mutex> guard(big_lock_);
   
   // Remove the child ID
   if (invocation_data_map_.unbind(message->message_id(),
                                   remote_info) != -1)
   {
      // Pass the received response to the message handler
      // via a Future
      remote_info.response_holder->set(message->next()->block());
      remote_info.response_holder = 0;
   }
}

/// Process response received from a handler
void ACE_PIP_Invocation_Manager::process_outbound_response(ACE_Message_Block* message, ACE_UINT64 token)
{
  // Parse the message
  ACE_PIP_Protocol_Message* response = new ACE_PIP_Protocol_Message;
  response->process_message_payload(message);
  response->message_type(ACE_PIP_Protocol_Message::RESPONSE);
  
  // Lookup the appropriate IO handler, and pass down the message
  ACE_PIP_Data_Message* data_message =
    static_cast<ACE_PIP_Data_Message*>(response->next());
  
  ACE_Guard<ACE_Mutex> guard(big_lock_);

  response->message_id(token);
  PRIORITY_TO_IO_HANDLER_MAP* handler_map(0);
  
  if (site_to_handlers_map_.find
      (data_message->destination_site_ID(),
       handler_map) != -1)
    {
      ACE_PIP_IO_Handler* handler(0);
      if (handler_map->find(data_message->message_priority(),
                            handler) != -1)
        {
          handler->put_message(response);
        }
    }
}

/// Process request to accelerate the priority of a process
void ACE_PIP_Invocation_Manager::process_acceleration(ACE_PIP_Protocol_Message* message)
{
   ACE_PIP_Accel_Message* accel_message =
      static_cast<ACE_PIP_Accel_Message*>(message->next());
 
   ACE_Guard<ACE_Mutex> guard(big_lock_);
  
   // Update the stored priority of the original message. This will enable subsequent upcalls
   // to adjust their priority appropriately
   Invocation_Data invocation_data;
   if (invocation_data_map_.unbind(message->message_id(), invocation_data) == 0)
   {
      if (invocation_data.priority < accel_message->new_priority())
      {
         invocation_data.priority = accel_message->new_priority();
      }
      
      invocation_data_map_.bind(message->message_id(), invocation_data);

      // Generate acceleration messages for each outgoing invocation
      // resulting from processing of incoming request. Do so
      // only if their priority is lower than the accelerated priority
      ACE_Hash_Map_Entry<ACE_UINT64, std::list<ACE_UINT64> >* child_entry(0);
      if (in_out_id_map_.find(message->message_id(), child_entry) == 0)
      {
         std::list<ACE_UINT64>::iterator child_iter = child_entry->item().begin();
         for (; child_iter != child_entry->item().end(); ++child_iter)
         {
            if (invocation_data_map_.unbind(*child_iter, invocation_data) == 0)
            {
               if (invocation_data.priority < accel_message->new_priority())
               {
                  invocation_data.priority = accel_message->new_priority();
                  invocation_data_map_.bind(*child_iter, invocation_data);
                  // Generate new message and send it to the appropriate site
                  PRIORITY_TO_IO_HANDLER_MAP* handler_map(0);
                  if (site_to_handlers_map_.find(invocation_data.site_id,
                                                 handler_map) == 0)
                  {
                     ACE_PIP_IO_Handler* handler(0);
                     if (handler_map->find(ACE_Event_Handler::HI_PRIORITY, handler) == 0)
                     {
                        ACE_PIP_Accel_Message* accel_copy = accel_message->copy();
                        ACE_PIP_Protocol_Message* proto_copy = message->copy();
                        proto_copy->next(accel_copy);
                        handler->put_message(proto_copy);
                     }
                  }
               }

               invocation_data_map_.bind(*child_iter, invocation_data);
            }
         }
      }
   }

   // delete the acceleration message here
}

/// Register an IO handler that can send messages on invocation
/// manager's behalf
int ACE_PIP_Invocation_Manager::register_IO_handler(ACE_PIP_IO_Handler* handler)
{
   // Extract the priority and remote address of handler
   ACE_UINT32 priority = handler->priority();
   ACE_Guard<ACE_Mutex> guard(big_lock_);
   
   // Map the destination site ID and priority to this handler
   PRIORITY_TO_IO_HANDLER_MAP* handler_map(0);
   if (site_to_handlers_map_.find(handler->destination_site_id(),
                                  handler_map) == -1)
   {
      ACE_NEW_RETURN(handler_map, PRIORITY_TO_IO_HANDLER_MAP, -1);
   }
  
   handler_map->bind(priority, handler);
   site_to_handlers_map_.bind(handler->destination_site_id(), handler_map);

   return 0;
}

void ACE_PIP_Invocation_Manager::unregister_IO_handler(ACE_PIP_IO_Handler* handler)
{
   ACE_UINT32 priority = handler->priority();
   
   ACE_Guard<ACE_Mutex> guard(big_lock_);
   
   // unbind the handler
   PRIORITY_TO_IO_HANDLER_MAP* handler_map(0);
   if (site_to_handlers_map_.find(handler->destination_site_id(),
                                  handler_map) != -1)
   {
      handler_map->unbind(priority, handler);
   }   
}

void ACE_PIP_Invocation_Manager::register_message_handler(ACE_PIP_Message_Handler* handler)
{
  // extract the object id from the handler
  // map the object id to the handler
  ACE_Guard<ACE_Mutex> guard(big_lock_);
  object_id_handler_map_.bind(handler->handler_id(), handler);
}

ACE_UINT64 ACE_PIP_Invocation_Manager::generate_message_id()
{
  return (((ACE_UINT64)site_id_) << 32) + message_counter_++;
}

void ACE_PIP_Invocation_Manager::site_id(ACE_UINT64 site_id)
{
  site_id_ = site_id;
}
