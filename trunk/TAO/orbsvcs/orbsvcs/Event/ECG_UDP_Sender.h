/* -*- C++ -*- */
/**
 * @file ECG_UDP_Sender.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 *
 */
#ifndef TAO_ECG_UDP_SENDER_H
#define TAO_ECG_UDP_SENDER_H
#include "ace/pre.h"

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/RtecUDPAdminC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_INET_Addr;
class ACE_SOCK_Dgram;
class ACE_Message_Block;
class TAO_ECG_UDP_Out_Endpoint;

/**
 * @class TAO_ECG_UDP_Sender
 *
 * @brief Send events received from a "local" EC using UDP.
 *
 * This class connect as a consumer to an EventChannel
 * and it sends the events using UDP, the UDP address can be a
 * normal IP address or it can be a multicast group.
 * The UDP address is obtained from a RtecUDPAdmin::AddrServer
 * class.
 * It marshalls the events using TAO CDR classes.
 *
 */
class TAO_RTEvent_Export TAO_ECG_UDP_Sender : public POA_RtecEventComm::PushConsumer
{
public:
  TAO_ECG_UDP_Sender (void);

  /// Get the local endpoint used to send the events.
  int get_local_addr (ACE_INET_Addr& addr);

  /**
   * To do its job this class requires to know the local EC it will
   * connect to; it also requires to build an RT_Info for the local
   * scheduler.
   * It only keeps a copy of its SupplierProxy, used for later
   * connection and disconnections.
   * @todo part of the RT_Info is hardcoded, we need to make it
   * parametric.
   */
  void init (RtecEventChannelAdmin::EventChannel_ptr lcl_ec,
             RtecUDPAdmin::AddrServer_ptr addr_server,
             TAO_ECG_UDP_Out_Endpoint *endpoint
             TAO_ENV_ARG_DECL_WITH_DEFAULTS);

  /**
   * The sender may need to fragment the message, otherwise the
   * network may drop the packets.
   * Setting the MTU can fail if the value is too small (at least the
   * header + 8 bytes must fit).
   */
  int mtu (CORBA::ULong mtu);
  CORBA::ULong mtu (void) const;

  /// Disconnect and shutdown the sender, no further connections will
  /// work unless init() is called again.
  void shutdown (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /// Connect (or reconnect) to the EC with the given subscriptions.
  void open (RtecEventChannelAdmin::ConsumerQOS &sub
             TAO_ENV_ARG_DECL_WITH_DEFAULTS);

  /// Disconnect from the EC, but reconnection is still possible.
  void close (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /// The PushConsumer methods.
  virtual void disconnect_push_consumer (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void push (const RtecEventComm::EventSet &events
                     TAO_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// Marshal one event
  /**
   * The event TTL field is adjusted during marshaling, but without
   * performing expensive copying.
   */
  void marshal_one_event (const RtecEventComm::Event& e,
                          TAO_OutputCDR &cdr
                          TAO_ENV_ARG_DECL);

  /// Helper function to marshal and send a single event.
  void send_event (const RtecUDPAdmin::UDP_Addr& udp_addr,
                   const RtecEventComm::Event& e
                   TAO_ENV_ARG_DECL);

  /// Helper function to send one or more events once they have been
  /// marshaled into a CDR stream.
  void send_cdr_stream (const RtecUDPAdmin::UDP_Addr& udp_addr,
                        TAO_OutputCDR &cdr
                        TAO_ENV_ARG_DECL);

  /**
   * Send one fragment, the first entry in the iovec is used to send
   * the header, the rest of the iovec array should contain pointers
   * to the actual data.
   */
  void send_fragment (const RtecUDPAdmin::UDP_Addr& udp_addr,
                      CORBA::ULong request_id,
                      CORBA::ULong request_size,
                      CORBA::ULong fragment_size,
                      CORBA::ULong fragment_offset,
                      CORBA::ULong fragment_id,
                      CORBA::ULong fragment_count,
                      iovec iov[],
                      int iovcnt
                      TAO_ENV_ARG_DECL_WITH_DEFAULTS);

  /**
   * Count the number of fragments that will be required to send the
   * message blocks in the range [begin,end)
   * The maximum fragment payload (i.e. the size without the header is
   * also required); <total_length> returns the total message size.
   */
  CORBA::ULong compute_fragment_count (const ACE_Message_Block* begin,
                                       const ACE_Message_Block* end,
                                       int iov_size,
                                       CORBA::ULong max_fragment_payload,
                                       CORBA::ULong& total_length);

private:
  /// The remote and the local EC, so we can reconnect when the
  /// subscription list changes.
  RtecEventChannelAdmin::EventChannel_var lcl_ec_;

  /// We talk to the EC (as a consumer) using this proxy.
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;

  /// We query this object to determine where are the events sent.
  RtecUDPAdmin::AddrServer_var addr_server_;

  /// The datagram used to sendto(), this object is *not* owned by the
  /// UDP_Sender.
  TAO_ECG_UDP_Out_Endpoint *endpoint_;

  /// The MTU for this sender...
  CORBA::ULong mtu_;
};

#if defined(__ACE_INLINE__)
#include "ECG_UDP_Sender.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_ECG_UDP_SENDER_H */
