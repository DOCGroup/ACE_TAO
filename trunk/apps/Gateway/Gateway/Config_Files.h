/* -*- C++ -*- */
// @(#)Config_Files.h	1.1	10/18/96


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

class CC_Config_File_Entry
  // = TITLE
  //     Stores the information in a Channel Connection entry.
{
public:
  int conn_id_;
  // Connection id for this Channel.

  char host_[BUFSIZ];
  // Host to connect with.

  u_short remote_port_;
  // Port to connect with.

  char direction_;
  // 'I' (input) or 'O' (output)

  int max_retry_delay_;
  // Maximum amount of time to wait for reconnecting.
  
  u_short local_port_;
  // Our local port number.
};

class CC_Config_File_Parser : public File_Parser<CC_Config_File_Entry>
  // = TITLE
  //     Parser for the Channel Connection file.
{
public:
  virtual FP::Return_Type
    read_entry (CC_Config_File_Entry &entry, int &line_number);
};

class RT_Config_File_Entry
  // = TITLE
  //     Stores the information in a Routing Table entry.
{
public:
  enum {
    MAX_DESTINATIONS = 1000 // Total number of multicast destinations.
  };

  int conn_id_;
  // Connection id for this channel.

  int logical_id_;
  // Logical routing id for this channel.
  
  int payload_type_;
  // Type of payload in the message.

  int destinations_[MAX_DESTINATIONS];
  // Connection ids for destinations that we're routing to.
  
  int total_destinations_;
  // Total number of these destinations.
};  

class RT_Config_File_Parser : public File_Parser<RT_Config_File_Entry>
  // = TITLE
  //     Parser for the Routing Table file.
{
public:
  virtual FP::Return_Type
    read_entry (RT_Config_File_Entry &entry, int &line_number);
};

#endif /* _CONFIG_FILES */
