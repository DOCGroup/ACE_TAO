/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Connection_Handler_Acceptor.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================

#ifndef _CONNECTION_HANDLER_ACCEPTOR
#define _CONNECTION_HANDLER_ACCEPTOR

#include "ace/Acceptor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Acceptor.h"
#include "Connection_Handler.h"

// Forward declaration
class Event_Channel;

/**
 * @class Connection_Handler_Acceptor
 *
 * @brief A concrete factory class that setups connections to peerds
 * and produces a new Connection_Handler object to do the dirty
 * work...
 */
class Connection_Handler_Acceptor : public ACE_Acceptor<Connection_Handler, ACE_SOCK_ACCEPTOR>
{
public:
  /// Constructor.
  Connection_Handler_Acceptor (Event_Channel &,
                               char connection_role);

  /// Hook method for creating an appropriate <Connection_Handler>.
  virtual int make_svc_handler (Connection_Handler *&ch);

  /// Hook method for accepting a connection into the
  /// <Connection_Handler>.
  virtual int accept_svc_handler (Connection_Handler *ch);

protected:
  /// Make life easier later on.
  typedef ACE_Acceptor<Connection_Handler, ACE_SOCK_ACCEPTOR>
          inherited;

  /// Reference to the event channel.
  Event_Channel &event_channel_;

  /// Keeps track of what type of proxy we need to create.
  Connection_Config_Info connection_config_info_;

  /// Make the appropriate type of <Connection_Handler>.
  Connection_Handler_Factory connection_handler_factory_;
};

#endif /* _CONNECTION_HANDLER_ACCEPTOR */
