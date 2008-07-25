 /**
  * @file PIP_Dispatcher.h
  *
  * // $Id$
  *
  * @author John Moore <ljohn7@gmail.com>
  *
  * This file contains the specification for a class
  * that dispatches priority inheritance protocol messages
  * to the appropriate message handler. 
*/


#ifndef _PIP_DISPATCHER_H_
#define _PIP_DISPATCHER_H_

// ACE definitions
#include "ace/Event_Handler.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/PIP_DA_Strategy_Adapter.h"
#include "ace/PIP_Messages.h"
#include "ace/RW_Thread_Mutex.h"
#include "ace/Semaphore.h"
#include "ace/Singleton.h"

// STL definitions
#include <list>
#include <map>
#include <set>
#include <vector>

// Forward Declarations
class ACE_PIP_Protocol_Message;

typedef std::map<ACE_UINT32, std::list<ACE_PIP_Protocol_Message*> > 
  PRIORITY_MESSAGE_LIST_MAP;

// Associate each message with a message ID
typedef ACE_Hash_Map_Manager_Ex<ACE_UINT64,
   ACE_PIP_Protocol_Message*,
   ACE_Hash<ACE_UINT64>,
   ACE_Equal_To<ACE_UINT64>,
   ACE_Null_Mutex> ID_MESSAGE_MAP;


/**
 * @class ACE_Dispatcher
 * @brief Dispatches ACE_PIP_Priority_Messages in priority order
 * message handlers. Additionally, notifies handlers when priority inversion is
 * detected.
 *
 * The PIP_Message_Dispatcher implements the priority inheritance protocol.
 * Upon receipt of messages, it determines the highest-priority message to 
 * be dispatched, and dispatches providing enough resources exist. If not enough exist,
 * and a lower priority message has been dispatched, an acceleration message is sent
 * to the corresponding handler to raise the priority of the message, thus 
 * mitigating the inversion.
*/
class ACE_Export ACE_PIP_Dispatcher : public ACE_Event_Handler
{
  public:
   
   /// Constructor
   ACE_PIP_Dispatcher();

   /// Destructor
   virtual ~ACE_PIP_Dispatcher();

   /// obtain the single instance of the dispatcher
   static ACE_PIP_Dispatcher* instance();

   /// Receive a message for eventual dispatching
   void process_message(ACE_PIP_Protocol_Message* message);

   /// Signals the dispatcher to dispatch a new message if possible.
   virtual int handle_output (ACE_HANDLE);

   /// Initializes dispatcher
   void init(ACE_PIP_DA_Strategy_Adapter<ACE_UINT32, ACE_Null_Mutex>* DA_strategy_adapter);

   /// Tell the dispatcher to stop dispatching and release all threads ASAP
   void shutdown();

   /// Accelerate the appropriate message
   void process_incoming_acceleration(ACE_PIP_Protocol_Message* message);

   /// Print statistics
   void print_results();

  private:

   // Dispatched_Message_Data stores the ID and priority
   // of a dispatched message
   class Dispatched_Message_Data
   {
   public:
     
     bool operator<(const Dispatched_Message_Data& other) const
     {
       return (priority < other.priority);
     }
     
     bool operator==(const Dispatched_Message_Data& other) const
     {
       return (id == other.id);
     }

     bool operator!=(const Dispatched_Message_Data& other) const
     {
       return !(*this == other);
     }
     
     ACE_UINT64 id;
     ACE_UINT32 priority;
   };

   class Dispatch_Test_Data
   {
   public:
     ACE_UINT64 id;
     ACE_UINT64 priority;
     ACE_UINT32 num_pending;
     ACE_UINT32 highest_priority;
     ACE_UINT32 lowest_priority;
   };

   /// store the message
   void process_incoming_request(ACE_PIP_Protocol_Message* message);

   /// Find the highest priority message and return it
   ACE_PIP_Protocol_Message* retrieve_highest_priority_pending_message();

   bool find_and_accelerate_lower_priority_message(ACE_UINT32 new_priority);
             
   
   // Dispatched message data is stored to determine which messages are 
   // currently assigned to a thread. This is useful for finding messages 
   // whose priority needs to be accelerated in the case where an inversion 
   // is detected.
   std::set<Dispatched_Message_Data> dispatched_messages_data_;
   ACE_Mutex dispatched_messages_lock_;

   ACE_UINT32 current_highest_priority_;
   ACE_UINT32 current_lowest_priority_;
   
   // Pending messages (those not dispatched) are stored in 2 ways for efficiency
   // 1.) By message id - this is useful for managing priority accelerations
   //     because we can find the appropriate message in constant time
   // 2.) By priority   - this is useful for determining which message to dispatch next
   //                     as messages are dispatched in priority order
   PRIORITY_MESSAGE_LIST_MAP pending_messages_by_priority_;
   ID_MESSAGE_MAP            pending_messages_by_message_id_;
   ACE_Mutex                 pending_messages_lock_;

   // Indicates the dispatcher has a thread waiting to
   // dispatch a message
   bool waiting_for_message_;

   // Number of threads that need to be returned in order to
   // dispatch the current message
   int num_threads_needed_;

   ACE_Semaphore message_available_signal_;
   ACE_Semaphore threads_available_signal_;

   ACE_PIP_DA_Strategy_Adapter<ACE_UINT32, ACE_Null_Mutex>* DA_strategy_adapter_;
   ACE_Mutex deadlock_avoidance_lock_;

   static ACE_PIP_Dispatcher* dispatcher_;
   static ACE_Mutex           instance_lock_;
   static bool                delete_dispatcher_;
   static bool                shutdown_;

   // Test variables
   ACE_UINT32 num_pending_messages_;
   ACE_UINT32 num_messages_received_;
   ACE_UINT32 num_messages_dispatched_;
   
   ACE_Vector<ACE_UINT64> received_ids_;
   ACE_Vector<ACE_UINT64> dispatched_ids_;
   ACE_Vector<Dispatch_Test_Data> dispatch_records_;
};

// Define a singleton class to make the dispatcher globally accessible
typedef ACE_Singleton<ACE_PIP_Dispatcher, ACE_Mutex>
   ACE_PIP_Dispatcher_Singleton;

#endif
