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

class Connection_Config_File_Entry
  // = TITLE
  //     Stores the Proxy_Handler entry for connection configuration.
{
public:
  int conn_id_;
  // Connection id for this Proxy_Handler.

  char host_[BUFSIZ];
  // Host to connect with.

  u_short remote_poconsumer_;
  // Port to connect with.

  char direction_;
  // 'S' (supplier) or 'C' (consumer).

  int max_retry_delay_;
  // Maximum amount of time to wait for reconnecting.
  
  u_short local_poconsumer_;
  // Our local port number.
};

class Connection_Config_File_Parser : public File_Parser<Connection_Config_File_Entry>
  // = TITLE
  //     Parser for the Proxy_Handler Connection file.
{
public:
  virtual FP::Return_Type
    read_entry (Connection_Config_File_Entry &entry, int &line_number);
};

class Consumer_Config_File_Entry
  // = TITLE
  //     Stores the information in a Consumer Map entry.
{
public:
  enum {
    MAX_DESTINATIONS = 1000 // Total number of multicast destinations.
  };

  int conn_id_;
  // Connection id for this channel.

  int supplier_id_;
  // Logical routing id for this channel.
  
  int type_;
  // Type of payload in the message.

  int destinations_[MAX_DESTINATIONS];
  // Connection ids for destinations that we're routing to.
  
  int total_destinations_;
  // Total number of these destinations.
};  

class Consumer_Config_File_Parser : public File_Parser<Consumer_Config_File_Entry>
  // = TITLE
  //     Parser for the Consumer Map file.
{
public:
  virtual FP::Return_Type
    read_entry (Consumer_Config_File_Entry &entry, int &line_number);
};

#endif /* _CONFIG_FILES */
