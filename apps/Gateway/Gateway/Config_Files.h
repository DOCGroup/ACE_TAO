/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Config_Files.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef _CONFIG_FILES
#define _CONFIG_FILES

#include "File_Parser.h"
#include "Event.h"

// Forward declaration.
class Event_Channel;

/**
 * @class Connection_Config_Info
 *
 * @brief Stores connection configuration information.
 */
class Connection_Config_Info
{
public:
  /// Connection id for this Connection_Handler.
  ACE_INT32 connection_id_;

  /// Host to connect with.
  char host_[BUFSIZ];

  /// Port to connect with.
  u_short remote_port_;

  /// 'S' (supplier) or 'C' (consumer).
  char connection_role_;

  /// Maximum amount of time to wait for reconnecting.
  ACE_INT32 max_retry_timeout_;

  /// Our local port number.
  u_short local_port_;

  /// Priority by which different Consumers and Suppliers should be
  /// serviced.
  ACE_INT32 priority_;

  /// We just need a place to store this until we can pass it along
  /// when creating a Connection_Handler.
  Event_Channel *event_channel_;
};

/**
 * @class Connection_Config_File_Parser
 *
 * @brief Parser for the Connection_Handler Connection file.
 */
class Connection_Config_File_Parser : public File_Parser<Connection_Config_Info>
{
public:
  /// Read in a <Connection_Config_Info> entry.
  virtual FPRT::Return_Type read_entry (Connection_Config_Info &entry,
                                        int &line_number);

};

/**
 * @class Consumer_Config_Info
 *
 * @brief Stores the information in a Consumer Map entry.
 */
class Consumer_Config_Info
{
public:
  /// Connection id.
  ACE_INT32 connection_id_;

  /// Message type.
  ACE_INT32 type_;

  /// Connection ids for consumers that will be routed information
  /// containing this <connection_id_>
  ACE_INT32 consumers_[MAX_CONSUMERS];

  /// Total number of these consumers.
  ACE_INT32 total_consumers_;
};

/**
 * @class Consumer_Config_File_Parser
 *
 * @brief Parser for the Consumer Map file.
 */
class Consumer_Config_File_Parser : public File_Parser<Consumer_Config_Info>
{
public:
  /// Read in a <Consumer_Config_Info> entry.
  virtual FPRT::Return_Type read_entry (Consumer_Config_Info &entry,
                                        int &line_number);
};

#endif /* _CONFIG_FILES */
