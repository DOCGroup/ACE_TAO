/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   TAO services
//
// = FILENAME
//   EC_Gateway_UDP
//
// = AUTHOR
//   Carlos O'Ryan
//
// = DESCRIPTION
//   Define helper classes to propagate events between ECs using
//   either UDP or multicast.
//   The architecture is a bit complicated and deserves some
//   explanation: sending the events over UDP (or mcast) is easy, a
//   Consumer (TAO_ECG_UDP_Sender) subscribes for a certain set of
//   events, its push() method marshalls the event set into a CDR
//   stream that is sent using an ACE_SOCK_CODgram. The subscription
//   set and IP address can be configured.
//   Another helper class (TAO_ECG_UDP_Receiver) acts as a supplier of
//   events; it receives a callback when an event is available on an
//   ACE_SOCK_Dgram, it demarshalls the event and pushes it to the
//   EC.  Two ACE_Event_Handler classes are provided that can forward
//   the events to this Supplier: TAO_ECG_Mcast_EH can receive events
//   from a multicast group; TAO_ECG_UDP_EH can receive events from a
//   regular UDP socket.
//
//   Matching of the events types carried by a multicast group (or IP
//   address) is up to the application. Gateway classes can be
//   implemented to automate this: the EC informs its gateways about
//   local changes in the subscriptions (for example), the Gateway
//   could then consult an administrative server that will inform it
//   which multicast groups carry those event types, it can then
//   create the proper event handlers and TAO_ECG_Receivers. An
//   analogous class can be implemented for the Supplier side.
//
//   An alternative approach would be to look the current set of
//   multicast groups and the events carried on each, using that
//   information a suitable TAO_ECG_UDP_Receiver can be configured
//   (and of course the Senders on the client side).
//
// = TODO
//   The class makes an extra copy of the events, we need to
//   investigate if closer collaboration with its collocated EC could
//   be used to remove that copy.
//
// ============================================================================

#ifndef TAO_EC_GATEWAY_UDP_H
#define TAO_EC_GATEWAY_UDP_H

#include "ace/SOCK_CODgram.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/orbsvcs_export.h"

class TAO_ORBSVCS_Export TAO_ECG_UDP_Sender : public POA_RtecEventComm::PushConsumer
{
  //
  // = TITLE
  //   Send events received from a "local" EC using UDP.
  //
  // = DESCRIPTION
  //   This class connect as a consumer to an EventChannel
  //   and it sends the events using UDP, the UDP address can be a
  //   normal IP address or it can be a multicast group.
  //   It marshalls the events using TAO CDR classes.
  //   No provisions are taken for message fragmentation.
  //
public:
  TAO_ECG_UDP_Sender (void);

  int get_local_addr (ACE_INET_Addr& addr);
  // Get the local endpoint used to send the events.

  void init (RtecEventChannelAdmin::EventChannel_ptr lcl_ec,
             RtecScheduler::Scheduler_ptr lcl_sched,
             const char* lcl_name,
             const ACE_INET_Addr& ipaddr,
             CORBA::Environment &_env);
  // To do its job this class requires to know the local EC it will
  // connect to; it also requires to build an RT_Info for the local
  // scheduler.
  // It only keeps a copy of its SupplierProxy, used for later
  // connection and disconnections.
  // @@ TODO part of the RT_Info is hardcoded, we need to make it
  // parametric.

  void shutdown (CORBA::Environment&);
  // Disconnect and shutdown the sender, no further connections will
  // work unless init() is called again.

  void open (RtecEventChannelAdmin::ConsumerQOS& sub,
             CORBA::Environment& env);
  // Connect (or reconnect) to the EC with the given subscriptions.

  void close (CORBA::Environment& _env);
  // Disconnect from the EC, but reconnection is still possible.

  virtual void disconnect_push_consumer (CORBA::Environment &);
  virtual void push (const RtecEventComm::EventSet &events,
                     CORBA::Environment &);
  // The PushConsumer methods.

private:
  RtecEventChannelAdmin::EventChannel_var lcl_ec_;
  // The remote and the local EC, so we can reconnect when the
  // subscription list changes.

  RtecScheduler::handle_t lcl_info_;
  // Our local and remote RT_Infos.

  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // We talk to the EC (as a consumer) using this proxy.

  ACE_SOCK_CODgram dgram_;
  // The datagram used to send the data.
};

class TAO_ORBSVCS_Export TAO_ECG_UDP_Receiver : public POA_RtecEventComm::PushSupplier
{
  //
  // = TITLE
  //   Decodes events from an ACE_SOCK_Dgram and pushes them to the
  //   Event_Channel.
  //
  // = DESCRIPTION
  //   This supplier receives events from an ACE_SOCK_Dgram, either
  //   from a UDP socket or a Mcast group, decodes them and push them
  //   to the EC.
  //   No provisions are taken for message reassembly.
  //
public:
  TAO_ECG_UDP_Receiver (void);

  void init (RtecEventChannelAdmin::EventChannel_ptr lcl_ec,
             RtecScheduler::Scheduler_ptr lcl_sched,
             const char* lcl_name,
             const ACE_INET_Addr& ignore_from,
             CORBA::Environment &_env);
  // To do its job this class requires to know the local EC it will
  // connect to; it also requires to build an RT_Info for the local
  // scheduler.
  // @@ TODO part of the RT_Info is hardcoded, we need to make it
  // parametric.

  void shutdown (CORBA::Environment&);
  // Disconnect and shutdown the gateway, no further connectsions

  void open (RtecEventChannelAdmin::SupplierQOS& pub,
             CORBA::Environment &env);
  // Connect to the EC using the given publications lists.

  virtual void close (CORBA::Environment& env);
  // Disconnect to the EC.

  int handle_input (ACE_SOCK_Dgram& dgram);
  // The Event_Handlers call this method when data is available at the
  // socket, the <dgram> must be ready for reading and contain a full
  // event.

  // The PushSupplier method.
  virtual void disconnect_push_supplier (CORBA::Environment &);

private:
  RtecEventChannelAdmin::EventChannel_var lcl_ec_;
  // The remote and the local EC, so we can reconnect when the list changes.

  RtecScheduler::handle_t lcl_info_;
  // Our RT_Info.

  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_proxy_;
  // We talk to the EC (as a consumer) using this proxy.

  ACE_INET_Addr ignore_from_;
  // Ignore any events coming from this IP addres.
};

class TAO_ORBSVCS_Export TAO_ECG_UDP_EH : public ACE_Event_Handler
{
  //
  // = TITLE
  //   Event Handler for UDP messages.
  //
  // = DESCRIPTION
  //   This object receives callbacks from the Reactor when data is
  //   available on a UDP socket, it forwards to the ECG_UDP_Receiver
  //   which reads the events and transform it into an event.
public:
  TAO_ECG_UDP_EH (TAO_ECG_UDP_Receiver *recv);

  int open (const ACE_INET_Addr& ipaddr);
  // Open the datagram and register with this->reactor()

  int close (void);
  // Close the datagram and unregister with the reactor.

  // Reactor callbacks
  virtual int handle_input (ACE_HANDLE fd);
  virtual ACE_HANDLE get_handle (void) const;

private:
  ACE_SOCK_Dgram dgram_;
  // The datagram used to receive the data.

  TAO_ECG_UDP_Receiver* receiver_;
  // We callback to this object when a message arrives.
};

class TAO_ORBSVCS_Export TAO_ECG_Mcast_EH : public ACE_Event_Handler
{
  //
  // = TITLE
  //   Event Handler for UDP messages.
  //
  // = DESCRIPTION
  //   This object receives callbacks from the Reactor when data is
  //   available on the mcast socket, it forwards to the UDP_Receive
  //   gateway which reads the events and transform it into an event.
  //
public:
  TAO_ECG_Mcast_EH (TAO_ECG_UDP_Receiver *recv);

  int open (const ACE_INET_Addr& mcast_group);
  // Open the datagram (join the mcast group) and register with
  // this->reactor()

  int close (void);
  // Close the datagram (leave the mcast group) and unregister with
  // the reactor.

  // Reactor callbacks
  virtual int handle_input (ACE_HANDLE fd);
  virtual ACE_HANDLE get_handle (void) const;

private:
  ACE_SOCK_Dgram_Mcast dgram_;
  // The datagram used to receive the data.

  TAO_ECG_UDP_Receiver* receiver_;
  // We callback to this object when a message arrives.
};


#endif /* ACE_EVENT_CHANNEL_UDP_H */
