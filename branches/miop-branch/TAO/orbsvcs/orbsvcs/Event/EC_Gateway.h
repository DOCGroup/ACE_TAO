/* -*- C++ -*- */
/**
 *  @file   EC_Gateway.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_EC_GATEWAY_H
#define TAO_EC_GATEWAY_H
#include "ace/pre.h"

#include "orbsvcs/Event/event_export.h"
#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/Channel_Clients.h"
#include "ace/Map_Manager.h"

/**
 * @class TAO_EC_Gateway
 *
 * @brief Event Channel Gateway
 *
 * There are several ways to connect several EC together, for
 * instance:
 * + A single class can use normal IIOP and connect to one EC as
 * a supplier and to another EC as a consumer.
 * + A class connects as a consumer and transmit the events using
 * multicast, another class receives the multicast messages and
 * transform them back into a push() call.
 * This is an abstract class to represent all the different
 * strategies for EC distribution.
 *
 */
class TAO_RTEvent_Export TAO_EC_Gateway : public POA_RtecEventChannelAdmin::Observer
{
public:
  /// Default constructor.
  TAO_EC_Gateway (void);

  /// Destructor
  virtual ~TAO_EC_Gateway (void);

  /// The gateway must disconnect from all the relevant event channels,
  /// or any other communication media (such as multicast groups).
  virtual void close (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS) = 0;

  /// Obtain and modify the observer handle.
  void observer_handle (RtecEventChannelAdmin::Observer_Handle h);
  RtecEventChannelAdmin::Observer_Handle observer_handle (void) const;

private:
  RtecEventChannelAdmin::Observer_Handle handle_;
};

// ****************************************************************
/**
 * @class TAO_EC_Gateway_IIOP
 *
 * @brief Event Channel Gateway using IIOP.
 *
 * This class mediates among two event channels, it connects as a
 * consumer of events with a remote event channel, and as a supplier
 * of events with the local EC.
 * As a consumer it gives a QoS designed to only accept the events
 * in which *local* consumers are interested.
 * Eventually the local EC should create this object and compute its
 * QoS in an automated manner; but this requires some way to filter
 * out the peers registered as consumers, otherwise we will get
 * loops in the QoS graph.
 * It uses exactly the same set of events in the publications list
 * when connected as a supplier.
 *
 * @note
 * An alternative implementation would be to register with the
 * remote EC as a supplier, and then filter on the remote EC, but
 * one of the objectives is to minimize network traffic.
 * On the other hand the events will be pushed to remote consumers,
 * event though they will be dropped upon receipt (due to the TTL
 * field); IMHO this is another suggestion that the EC needs to know
 * (somehow) which consumers are truly its peers in disguise.
 *
 * @todo: The class makes an extra copy of the events, we need to
 * investigate if closer collaboration with its collocated EC could
 * be used to remove that copy.
 */
class TAO_RTEvent_Export TAO_EC_Gateway_IIOP : public TAO_EC_Gateway
{
public:
  TAO_EC_Gateway_IIOP (void);
  ~TAO_EC_Gateway_IIOP (void);

  /// To do its job this class requires to know the local and remote
  /// ECs it will connect to,
  void init (RtecEventChannelAdmin::EventChannel_ptr rmt_ec,
             RtecEventChannelAdmin::EventChannel_ptr lcl_ec
             TAO_ENV_ARG_DECL);

  /// The channel is disconnecting.
  void disconnect_push_supplier (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /// The channel is disconnecting.
  void disconnect_push_consumer (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /// This is the Consumer side behavior, it pushes the events to the
  /// local event channel.
  void push (const RtecEventComm::EventSet &events
             TAO_ENV_ARG_DECL_WITH_DEFAULTS);

  /// Disconnect and shutdown the gateway
  int shutdown (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  // The following methods are documented in the base class.
  virtual void close (TAO_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  virtual void update_consumer (const RtecEventChannelAdmin::ConsumerQOS& sub
                                TAO_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void update_supplier (const RtecEventChannelAdmin::SupplierQOS& pub
                                TAO_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

private:
  void close_i (TAO_ENV_SINGLE_ARG_DECL_NOT_USED);

  void update_consumer_i (const RtecEventChannelAdmin::ConsumerQOS& sub
                          TAO_ENV_ARG_DECL);

protected:
  /// Do the real work in init()
  void init_i (RtecEventChannelAdmin::EventChannel_ptr rmt_ec,
               RtecEventChannelAdmin::EventChannel_ptr lcl_ec
               TAO_ENV_ARG_DECL);

protected:
  /// Lock to synchronize internal changes
  TAO_SYNCH_MUTEX lock_;

  /// How many threads are running push() we cannot make changes until
  /// that reaches 0
  CORBA::ULong busy_count_;

  /**
   * An update_consumer() message arrived *while* we were doing a
   * push() the modification is stored <pub_>, if multiple
   * update_consumer messages arrive only the last one is executed.
   */
  int update_posted_;
  RtecEventChannelAdmin::ConsumerQOS c_qos_;

  /// The remote and the local EC, so we can reconnect when the list changes.
  RtecEventChannelAdmin::EventChannel_var rmt_ec_;
  RtecEventChannelAdmin::EventChannel_var lcl_ec_;

  /// Our local and remote RT_Infos.
  RtecBase::handle_t rmt_info_;
  RtecBase::handle_t lcl_info_;

  /// Our consumer personality....
  /// If it is not 0 then we must deactivate the supplier
  ACE_PushConsumer_Adapter<TAO_EC_Gateway_IIOP> consumer_;
  int consumer_is_active_;

  /// Our supplier personality....
  /// If it is not 0 then we must deactivate the supplier
  ACE_PushSupplier_Adapter<TAO_EC_Gateway_IIOP> supplier_;
  int supplier_is_active_;

  // We use a different Consumer_Proxy
  typedef ACE_Map_Manager<RtecEventComm::EventSourceID,RtecEventChannelAdmin::ProxyPushConsumer_ptr,ACE_Null_Mutex> Consumer_Map;
  typedef ACE_Map_Iterator<RtecEventComm::EventSourceID,RtecEventChannelAdmin::ProxyPushConsumer_ptr,ACE_Null_Mutex> Consumer_Map_Iterator;

  /// We talk to the EC (as a supplier) using either an per-supplier
  /// proxy or a generic proxy for the type only subscriptions.
  Consumer_Map consumer_proxy_map_;
  RtecEventChannelAdmin::ProxyPushConsumer_var default_consumer_proxy_;

  /// We talk to the EC (as a consumer) using this proxy.
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;
};

#include "ace/post.h"
#endif /* ACE_EC_GATEWAY_H */
