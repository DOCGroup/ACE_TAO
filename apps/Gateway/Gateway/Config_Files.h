/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    Config_Files.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (_CONFIG_FILES)
#define _CONFIG_FILES

#include "ace/OS.h"
#include "File_Parser.h"

// Forward declaration.
class ACE_Event_Channel;

class Proxy_Config_Info
  // = TITLE
  //     Stores connection configuration information.
{
public:
  ACE_INT32 proxy_id_;
  // Connection id for this Proxy_Handler.

  char host_[BUFSIZ];
  // Host to connect with.

  u_short remote_port_;
  // Port to connect with.

  char proxy_role_;
  // 'S' (supplier) or 'C' (consumer).

  ACE_INT32 max_retry_timeout_;
  // Maximum amount of time to wait for reconnecting.
  
  u_short local_port_;
  // Our local port number.

  ACE_INT32 priority_;
  // Priority by which different Consumers and Suppliers should be
  // serviced.

  ACE_Event_Channel *event_channel_;
  // We just need a place to store this until we can pass it along
  // when creating a Proxy_Handler.
};

class Proxy_Config_File_Parser : public File_Parser<Proxy_Config_Info>
  // = TITLE
  //     Parser for the Proxy_Handler Connection file.
{
public:
  virtual FP::Return_Type
    read_entry (Proxy_Config_Info &entry, int &line_number);
};

class Consumer_Config_Info
  // = TITLE
  //     Stores the information in a Consumer Map entry.
{
public:
  enum {
    MAX_CONSUMERS = 1000 // Total number of multicast consumers.
  };

  ACE_INT32 proxy_id_;
  // Connection id for this proxy.

  ACE_INT32 supplier_id_;
  // Logical supplier id for this proxy.
  
  ACE_INT32 type_;
  // Message type.

  ACE_INT32 consumers_[MAX_CONSUMERS];
  // Connection ids for consumers that we're routing to.
  
  int total_consumers_;
  // Total number of these consumers.
};  

class Consumer_Config_File_Parser : public File_Parser<Consumer_Config_Info>
  // = TITLE
  //     Parser for the Consumer Map file.
{
public:
  virtual FP::Return_Type
    read_entry (Consumer_Config_Info &entry, int &line_number);
};

#endif /* _CONFIG_FILES */
