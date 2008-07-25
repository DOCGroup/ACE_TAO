#include "ace/PIP_Dispatcher.h"
#include "ace/PIP_Invocation_Manager.h"
#include "ace/PIP_Messages.h"
#include "ace/Reactor.h"

ACE_PIP_Dispatcher* ACE_PIP_Dispatcher::dispatcher_        = 0;
ACE_Mutex           ACE_PIP_Dispatcher::instance_lock_;
bool                ACE_PIP_Dispatcher::delete_dispatcher_ = false;
bool                ACE_PIP_Dispatcher::shutdown_          = false;

/// Constructor
ACE_PIP_Dispatcher::ACE_PIP_Dispatcher()
  : current_highest_priority_(ACE_Event_Handler::LO_PRIORITY)
  , current_lowest_priority_(ACE_Event_Handler::LO_PRIORITY)
  , waiting_for_message_(false)
  , num_threads_needed_(0)
  , message_available_signal_(0)
  , threads_available_signal_(0)
  , DA_strategy_adapter_(0)
  , num_pending_messages_(0)
  , num_messages_received_(0)
  , num_messages_dispatched_(0)
{
}

/// Destructor
ACE_PIP_Dispatcher::~ACE_PIP_Dispatcher()
{
   ACE_PIP_Protocol_Message* message(0);
   
   // Destroy all messages that have yet to be dispatched
   this->pending_messages_lock_.acquire();
   while (this->pending_messages_by_message_id_.current_size() != 0)
   {
      this->pending_messages_by_message_id_.unbind(
         this->pending_messages_by_message_id_.begin()->key(),
	 message);

      if (message)
      {
	 delete message;
	 message = 0;
      }
   }
   this->pending_messages_lock_.release();
}


ACE_PIP_Dispatcher* ACE_PIP_Dispatcher::instance()
{
  if (ACE_PIP_Dispatcher::dispatcher_ == 0)
    {
      instance_lock_.acquire();
      
      if (ACE_PIP_Dispatcher::dispatcher_ == 0)
        {
          ACE_NEW_RETURN (ACE_PIP_Dispatcher::dispatcher_,
                          ACE_PIP_Dispatcher,
                          0);

          delete_dispatcher_ = true;
        }

      instance_lock_.release();
    }

  return dispatcher_;
}

/// Receive a message for eventual dispatching
void ACE_PIP_Dispatcher::process_message(ACE_PIP_Protocol_Message* message)
{
  switch (message->message_type())
  {
     case ACE_PIP_Protocol_Message::ACCEL:
       process_incoming_acceleration(message);
     break;
     
     case ACE_PIP_Protocol_Message::REQUEST:
        process_incoming_request(message);
        break;
        
     case ACE_PIP_Protocol_Message::RESPONSE:
        // Forward the response to the invocation manager
        ACE_PIP_Invocation_Manager::instance()->process_inbound_response(message);
        break;
        
     default:
       break;
  }
}


/// Signals the dispatcher to dispatch a new message if possible.
int ACE_PIP_Dispatcher::handle_output (ACE_HANDLE)
{
   ACE_PIP_Protocol_Message* message(0);
   bool message_dispatched(false);
   
   while (!message_dispatched && !shutdown_)
   {
      // get the highest priority message
      this->pending_messages_lock_.acquire();
      message = retrieve_highest_priority_pending_message();
      if (message)
      {
         ACE_PIP_Data_Message* data_message =
            static_cast<ACE_PIP_Data_Message*>(message->next());
         
         this->deadlock_avoidance_lock_.acquire();
         
         /// If dispatching could potentially cause deadlock, try to accelerate all lower priority
         /// messages and then wait for threads to become available
         this->num_threads_needed_ = DA_strategy_adapter_->is_deadlock_potential(
            data_message->destination_handler_ID(),
            data_message->message_priority());
         
         if (this->num_threads_needed_ > 0)
         {
            this->deadlock_avoidance_lock_.release();
            find_and_accelerate_lower_priority_message(data_message->message_priority());

            // Wait for signal indicating enough threads exist to dispatch the message
            this->threads_available_signal_.acquire();

            // Before grabing the deadlock avoidance lock, check to make sure
            // we haven't been told to shutdown.
            if (shutdown_)
              break;

              this->deadlock_avoidance_lock_.acquire();
         }

	 // At this point, sufficient threads exist to dispatch the message 
         // without threat of deadlock, so grant a thread
	 DA_strategy_adapter_->grant(data_message->destination_handler_ID(),
                                     data_message->message_priority());

	 this->deadlock_avoidance_lock_.release();
	 
	 // Transfer the message to the "dispatched" list  
	 this->dispatched_messages_lock_.acquire();
         Dispatched_Message_Data dispatch_record;
         dispatch_record.id = message->message_id();
         dispatch_record.priority = data_message->message_priority();
         this->dispatched_messages_data_.insert(dispatch_record);
	 this->dispatched_messages_lock_.release();

         //-------------TEST DATA------------------
         // store statistics to be printed later
         Dispatch_Test_Data test_data;
         test_data.id = message->message_id();
         test_data.priority = data_message->message_priority();
         test_data.num_pending = this->num_pending_messages_;
         test_data.highest_priority = this->current_highest_priority_;
         test_data.lowest_priority = this->current_lowest_priority_;
         dispatch_records_.push_back(test_data);

         dispatched_ids_.push_back(message->message_id());

         ++num_messages_dispatched_;
         --this->num_pending_messages_;
	 this->pending_messages_lock_.release();
         //-----------------------------------------

	 // Request another thread to be associated with dispatcher
	 ACE_Reactor::instance()->notify(this, ACE_Event_Handler::WRITE_MASK);
	 
	 message_dispatched = true;

         // Pass the message to the invocation manager for processing
         ACE_PIP_Invocation_Manager::instance()->process_inbound_request(message);
	 
         // All processing associated with the message has been completed
         // so discard the record
	 this->dispatched_messages_lock_.acquire();
	 this->dispatched_messages_data_.erase(dispatch_record);
	 this->dispatched_messages_lock_.release();
         
	 // Cleanup message information and release the thread resource
         this->deadlock_avoidance_lock_.acquire();
         DA_strategy_adapter_->release(data_message->destination_handler_ID(),
                                       data_message->message_priority());
         
         if (this->num_threads_needed_ > 0)
	 {
            --this->num_threads_needed_;
            if (this->num_threads_needed_ == 0)
            {
               this->threads_available_signal_.release();
            }
	 }
         
	 this->deadlock_avoidance_lock_.release();
      }
      else
      {
	 // There are no messages to dispatch, so wait for one to arrive
	 this->waiting_for_message_ = true;
	 this->pending_messages_lock_.release();
	 this->message_available_signal_.acquire();

         // Before dispatching a message, make sure we haven't been
         // instructed to shutdown
         if (shutdown_)
           break;
      }
   }

  return 0;
}


/// Initializes dispatcher
void ACE_PIP_Dispatcher::init(ACE_PIP_DA_Strategy_Adapter<ACE_UINT32, ACE_Null_Mutex>* DA_strategy_adapter)
{
  DA_strategy_adapter_ = DA_strategy_adapter;
  this->waiting_for_message_ = true;
  ACE_Reactor::instance()->notify(this, ACE_Event_Handler::WRITE_MASK);
}

/// store the message
void ACE_PIP_Dispatcher::process_incoming_request(ACE_PIP_Protocol_Message* message)
{
   // Store the message token 2 ways to enable efficient dispatching as well as
   // efficient lookup for accelerations
   this->pending_messages_lock_.acquire();

   //-------TEST DATA------------------------
   ++num_messages_received_;
   ++this->num_pending_messages_;
   received_ids_.push_back(message->message_id());

   //------------------------------------------
   ACE_UINT32 priority =
      static_cast<ACE_PIP_Data_Message*>(message->next())->message_priority();

   // update the priority upper and lower bounds. These values are stored to
   // avoid checking the full range of priorities when dispatching messages
   if (priority > this->current_highest_priority_)
   {
      this->current_highest_priority_ = priority;
   }
   else if (priority < this->current_lowest_priority_)
   {
      this->current_lowest_priority_ = priority;
   }

   PRIORITY_MESSAGE_LIST_MAP::iterator 
      message_iter = this->pending_messages_by_priority_.find(priority);
   
   if (message_iter == this->pending_messages_by_priority_.end())
   {
      // Create a new entry for this priority level
      std::list<ACE_PIP_Protocol_Message*> new_priority_list;
      new_priority_list.push_back(message);
      this->pending_messages_by_priority_.insert(
         make_pair(priority, new_priority_list));         
   }
   else
   {
      // Priority already exists, so add the message token to the list
      message_iter->second.push_back(message);
   }
   
   this->pending_messages_by_message_id_.bind(message->message_id(), message);

   if (this->waiting_for_message_)
   {
      this->waiting_for_message_ = false;
      
      // Signal waiting dispatcher thread to dispatch new message
      this->message_available_signal_.release();
   }
   
   this->pending_messages_lock_.release();

}

/// Find the highest priority message and return it
ACE_PIP_Protocol_Message* ACE_PIP_Dispatcher::
   retrieve_highest_priority_pending_message()
{
   ACE_PIP_Protocol_Message* message(0);
   for (ACE_INT32 current_priority = (ACE_INT32)this->current_highest_priority_;
        current_priority >= (ACE_INT32)this->current_lowest_priority_;
        --current_priority)
   {
      PRIORITY_MESSAGE_LIST_MAP::iterator 
         pending_message_iter = this->pending_messages_by_priority_.find(current_priority);
      
      for (; pending_message_iter != this->pending_messages_by_priority_.end(); 
           ++pending_message_iter)
      {
         std::list<ACE_PIP_Protocol_Message*>::iterator next_message_iter = 
            pending_message_iter->second.begin();
      
         if (next_message_iter != pending_message_iter->second.end())
         {
            // The highest-priority message has been found. Grab the message
            // and remove it from both containers
            message = *next_message_iter;
            pending_message_iter->second.pop_front();
            this->pending_messages_by_message_id_.unbind(message->message_id());
            break;
         }
         else
         {
            // There are no messages at this priority. Since the search begins at
            // the highest priority, lower the highest priority until a message
            // is found
            if (this->current_highest_priority_ > this->current_lowest_priority_)
            {
               --this->current_highest_priority_;
            }
         }
      }

      if (message)
      {
         break;
      }
   }

   return message;
}

bool ACE_PIP_Dispatcher::
find_and_accelerate_lower_priority_message(ACE_UINT32 new_priority)
{
   bool found(false);
   bool erased_this_pass(true);

   this->dispatched_messages_lock_.acquire();

   while(erased_this_pass)
   {
      erased_this_pass = false;
   
      std::set<Dispatched_Message_Data>::iterator iter = this->dispatched_messages_data_.begin();
   
      // Find all dispatched messages having priority lower than new_priority. For each
      // send an acceleration message, and update the dispatch record
      for (; iter != this->dispatched_messages_data_.end() &&
              this->num_threads_needed_ > 0; ++iter)
      {
         if (iter->priority < new_priority)
         {
            // A message has been found that has a lower priority, 
            // so the send an acceleration message
            ACE_PIP_Accel_Message* accel_message = new ACE_PIP_Accel_Message;
            accel_message->old_priority(iter->priority);
            accel_message->new_priority(new_priority);
            
            ACE_PIP_Protocol_Message* protocol_message = new ACE_PIP_Protocol_Message;
            protocol_message->message_type(ACE_PIP_Protocol_Message::ACCEL);
            protocol_message->message_id(iter->id);
            protocol_message->next(accel_message);
            
            Dispatched_Message_Data dispatch_record = *iter;
            this->dispatched_messages_data_.erase(iter);
            dispatch_record.priority = new_priority;
            this->dispatched_messages_data_.insert(dispatch_record);
            ACE_PIP_Invocation_Manager::instance()->process_acceleration(protocol_message);
            found = true;
            erased_this_pass = true;
            break;
         }
      }
   }
   
   this->dispatched_messages_lock_.release();
   this->pending_messages_lock_.release();   

   return found;
}

void ACE_PIP_Dispatcher::shutdown()
{
  shutdown_ = true;

  // Pulse signals so waiting threads can quit
  this->message_available_signal_.release();
  this->threads_available_signal_.release();
}

void ACE_PIP_Dispatcher::process_incoming_acceleration(ACE_PIP_Protocol_Message* message)
{
  bool updated_pending(false);
  // Look for pending message. If the message is pending, update the priority, move it around in data structures, and quit

  ACE_PIP_Accel_Message* accel_message = 
     static_cast<ACE_PIP_Accel_Message*>(message->next());

  this->pending_messages_lock_.acquire();
  ACE_Hash_Map_Entry<ACE_UINT64, ACE_PIP_Protocol_Message*>* entry(0);
  if (this->pending_messages_by_message_id_.find(message->message_id(), entry) == 0)
  {
     ACE_PIP_Data_Message* data_message = 
        static_cast<ACE_PIP_Data_Message*>(entry->item()->next());

     data_message->message_priority(accel_message->new_priority());

     // move the message from one priority to the other
     updated_pending = true;
  }
  this->pending_messages_lock_.release();
  
  if (!updated_pending)
  {
    bool found(false);
     ACE_Guard<ACE_Mutex> guard(this->dispatched_messages_lock_);
     // Message is not pending, so must already be dispatche
     std::set<Dispatched_Message_Data>::iterator iter = this->dispatched_messages_data_.begin();
   
      // Find all dispatched messages having priority lower than new_priority. For each
      // send an acceleration message, and update the dispatch record
     for (; iter != this->dispatched_messages_data_.end(); ++iter)
     {
        if ((iter->id == message->message_id()) && 
            (iter->priority < accel_message->new_priority()))
         {
            Dispatched_Message_Data dispatch_record = *iter;
            this->dispatched_messages_data_.erase(iter);
            dispatch_record.priority = accel_message->new_priority();
            this->dispatched_messages_data_.insert(dispatch_record);
            ACE_PIP_Invocation_Manager::instance()->process_acceleration(message);
            found = true;
            break;
         }
     }

     if (!found)
     {
        for (size_t index = 0; index < received_ids_.size(); ++index)
        {
           if (received_ids_[index] == message->message_id())
           {
              found = true;
              break;
           }
        }
     }  
  }
}  
void ACE_PIP_Dispatcher::print_results()
{
   printf("----------------------DISPATCHER_RESULTS-------------\n");
   printf("Num received: %d\n", num_messages_received_);
   printf("Num dispatched: %d\n\n", num_messages_dispatched_);
   printf("Received Ids: \n");
   
   for (size_t index = 0; index < received_ids_.size(); ++index)
   {
      printf("%lld\n", received_ids_[index]);
   }

   printf("\nDispatched Ids: \n");
   for (size_t dispatched_index = 0; dispatched_index < dispatched_ids_.size();
        ++dispatched_index)
   {
      printf("%lld\n", dispatched_ids_[dispatched_index]);
   }

   printf("Num received: %d\n", received_ids_.size());
   printf("Num dispatched: %d\n", dispatched_ids_.size());
   printf("---------------------\n");
}
