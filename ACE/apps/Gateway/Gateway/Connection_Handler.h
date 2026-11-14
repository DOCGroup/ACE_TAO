/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Connection_Handler.h
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
  Connection_Handler ();

  /// Real constructor.
  explicit Connection_Handler (Connection_Config_Info const &);
  Connection_Handler (Connection_Handler const &) = delete;
  Connection_Handler (Connection_Handler &&) = delete;
  Connection_Handler &operator= (Connection_Handler const &) = delete;
  Connection_Handler &operator= (Connection_Handler &&) = delete;

  /// Initialize and activate a single-threaded <Connection_Handler>
  /// (called by <ACE_Connector::handle_output>).
  int open (void * = nullptr) override;

  /// Current state of the connection handler.
  enum State
  {
    IDLE = 1,      // Prior to initialization.
    CONNECTING,    // During connection establishment.
    ESTABLISHED,   // Connection_Handler is established and active.
    DISCONNECTING, // Connection_Handler is in the process of connecting.
    FAILED         // Connection_Handler has failed.
  };

  /// Get or set the current state.
  void state (State);
  State state () const;

  /// Get or set the remote INET address.
  void remote_addr (ACE_INET_Addr &);
  ACE_INET_Addr const &remote_addr () const;

  /// Get or set the local INET address.
  void local_addr (ACE_INET_Addr &);
  ACE_INET_Addr const &local_addr () const;

  /// Get or set the connection ID.
  void connection_id (CONNECTION_ID);
  CONNECTION_ID connection_id () const;

  /// Get or set the current retry timeout delay.
  void timeout (long);
  long timeout ();

  /// Get or set the maximum retry timeout delay.
  void max_timeout (long);
  long max_timeout () const;

  /// Get or set the proxy role ('S' for Supplier and 'C' for Consumer).
  void connection_role (char);
  char connection_role () const;

  /// Get or set the event channel.
  void event_channel (Event_Channel *);
  Event_Channel *event_channel () const;

  /// Get or set the total number of bytes sent or received on this proxy.
  /// Increment count by <bytes>.
  /// Return the current byte count.
  void total_bytes (size_t bytes);
  size_t total_bytes () const;

  /// Perform timer-based Connection_Handler reconnection.
  int handle_timeout (ACE_Time_Value const &, void const *arg) override;

  /// Perform Connection_Handler termination.
  int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                    ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK) override;

protected:
  /// Address of peer.
  ACE_INET_Addr remote_addr_;

  /// Address of us.
  ACE_INET_Addr local_addr_;

  /// The assigned connection ID of this entry.
  CONNECTION_ID connection_id_ {};

  /// The total number of bytes sent/received on this proxy.
  size_t total_bytes_ {};

  /// The current state of the proxy.
  State state_ {Connection_Handler::IDLE};

  /// Amount of time to wait between reconnection attempts.
  long timeout_ {1};

  /// Maximum amount of time to wait between reconnection attempts.
  long max_timeout_ {1};

  /// Indicates which role the proxy plays ('S' == Supplier and 'C' ==
  /// Consumer).
  char connection_role_ {};

  /// Reference to the <Event_Channel> that we use to forward all
  /// the events from Consumers and Suppliers.
  Event_Channel *event_channel_ {};
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
  Connection_Handler *make_connection_handler (Connection_Config_Info const &);
};

#endif /* _CONNECTION_HANDLER */
