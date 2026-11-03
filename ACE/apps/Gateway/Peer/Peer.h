/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Peer.h
 *
 *  These classes process Supplier/Consumer events sent from the
 *  gateway (gatewayd) to its various peers (peerd).  The general
 *  collaboration works as follows:
 *
 *  1. <Peer_Acceptor> creates a listener endpoint and waits
 *     passively for gatewayd to connect with it.
 *
 *  2. When a gatewayd connects, <Peer_Acceptor> creates an
 *     <Peer_Handler> object that sends/receives events from
 *     gatewayd on that connection.
 *
 *  3. The <Peer_Handler> waits for gatewayd to inform it of its
 *     connection ID, which is prepended to all subsequent outgoing
 *     events sent from peerd.
 *
 *  4. Once the connection ID is set, peerd periodically sends events
 *     to gatewayd.  Peerd also receives and "processes" events
 *     forwarded to it from gatewayd.  In this program, peerd
 *     "processes" the events sent to it by writing them to stdout.
 *
 *  Note that in the current peerd implementation, one Peer process
 *  cannot serve as both a Consumer and Supplier of Events.  This is
 *  because the gatewayd establishes a separate connection for
 *  Suppliers and Consumers and the peerd only maintains a single
 *  <Peer_Handler> object to handle this one connection.  Enhancing
 *  this implementation to be both a Consumer and Supplier
 *  simultaneously is straightforward, however.  In addition,
 *  multiple peerd processes can already work together to play these
 *  different roles.
 *
 *  @author Douglas C. Schmidt
 */
//=============================================================================


#ifndef PEER_H
#define PEER_H

#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/Svc_Handler.h"
#include "ace/Connector.h"
#include "ace/Null_Condition.h"
#include "ace/Null_Mutex.h"
#include "Options.h"

ACE_SVC_FACTORY_DECLARE (Peer_Factory)

#if defined ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT
template class ACE_Svc_Export ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT */

/**
 * @class Peer_Handler
 *
 * @brief Handle Peer events arriving from a Gateway.
 */
class ACE_Svc_Export Peer_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  /// Initialize the peer.
  Peer_Handler ();

  /// Shutdown the Peer.
  ~Peer_Handler () override;
  Peer_Handler (Peer_Handler const &) = delete;
  Peer_Handler (Peer_Handler &&) = delete;
  Peer_Handler &operator= (Peer_Handler const &) = delete;
  Peer_Handler &operator= (Peer_Handler &&) = delete;

  /// Initialize the handler when called by
  /// <ACE_Acceptor::handle_input>.
  int open (void * = nullptr) override;

  /// Receive and process peer events.
  int handle_input (ACE_HANDLE) override;

  /// Send a event to a gateway (may be queued if necessary due to flow
  /// control).
  int put (ACE_Message_Block *, ACE_Time_Value *tv = nullptr) override;

  /// Finish sending a event when flow control conditions abate.
  int handle_output (ACE_HANDLE) override;

  /// Periodically send events via <ACE_Reactor> timer mechanism.
  int handle_timeout (const ACE_Time_Value &,
                      const void *arg) override;

  /// Perform object termination.
  int handle_close (ACE_HANDLE = ACE_INVALID_HANDLE,
                    ACE_Reactor_Mask = ACE_Event_Handler::ALL_EVENTS_MASK) override;

protected:
  using inherited = ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;

  /// Transmit <mb> to the gatewayd.
  int transmit (ACE_Message_Block *mb,
                size_t n,
                int event_type);

  /// Receive an Peer event from gatewayd.
  virtual int recv (ACE_Message_Block *&mb);

  /// Send an Peer event to gatewayd, using <nonblk_put>.
  virtual int send (ACE_Message_Block *mb);

  /// Perform a non-blocking <put>, which tries to send an event to the
  /// gatewayd, but only if it isn't flow controlled.
  virtual int nonblk_put (ACE_Message_Block *mb);

  /// Register Consumer subscriptions with the gateway.
  int subscribe ();

  // = Event/state/action handlers.
  /// Receive a event from stdin and send it to the gateway.
  int transmit_stdin ();

  /// Action that receives the route id.
  int await_connection_id ();

  /// Action that receives events.
  int await_events ();

  /// Pointer-to-member-function for the current action to run in this
  /// state.  This points to one of the preceding 3 methods.
  int (Peer_Handler::*do_action_)();

  /// Connection ID of the peer, which is obtained from the gatewayd.
  CONNECTION_ID connection_id_;

  /// Keep track of event fragments that arrive in non-blocking recv's
  /// from the gatewayd.
  /// This handler owns the fragment until it is released or passed on.
  ACE_Message_Block *msg_frag_;

  /// The total number of bytes sent/received to the gatewayd thus far.
  size_t total_bytes_;

  /// Used to call register_stdin_handle only once.  Otherwise, thread
  /// leak will occur on Win32.
  bool first_time_;
};

#if defined ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT
template class ACE_Svc_Export ACE_Acceptor<Peer_Handler, ACE_SOCK_ACCEPTOR>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT */

/**
 * @class Peer_Acceptor
 *
 * @brief Passively accept connections from gatewayd and dynamically
 * create a new <Peer_Handler> object to communicate with the
 * gatewayd.
 */
class ACE_Svc_Export Peer_Acceptor : public ACE_Acceptor<Peer_Handler, ACE_SOCK_ACCEPTOR>
{
public:
  /// Default initialization.
  Peer_Acceptor ();
  Peer_Acceptor (Peer_Acceptor const &) = delete;
  Peer_Acceptor (Peer_Acceptor &&) = delete;
  Peer_Acceptor &operator= (Peer_Acceptor const &) = delete;
  Peer_Acceptor &operator= (Peer_Acceptor &&) = delete;

  ///  the <Peer_Acceptor>.
  int start (u_short);

  /// Terminate the <Peer_Acceptor>.
  int close () override;

  /// Factory method that creates a <Peer_Handler> just once.
  int make_svc_handler (Peer_Handler *&) override;

private:
  /// Factor out common code for initializing the <Peer_Acceptor>.
  int open_acceptor (u_short port);

  /// Pointer to <Peer_Handler> allocated once and destroyed by ACE_Acceptor.
  Peer_Handler *peer_handler_;

  /// Our acceptor addr.
  ACE_INET_Addr addr_;

  using inherited = ACE_Acceptor<Peer_Handler, ACE_SOCK_ACCEPTOR>;
};

#if defined ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT
template class ACE_Svc_Export ACE_Connector<Peer_Handler, ACE_SOCK_CONNECTOR>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT */

/**
 * @class Peer_Connector
 *
 * @brief Actively establish connections with gatewayd and dynamically
 * create a new <Peer_Handler> object to communicate with the
 * gatewayd.
 */
class ACE_Svc_Export Peer_Connector : public ACE_Connector<Peer_Handler, ACE_SOCK_CONNECTOR>
{
public:
  Peer_Connector () = default;

  /**
   * Initialize the <Peer_Connector>.  NOTE:  the arguments are
   * ignored.  They are only provided to avoid a compiler warning
   * about hiding the virtual function ACE_Connector<Peer_Handler,
   * ACE_SOCK_CONNECTOR>::open(ACE_Reactor*, int).
   */
  int open (ACE_Reactor * = nullptr, int = 0) override;
  Peer_Connector (Peer_Connector const &) = delete;
  Peer_Connector (Peer_Connector &&) = delete;
  Peer_Connector &operator= (Peer_Connector const &) = delete;
  Peer_Connector &operator= (Peer_Connector &&) = delete;

private:
  /// Factor out common code for initializing the <Peer_Connector>.
  int open_connector (Peer_Handler *&ph, u_short port);

  /// Consumer <Peer_Handler> that is connected to a gatewayd.
  Peer_Handler *consumer_peer_handler_;

  /// Supplier <Peer_Handler> that is connected to a gatewayd.
  Peer_Handler *supplier_peer_handler_;
};

/**
 * @class Peer_Factory
 *
 * @brief A factory class that actively and/or passively establishes
 * connections with the gatewayd.
 */
class ACE_Svc_Export Peer_Factory : public ACE_Service_Object
{
public:
  Peer_Factory () = default;

  Peer_Factory (Peer_Factory const &) = delete;
  Peer_Factory (Peer_Factory &&) = delete;
  Peer_Factory &operator= (Peer_Factory const &) = delete;
  Peer_Factory &operator= (Peer_Factory &&) = delete;

  // = Dynamic initialization and termination hooks from <ACE_Service_Object>.

  /// Initialize the acceptor and connector.
  int init (int argc, ACE_TCHAR *argv[]) override;

  /// Perform termination activities.
  int fini () override;

  /// Return info about this service.
  int info (ACE_TCHAR **, size_t) const override;

  /// Handle various signals (e.g., SIGPIPE, SIGINT, and SIGQUIT).
  int handle_signal (int signum, siginfo_t *, ucontext_t *) override;

private:
  /// Pointer to an instance of our <Peer_Acceptor> that's used to
  /// accept connections and create Consumers.
  Peer_Acceptor consumer_acceptor_;

  /// Pointer to an instance of our <Peer_Acceptor> that's used to
  /// accept connections and create Suppliers.
  Peer_Acceptor supplier_acceptor_;

  /**
   * An instance of our <Peer_Connector>.  Note that one
   * <Peer_Connector> is used to establish <Peer_Handler>s for both
   * Consumers and Suppliers.
   */
  Peer_Connector connector_;
};

#endif /* PEER_H */
