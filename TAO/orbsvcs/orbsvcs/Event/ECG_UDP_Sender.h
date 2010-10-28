// -*- C++ -*-

/**
 * @file ECG_UDP_Sender.h
 *
 * $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *  @author Marina Spivak (marina@atdesk.com)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 *
 * Define helper classes to propagate events between ECs using
 * either UDP or multicast.
 * The architecture is a bit complicated and deserves some
 * explanation: sending the events over UDP (or mcast) is easy, a
 * Consumer (TAO_ECG_UDP_Sender) subscribes for a certain set of
 * events, its push() method marshalls the event set into a CDR
 * stream that is sent using an ACE_SOCK_Dgram. The subscription
 * set and IP address can be configured.
 * Another helper class (TAO_ECG_UDP_Receiver) acts as a supplier of
 * events; it receives a callback when an event is available on an
 * ACE_SOCK_Dgram, it demarshalls the event and pushes it to the
 * EC.  Two ACE_Event_Handler classes are provided that can forward
 * the events to this Supplier: TAO_ECG_Mcast_EH can receive events
 * from a multicast group; TAO_ECG_UDP_EH can receive events from a
 * regular UDP socket.
 *
 * @todo The class makes an extra copy of the events, we need to
 * investigate if closer collaboration with its collocated EC could
 * be used to remove that copy.
 *
 */

#ifndef TAO_ECG_UDP_SENDER_H
#define TAO_ECG_UDP_SENDER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/RtecUDPAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "orbsvcs/Event/event_serv_export.h"
#include "orbsvcs/RtecEventChannelAdminS.h"

#include "orbsvcs/Event/EC_Lifetime_Utils.h"
#include "orbsvcs/Event/EC_Lifetime_Utils_T.h"
#include "orbsvcs/Event/ECG_CDR_Message_Sender.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_SOCK_Dgram;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ECG_UDP_Out_Endpoint;

/**
 * @class TAO_ECG_UDP_Sender_Disconnect_Command
 *
 * @brief Disconnects consumer represented by @a proxy from the Event Channel.
 *
 * Utility class for use as a template argument to TAO_EC_Auto_Command.
 * TAO_EC_Auto_Command<TAO_ECG_UDP_Sender_Disconnect_Command> manages
 * consumer connection to the Event Channel, automatically disconnecting from
 * @a proxy in its destructor, if necessary.
 */
class TAO_RTEvent_Serv_Export TAO_ECG_UDP_Sender_Disconnect_Command
{
public:
  TAO_ECG_UDP_Sender_Disconnect_Command (void);
  TAO_ECG_UDP_Sender_Disconnect_Command (
              RtecEventChannelAdmin::ProxyPushSupplier_ptr proxy);

  TAO_ECG_UDP_Sender_Disconnect_Command (
              const TAO_ECG_UDP_Sender_Disconnect_Command & rhs);

  TAO_ECG_UDP_Sender_Disconnect_Command &
   operator= (const TAO_ECG_UDP_Sender_Disconnect_Command & rhs);

  void execute (void);

private:

  RtecEventChannelAdmin::ProxyPushSupplier_var proxy_;
};


/**
 * @class TAO_ECG_UDP_Sender
 *
 * @brief Send events received from a "local" EC using UDP.
 *        NOT THREAD-SAFE.
 * This class connect as a consumer to an EventChannel
 * and forwards the events it receives from that EC using UDP.
 *
 */
class TAO_RTEvent_Serv_Export TAO_ECG_UDP_Sender :
  public virtual POA_RtecEventComm::PushConsumer,
  public TAO_EC_Deactivated_Object
{
public:

  /// Initialization and termination methods.
  //@{

  /// Create a new TAO_ECG_UDP_Sender object.
  /// (Constructor access is restricted to insure that all
  /// TAO_ECG_UDP_Sender objects are heap-allocated.)
  static TAO_EC_Servant_Var<TAO_ECG_UDP_Sender> create (CORBA::Boolean crc = 0);

  ~TAO_ECG_UDP_Sender (void);

  /**
   * @param lcl_ec Event Channel to which we will act as a consumer of events.
   * @param addr_server Address server used to obtain event type to
   *        multicast group mapping.
   * @param endpoint_rptr Endpoint for sending udp/multicast messages.
   *        Endpoint's dgram must be open!
   *
   * To insure proper resource clean up, if init () is successful,
   * shutdown () must be called when the sender is no longer needed.
   * This is done by disconnect_push_consumer() method.  If
   * disconnect_push_consumer() will not be called, it is the
   * responsibility of the user.
   * Furthermore, if shutdown() is not explicitly called by
   * either disconnect_push_consumer () or the user, the sender
   * will clean up the resources in its destructor, however, certain
   * entities involved in cleanup must still exist at that point,
   * e.g., POA.
   */
  void init (RtecEventChannelAdmin::EventChannel_ptr lcl_ec,
             RtecUDPAdmin::AddrServer_ptr addr_server,
             TAO_ECG_Refcounted_Endpoint endpoint_rptr);

  /// Connect or reconnect to the EC with the given subscriptions.
  /**
   * @note if we are already connected to EC and a reconnection is
   * necessary, the EC must have reconnects enabled in order for this
   * method to succeed.
   */
  void connect (const RtecEventChannelAdmin::ConsumerQOS &sub);

  /// Deactivate from POA and disconnect from EC, if necessary.  Shut
  /// down all sender components.
  /**
   * Calling this method may result in decrementing of the reference
   * count (due to deactivation) and deletion of the object.
   */
  void shutdown (void);
  //@}

  /// Accessors.
  //@{
  /**
   * The sender may need to fragment the message, otherwise the
   * network may drop the packets.
   * Setting the MTU can fail if the value is too small (at least the
   * header + 8 bytes must fit).
   */
  int mtu (CORBA::ULong mtu);
  CORBA::ULong mtu (void) const;

  /// Get the local endpoint used to send the events.
  int get_local_addr (ACE_INET_Addr& addr);
  //@}

  /// The PushConsumer methods.
  //@{
  /// Invokes shutdown (), which may result in the object being deleted, if
  /// refcounting is used to manage its lifetime.
  virtual void disconnect_push_consumer (void);
  virtual void push (const RtecEventComm::EventSet &events);
  //@}

protected:

  /// Constructor (protected).  Clients can create new
  /// TAO_ECG_UDP_Sender objects using the static create() method.
  TAO_ECG_UDP_Sender (CORBA::Boolean crc = 0);

private:

  /// Helpers for the connect() method.
  //@{
  // Establishes connection to the Event Channel for the first time.
  void new_connect (const RtecEventChannelAdmin::ConsumerQOS& sub);

  // Updates existing connection to the Event Channel.
  void reconnect (const RtecEventChannelAdmin::ConsumerQOS& sub);
  //@}

  /// Proxy used to receive events from the Event Channel.
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;

  /// Event Channel to which we act as a consumer.
  RtecEventChannelAdmin::EventChannel_var lcl_ec_;

  /// We query this object to determine where the events should be sent.
  RtecUDPAdmin::AddrServer_var addr_server_;

  /// Helper for fragmenting and sending cdr-encoded events using udp.
  TAO_ECG_CDR_Message_Sender cdr_sender_;

  typedef TAO_EC_Auto_Command<TAO_ECG_UDP_Sender_Disconnect_Command>
  ECG_Sender_Auto_Proxy_Disconnect;
  /// Manages our connection to Supplier Proxy.
  ECG_Sender_Auto_Proxy_Disconnect auto_proxy_disconnect_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(__ACE_INLINE__)
#include "orbsvcs/Event/ECG_UDP_Sender.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_ECG_UDP_SENDER_H */
