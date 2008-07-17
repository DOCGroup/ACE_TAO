 /**
  * @file PIP_Invocation_Manager.h
  *
  * // $Id$
  *
  * @author John Moore <ljohn7@gmail.com>
  *
  * This file contains the specification for a class
  * that tracks handler invocations at a particular site
  */


#ifndef _PIP_INVOCATION_MANAGER_H_
#define _PIP_INVOCATION_MANAGER_H_

#include "ace/Containers_T.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Message_Block.h"
#include "ace/PIP_Messages.h"
#include "ace/PIP_Message_Handler.h"
#include "ace/Singleton.h"
#include "ace/Mutex.h"
#include "ace/Null_Mutex.h"
#include "ace/Future.h"

#include <list>
class ACE_PIP_IO_Handler;

struct Invocation_Data
{
  ACE_Future<ACE_Message_Block*>* response_holder;
  ACE_UINT32                      site_id;
  ACE_UINT32                      priority;
};

// Typedefs
typedef ACE_Hash_Map_Manager_Ex<ACE_UINT64,
  std::list<ACE_UINT64>,
  ACE_Hash<ACE_UINT64>,
  ACE_Equal_To<ACE_UINT64>,
  ACE_Null_Mutex> ID_TO_ID_LIST_MAP;

typedef ACE_Hash_Map_Manager_Ex<ACE_UINT32,
  ACE_PIP_Message_Handler*,
  ACE_Hash<ACE_UINT32>,
  ACE_Equal_To<ACE_UINT32>,
  ACE_Null_Mutex> ID_TO_HANDLER_MAP;

typedef ACE_Hash_Map_Manager_Ex<ACE_UINT64,
  Invocation_Data,
  ACE_Hash<ACE_UINT64>,
  ACE_Equal_To<ACE_UINT64>,
  ACE_Null_Mutex> ID_TO_INVOCATION_RECORD_MAP;

typedef ACE_Hash_Map_Manager_Ex<ACE_UINT32,
  ACE_PIP_IO_Handler*,
  ACE_Hash<ACE_UINT32>,
  ACE_Equal_To<ACE_UINT32>,
  ACE_Null_Mutex> PRIORITY_TO_IO_HANDLER_MAP;

typedef ACE_Hash_Map_Manager_Ex<ACE_UINT32,
  PRIORITY_TO_IO_HANDLER_MAP*,
  ACE_Hash<ACE_UINT64>,
  ACE_Equal_To<ACE_UINT64>,
  ACE_Null_Mutex> SITE_TO_IO_HANDLERS_MAP;

/**
 * @class ACE_PIP_Invocation_Manager
 * @brief 
*/
class ACE_Export ACE_PIP_Invocation_Manager
{
 public:

  /// Constructor
  ACE_PIP_Invocation_Manager();

  /// Destructor
  ~ACE_PIP_Invocation_Manager();

  /// Get the singleton instance of the Invocation Manager
  static ACE_PIP_Invocation_Manager* instance();

  /// Associated a site ID with the Invocation Manager
  static void set_site_id(ACE_UINT64 site_id);
  
  /// Process request made on local handler
  void process_inbound_request(ACE_PIP_Protocol_Message* message);

  /// Processes request to be forwarded to another handler
  void process_outbound_request(ACE_Message_Block* message, 
                                ACE_UINT64 token,
                                ACE_Future<ACE_Message_Block*>*& response_holder);

  /// Process response to message sent by local handler
  void process_inbound_response(ACE_PIP_Protocol_Message* message);

  /// Process response sent to remote handler
  void process_outbound_response(ACE_Message_Block* message, ACE_UINT64 token);

  /// Process request to accelerate the priority of a process
  void process_acceleration(ACE_PIP_Protocol_Message* message);

  /// Register an IO handler that can send messages of a certain priority
  /// for the Invocation Manager
  void register_IO_handler(ACE_PIP_IO_Handler* handler);

  /// Un-register an IO handler 
  void unregister_IO_handler(ACE_PIP_IO_Handler* handler);

  /// Register user-level message handler
  void register_message_handler(ACE_PIP_Message_Handler* handler);

 private:


  ACE_UINT64 generate_message_id();

  ACE_UINT64 message_counter_;

  static ACE_UINT64 message_id_base_;

  static ACE_UINT32 site_id_;

  static ACE_PIP_Invocation_Manager* invocation_manager_;

  static ACE_Mutex instance_lock_;

  ACE_Mutex  big_lock_;

  static bool delete_manager_;

  // Mapping of incoming messages to corresponding outgoing messages.
  // This is used to track invocations resulting from an incoming invocation
  // in order to pass acceleration messages along the chain
  ID_TO_ID_LIST_MAP in_out_id_map_;

  // Mapping of user-level handler ID to corresponding handler
  ID_TO_HANDLER_MAP object_id_handler_map_;

  // Mapping of site IDs to corresponding I/O handler map
  SITE_TO_IO_HANDLERS_MAP site_to_handlers_map_;

  // Maps message ID to data such as destination site
  // and priority
  ID_TO_INVOCATION_RECORD_MAP invocation_data_map_;
};


#endif
