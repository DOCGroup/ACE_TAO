/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    gateway
// 
// = FILENAME
//    Connection_Handler_acceptor.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (_CONNECTION_HANDLER_ACCEPTOR)
#define _CONNECTION_HANDLER_ACCEPTOR

#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "Connection_Handler.h"

// Forward declaration
class Event_Channel;

class Connection_Handler_Acceptor : public ACE_Acceptor<Connection_Handler, ACE_SOCK_ACCEPTOR>
{
  // = TITLE
  //     A concrete factory class that setups connections to peerds
  //     and produces a new Connection_Handler object to do the dirty
  //     work...
public:
  Connection_Handler_Acceptor (Event_Channel &, 
                          char connection_role);

  virtual int make_svc_handler (Connection_Handler *&ph);
  // Hook method for creating an appropriate <Connection_Handler>.

protected:  
  Event_Channel &event_channel_;
  // Reference to the event channel.
  
  Connection_Config_Info connection_config_info_;
  // Keeps track of what type of proxy we need to create.

  Connection_Handler_Factory connection_handler_factory_;
  // Make the appropriate type of <Connection_Handler>.
};

#endif /* _CONNECTION_HANDLER_ACCEPTOR */
