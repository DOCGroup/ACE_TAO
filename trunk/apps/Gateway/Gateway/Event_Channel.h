/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    apps
// 
// = FILENAME
//    Event_Channel.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_EVENT_CHANNEL)
#define ACE_EVENT_CHANNEL

#include "IO_Handler_Connector.h"

template <class SUPPLIER_HANDLER, class CONSUMER_HANDLER>
class ACE_Svc_Export ACE_Event_Channel : public ACE_Event_Handler
  // = TITLE
  //    Define a generic Event_Channel.
{
public:
  // = Initialization and termination methods.
  ACE_Event_Channel (void);

  int open (int argc, char *argv[]);
  // Initialize the Channel.

  int close (void);
  // Close down the Channel.

private:
  int parse_args (int argc, char *argv[]);
  // Parse the command-line arguments.

  int parse_connection_config_file (void);
  // Parse the connection configuration file.

  int parse_consumer_config_file (void);
  // Parse the consumer map configuration file.

  int initiate_connections (void);
  // Initiate connections to the peers.

  virtual int handle_timeout (const ACE_Time_Value &, const void *arg);
  // Perform timer-based performance profiling.

  const char *connection_config_file_;
  // Name of the connection configuration file.

  const char *consumer_config_file_;
  // Name of the consumer map configuration file.

  int active_connector_role_;
  // Enabled if we are playing the role of the active Connector.

  int performance_window_;
  // Number of seconds after connection establishment to report
  // throughput.
  
  int blocking_semantics_;
  // 0 == blocking connects, ACE_NONBLOCK == non-blocking connects.

  int debug_;
  // Are we debugging?

  IO_Handler_Connector *connector_;
  // This is used to establish the connections actively.

  int socket_queue_size_;
  // Size of the socket queue (0 means "use default").

  // = Make life easier by defining typedefs.
  typedef ACE_Map_Manager<CONN_ID, IO_Handler *, MAP_MUTEX> CONNECTION_MAP;
  typedef ACE_Map_Iterator<CONN_ID, IO_Handler *, MAP_MUTEX> CONNECTION_MAP_ITERATOR;
  typedef ACE_Map_Entry<CONN_ID, IO_Handler *> CONNECTION_MAP_ENTRY;

  CONNECTION_MAP connection_map_;
  // Table that maps Connection IDs to IO_Handler *'s.

  Consumer_Map consumer_map_;
  // Map that associates event addresses to a set of Consumer_Handler
  // *'s.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Event_Channel.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Event_Channel.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_EVENT_CHANNEL */
