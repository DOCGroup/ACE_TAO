/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   TAO services
//
// = FILENAME
//   EC_Gateway
//
// = AUTHOR
//   Carlos O'Ryan
//
// = DESCRIPTION
//   This class can be used to connect two event channels; the class
//   connects to a "remote" EC as a consumer, it also connects to the
//   <local> EC as a supplier of events, this later EC is usually
//   collocated.
//   The QoS parameters for both connections must be provided by the
//   user.
//   To avoid infinite loops of events the Gateway descreases the TTL
//   field of the events and will not deliver any events with TTL less
//   than or equal to 0.
//
// = TODO
//   The class makes an extra copy of the events, we need to
//   investigate if closer collaboration with its collocated EC could
//   be used to remove that copy.
//
// ============================================================================

#ifndef TAO_EC_GATEWAY_H
#define TAO_EC_GATEWAY_H

#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/Channel_Clients_T.h"
#include "orbsvcs/orbsvcs_export.h"

class TAO_ORBSVCS_Export TAO_EC_Gateway : public POA_RtecEventChannelAdmin::Observer
{
  // = TITLE
  //   Event Channel Gateway
  //
  // = DESCRIPTION
  //   There are several ways to connect several EC together, for
  //   instance:
  //   + A single class can use normal IIOP and connect to one EC as
  //     a supplier and to another EC as a consumer.
  //   + A class connects as a consumer and transmit the events using
  //     multicast, another class receives the multicast messages and
  //     transform them back into a push() call.
  //
  //   This is an abstract class to represent all the different
  //   strategies for EC distribution.
  //
public:
  TAO_EC_Gateway (void);
  // Default constructor.

  virtual ~TAO_EC_Gateway (void);
  // Destructor

  virtual void close (CORBA::Environment& env) = 0;
  // The gateway must disconnect from all the relevant event channels,
  // or any other communication media (such as multicast groups).

  void observer_handle (RtecEventChannelAdmin::Observer_Handle h);
  RtecEventChannelAdmin::Observer_Handle observer_handle (void) const;
  // Obtain and modify the observer handle.

private:
  RtecEventChannelAdmin::Observer_Handle handle_;
};

// ****************************************************************
class TAO_ORBSVCS_Export TAO_EC_Gateway_IIOP : public TAO_EC_Gateway
//
// = TITLE
//   Event Channel Gateway using IIOP.
//
// = DESCRIPTION
//   This class mediates among two event channels, it connects as a
//   consumer of events with a remote event channel, and as a supplier
//   of events with the local EC.
//   As a consumer it gives a QoS designed to only accept the events
//   in which *local* consumers are interested.
//   Eventually the local EC should create this object and compute its
//   QoS in an automated manner; but this requires some way to filter
//   out the peers registered as consumers, otherwise we will get
//   loops in the QoS graph.
//   It uses exactly the same set of events in the publications list
//   when connected as a supplier.
//
// = NOTES
//   An alternative implementation would be to register with the
//   remote EC as a supplier, and then filter on the remote EC, but
//   one of the objectives is to minimize network traffic.
//   On the other hand the events will be pushed to remote consumers,
//   event though they will be dropped upon receipt (due to the TTL
//   field); IMHO this is another suggestion that the EC needs to know
//   (somehow) which consumers are truly its peers in disguise.
//
//
{
public:
  TAO_EC_Gateway_IIOP (void);
  ~TAO_EC_Gateway_IIOP (void);

  void init (RtecEventChannelAdmin::EventChannel_ptr rmt_ec,
             RtecEventChannelAdmin::EventChannel_ptr lcl_ec,
             RtecScheduler::Scheduler_ptr rmt_sched,
             RtecScheduler::Scheduler_ptr lcl_sched,
             const char* lcl_name,
             const char* rmt_name,
             CORBA::Environment &_env);
  // To do its job this class requires to know the local and remote
  // ECs it will connect to; furthermore it also requires to build
  // RT_Infos for the local and remote schedulers.
  // @@ TODO part of the RT_Info is hardcoded, we need to make it
  // parametric.

  void disconnect_push_supplier (CORBA::Environment &);
  // The channel is disconnecting.

  void disconnect_push_consumer (CORBA::Environment &);
  // The channel is disconnecting.

  void push (const RtecEventComm::EventSet &events,
             CORBA::Environment &);
  // This is the Consumer side behavior, it pushes the events to the
  // local event channel.

  int shutdown (CORBA::Environment&);
  // Disconnect and shutdown the gateway

  // The following methods are documented in the base class.
  virtual void close (CORBA::Environment& _env);
  virtual void update_consumer (const RtecEventChannelAdmin::ConsumerQOS& sub,
                                CORBA::Environment& env);
  virtual void update_supplier (const RtecEventChannelAdmin::SupplierQOS& pub,
                                CORBA::Environment& env);

private:
  void close_i (CORBA::Environment& );

  void update_consumer_i (const RtecEventChannelAdmin::ConsumerQOS& sub,
                          CORBA::Environment& env);

private:
  ACE_SYNCH_MUTEX lock_;
  // Lock to synchronize internal changes

  CORBA::ULong busy_count_;
  // How many threads are running push() we cannot make changes until
  // that reaches 0

  int update_posted_;
  RtecEventChannelAdmin::ConsumerQOS c_qos_;
  // An update_consumer() message arrived *while* we were doing a
  // push() the modification is stored <pub_>, if multiple
  // update_consumer messages arrive only the last one is executed.

  RtecEventChannelAdmin::EventChannel_var rmt_ec_;
  RtecEventChannelAdmin::EventChannel_var lcl_ec_;
  // The remote and the local EC, so we can reconnect when the list changes.

  RtecScheduler::handle_t rmt_info_;
  RtecScheduler::handle_t lcl_info_;
  // Our local and remote RT_Infos.

  ACE_PushConsumer_Adapter<TAO_EC_Gateway_IIOP> consumer_;
  // Our consumer personality....
  int consumer_is_active_;
  // If it is not 0 then we must deactivate the supplier

  ACE_PushSupplier_Adapter<TAO_EC_Gateway_IIOP> supplier_;
  // Our supplier personality....
  int supplier_is_active_;
  // If it is not 0 then we must deactivate the supplier

  // We use a different Consumer_Proxy
  typedef ACE_Map_Manager<RtecEventComm::EventSourceID,RtecEventChannelAdmin::ProxyPushConsumer_ptr,ACE_Null_Mutex> Consumer_Map;
  typedef ACE_Map_Iterator<RtecEventComm::EventSourceID,RtecEventChannelAdmin::ProxyPushConsumer_ptr,ACE_Null_Mutex> Consumer_Map_Iterator;

  Consumer_Map consumer_proxy_map_;
  RtecEventChannelAdmin::ProxyPushConsumer_var default_consumer_proxy_;
  // We talk to the EC (as a supplier) using either an per-supplier
  // proxy or a generic proxy for the type only subscriptions.

  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
  // We talk to the EC (as a consumer) using this proxy.
};

#endif /* ACE_EC_GATEWAY_H */
