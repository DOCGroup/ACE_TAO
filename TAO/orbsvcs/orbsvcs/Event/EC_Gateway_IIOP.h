/* -*- C++ -*- */
/**
 *  @file   EC_Gateway_IIOP.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *  @author Johnny Willemsen  (jwillemsen@remedy.nl)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_EC_GATEWAY_IIOP_H
#define TAO_EC_GATEWAY_IIOP_H
#include /**/ "ace/pre.h"

#include /**/ "event_export.h"
#include "EC_Gateway.h"
#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/Channel_Clients.h"
#include "ace/Map_Manager.h"

class TAO_ECG_ConsumerEC_Control;
class TAO_EC_Gateway_IIOP_Factory;

/**
 * @class TAO_EC_Gateway_IIOP
 *
 * @brief Event Channel Gateway using IIOP.
 *
 * This class mediates among two event channels, it connects as a consumer of
 * events with a remote event channel, and as a supplier of events with the
 * local EC. As a consumer it gives a QoS designed to only accept the events
 * in which *local* consumers are interested. Eventually the local EC should
 * create this object and compute its QoS in an automated manner; but this
 * requires some way to filter out the peers registered as consumers,
 * otherwise we will get loops in the QoS graph.
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
  virtual ~TAO_EC_Gateway_IIOP (void);

  /**
   * To do its job this class requires to know the local and remote ECs it will
   * connect to.
   * @return 0 in case of success, -1 in case of failure
   */
  int init (RtecEventChannelAdmin::EventChannel_ptr supplier_ec,
            RtecEventChannelAdmin::EventChannel_ptr consumer_ec
            ACE_ENV_ARG_DECL);

  /// The channel is disconnecting.
  void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /// The channel is disconnecting.
  void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /// This is the consumer side behavior, it pushes the events to the
  /// local event channel.
  void push (const RtecEventComm::EventSet &events
             ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /// Disconnect and shutdown the gateway
  int shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  // The following methods are documented in the base class.
  virtual void close (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  virtual void update_consumer (const RtecEventChannelAdmin::ConsumerQOS& sub
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void update_supplier (const RtecEventChannelAdmin::SupplierQOS& pub
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  /// Check whether the consumer event channel is non existent or not
  CORBA::Boolean consumer_ec_non_existent (CORBA::Boolean_out disconnected
                                           ACE_ENV_ARG_DECL);

  /**
   * Cleanup all consumer proxies we have without trying to tell the
   * consumer that we are going to disconnect. This can be used to cleanup
   * the consumer proxy administration in case we know that the consumers
   * are all unreachable.
   */
  void cleanup_consumer_proxies (ACE_ENV_SINGLE_ARG_DECL);

private:
  void close_i (ACE_ENV_SINGLE_ARG_DECL);

  void disconnect_supplier_proxy_i (ACE_ENV_SINGLE_ARG_DECL);

  void disconnect_consumer_proxies_i (ACE_ENV_SINGLE_ARG_DECL);

  void cleanup_consumer_proxies_i (ACE_ENV_SINGLE_ARG_DECL);

  void update_consumer_i (const RtecEventChannelAdmin::ConsumerQOS& sub
                          ACE_ENV_ARG_DECL);

  CORBA::Boolean is_consumer_ec_connected_i (void) const;

  void push_to_consumer (RtecEventChannelAdmin::ProxyPushConsumer_ptr consumer,
                         const RtecEventComm::EventSet& event ACE_ENV_ARG_DECL);

protected:
  /// Do the real work in init()
  int init_i (RtecEventChannelAdmin::EventChannel_ptr supplier_ec,
              RtecEventChannelAdmin::EventChannel_ptr consumer_ec
              ACE_ENV_ARG_DECL);

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

  /// The event channel acting as supplier for this gateway so we can reconnect
  /// when the list changes.
  RtecEventChannelAdmin::EventChannel_var supplier_ec_;

  /// The event channel acting as consumer of this gateway
  RtecEventChannelAdmin::EventChannel_var consumer_ec_;

  /// Our RT_Infos for the event channel that is the supplier.
  RtecBase::handle_t supplier_info_;
  /// Our RT_Infos for the event channel that is the consumer.
  RtecBase::handle_t consumer_info_;

  /// Our consumer personality....
  ACE_PushConsumer_Adapter<TAO_EC_Gateway_IIOP> consumer_;

  /// If it is not 0 then we must deactivate the consumer
  int consumer_is_active_;

  /// Our supplier personality....
  ACE_PushSupplier_Adapter<TAO_EC_Gateway_IIOP> supplier_;

  /// If it is not 0 then we must deactivate the supplier
  int supplier_is_active_;

  // We use a different Consumer_Proxy
  typedef ACE_Map_Manager<RtecEventComm::EventSourceID,RtecEventChannelAdmin::ProxyPushConsumer_ptr,ACE_Null_Mutex> Consumer_Map;
  typedef ACE_Map_Iterator<RtecEventComm::EventSourceID,RtecEventChannelAdmin::ProxyPushConsumer_ptr,ACE_Null_Mutex> Consumer_Map_Iterator;

  /// We talk to the EC (as a supplier) using either an per-supplier
  /// proxy or a generic proxy for the type only subscriptions. We push the
  /// events to these proxies
  Consumer_Map consumer_proxy_map_;
  RtecEventChannelAdmin::ProxyPushConsumer_var default_consumer_proxy_;

  /// We talk to the EC (as a consumer) using this proxy. We receive the events
  /// from these proxy
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;

  TAO_ECG_ConsumerEC_Control* ec_control_;

  TAO_EC_Gateway_IIOP_Factory* factory_;
};

#include /**/ "ace/post.h"
#endif /* ACE_EC_GATEWAY_IIOP_H */
