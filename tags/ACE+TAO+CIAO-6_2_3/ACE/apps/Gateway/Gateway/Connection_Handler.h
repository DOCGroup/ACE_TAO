/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Connection_Handler.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt
 */
//=============================================================================


#ifndef _CONNECTION_HANDLER
#define _CONNECTION_HANDLER

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/SOCK_Connector.h"
#include "ace/Svc_Handler.h"
#include "Config_Files.h"
#include "Options.h"
#include "Event.h"

// Forward declaration.
class Event_Channel;

/**
 * @class Connection_Handler
 *
 * @brief <Connection_Handler> contains info about connection state and
 * addressing.
 *
 * The <Connection_Handler> classes process events sent to the
 * Event Channel from Suppliers and forward them to Consumers.
 */
class Connection_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_SYNCH>
{
public:
  /// Default constructor (needed to make <ACE_Connector> happy).
  Connection_Handler (void);

  /// Real constructor.
  Connection_Handler (const Connection_Config_Info &);

  /// Initialize and activate a single-threaded <Connection_Handler>
  /// (called by <ACE_Connector::handle_output>).
  virtual int open (void * = 0);

  // = The current state of the Connection_Handler.
  enum State
  {
    IDLE = 1,      // Prior to initialization.
    CONNECTING,    // During connection establishment.
    ESTABLISHED,   // Connection_Handler is established and active.
    DISCONNECTING, // Connection_Handler is in the process of connecting.
    FAILED         // Connection_Handler has failed.
  };

  // = Set/get the current state.
  void state (State);
  State state (void) const;

  // = Set/get remote INET addr.
  void remote_addr (ACE_INET_Addr &);
  const ACE_INET_Addr &remote_addr (void) const;

  // = Set/get local INET addr.
  void local_addr (ACE_INET_Addr &);
  const ACE_INET_Addr &local_addr (void) const;

  // = Set/get connection id.
  void connection_id (CONNECTION_ID);
  CONNECTION_ID connection_id (void) const;

  // = Set/get the current retry timeout delay.
  void timeout (long);
  long timeout (void);

  // = Set/get the maximum retry timeout delay.
  void max_timeout (long);
  long max_timeout (void) const;

  // = Set/get proxy role (i.e., 'S' for Supplier and 'C' for Consumer
  // (necessary for error checking).
  void connection_role (char);
  char connection_role (void) const;

  // = Set/get the <Event_Channel> *.
  void event_channel (Event_Channel *);
  Event_Channel *event_channel (void) const;

  // = The total number of bytes sent/received on this proxy.
  /// Increment count by <bytes>.
  /// Return the current byte count.
  void total_bytes (size_t bytes);
  size_t total_bytes (void) const;

  /// Perform timer-based Connection_Handler reconnection.
  virtual int handle_timeout (const ACE_Time_Value &, const void *arg);

  /// Perform Connection_Handler termination.
  virtual int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                            ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK);

protected:
  /// Address of peer.
  ACE_INET_Addr remote_addr_;

  /// Address of us.
  ACE_INET_Addr local_addr_;

  /// The assigned connection ID of this entry.
  CONNECTION_ID connection_id_;

  /// The total number of bytes sent/received on this proxy.
  size_t total_bytes_;

  /// The current state of the proxy.
  State state_;

  /// Amount of time to wait between reconnection attempts.
  long timeout_;

  /// Maximum amount of time to wait between reconnection attempts.
  long max_timeout_;

  /// Indicates which role the proxy plays ('S' == Supplier and 'C' ==
  /// Consumer).
  char connection_role_;

  /// Reference to the <Event_Channel> that we use to forward all
  /// the events from Consumers and Suppliers.
  Event_Channel *event_channel_;
};

/**
 * @class Connection_Handler_Factory
 *
 * @brief Creates the appropriate type of <Connection_Handler>.
 *
 * <Connection_Handler>s can include <Consumer_Handler>,
 * <Supplier_Handler>, <Thr_Consumer_Handler>, or
 * <Thr_Supplier_Handler>).
 */
class Connection_Handler_Factory
{
public:
  /// Make the appropriate type of <Connection_Handler>, based on the
  /// <Connection_Config_Info> parameter.
  Connection_Handler *make_connection_handler (const Connection_Config_Info &);
};

#endif /* _CONNECTION_HANDLER */
