// -*- C++ -*-

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
#include "ace/Null_Mutex.h"

class TAO_ECG_ConsumerEC_Control;
class TAO_EC_Gateway_IIOP_Factory;

/**
 * @class TAO_EC_Gateway_IIOP
 *
 * @brief Event Channel Gateway using IIOP.
 *
 * This class mediates among two event channels. It connects as a
 * consumer of events with a remote event channel and as a supplier of
 * events with the local EC. As a consumer it gives a QoS designed to
 * only accept the events in which *local* consumers are
 * interested. Eventually the local EC should create this object and
 * compute its QoS in an automated manner but this requires some way
 * to filter out the peers registered as consumers otherwise we will
 * get loops in the QoS graph.  It uses exactly the same set of events
 * in the publications list when connected as a supplier.
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
  /**
   * @brief Constructor
   */
  TAO_EC_Gateway_IIOP (void);

  /**
   * @brief Destructor
   */
  virtual ~TAO_EC_Gateway_IIOP (void);

  /**
   * @brief Wrapper function to initialize the class with a pointer to
   * the EC supplying events to the Gateway and a pointer to the EC
   * consuming events from the Gateway.
   * @param supplier_ec Reference to the EC supplying events
   * @param consumer_ec Reference to the EC consuming events
   * @return 0 in case of success, -1 in case of failure
   */
  int init (RtecEventChannelAdmin::EventChannel_ptr supplier_ec,
            RtecEventChannelAdmin::EventChannel_ptr consumer_ec
            ACE_ENV_ARG_DECL);

  /**
   * @brief Perform cleanup of gateway when disconnecting from the
   * PushSupplierProxy.
   *
   * This function is called by the PushSupplierAdapter when the event
   * channel is disconnected. This is here because it is required by
   * the adapter but there is no need to perform any cleanup of the
   * gateway when disconnecting.
   */
  void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /**
   * @brief Perform clean of gateway when disconnecting from the
   * PushSupplierConsumer.
   *
   * This function is called by the PushConsumerAdapter when the
   * event channel is disconnected. This is here because it is required by
   * the adapter but there is no need to perform any cleanup of the
   * gateway when disconnecting.
   */
  void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /**
   * @brief Push event to the consumer channel.
   *
   * This function is called by the PushConsumerAdapter when receiving
   * an events from the event channel. This is the consumer side
   * behavior. This function pushes the events to the consumer channel.
   *
   * @param events Set of events received from the PushConsumerAdapter
   */
  void push (const RtecEventComm::EventSet &events
             ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  /// Disconnect and shutdown the gateway
  int shutdown (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);


  /**
   * @brief Disconnect gateway from all ECs.
   *
   * The gateway must disconnect from all the relevant event channels,
   * or any other communication media (such as multicast groups).
   */
  virtual void close (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  /**
   * @brief Update the list of consumers wanting events from the
   * gateway.
   *
   * The set of the consumer subscriptions is sent to the event
   * channel. Defined in RtecEventChannelAdmin.idl as a part of the
   * Observer interface.
   *
   * @param sub New list of consumer subscriptions
   */
  virtual void update_consumer (const RtecEventChannelAdmin::ConsumerQOS& sub
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * @brief The list of all the event publications is passed to the
   * event channel. Defined in RtecEventChannelAdmin.idl as a part of
   * the Observer interface.
   *
   * @param pub New list of supplier publications
   */
  virtual void update_supplier (const RtecEventChannelAdmin::SupplierQOS& pub
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  /// Let the gateway reconnect itself to the consumer ec given exisiting QoS
  void reconnect_consumer_ec(ACE_ENV_SINGLE_ARG_DECL);

  /**
   * @brief Check whether the consumer event channel is non existent or not.
   *
   * @param disconnected Boolean value result of check to determine if the
   * consumer EC is connected or not.
   *
   * @return If the TAO setup is not minimum corba then the boolean
   * value returned is the result of the CORBA call on the consumer EC
   * object to determine if it exists or not. Otherwise the return
   * value is always true (0).
   */
  CORBA::Boolean consumer_ec_non_existent (CORBA::Boolean_out disconnected
                                           ACE_ENV_ARG_DECL);

  /**
   * @brief Cleanup all consumer proxies without disconnecting.
   *
   * Cleanup all consumer proxies we have without trying to tell the
   * consumer that we are going to disconnect. This can be used to
   * cleanup the consumer proxy administration in case we know that
   * the consumers are all unreachable.
   */
  void cleanup_consumer_proxies (ACE_ENV_SINGLE_ARG_DECL);

  /**
   * @brief Cleanup all connections to the consumer EC.
   *
   * Cleanup the connection to the consumer event channel. This is a
   * wrapper function which locks the mutex and calls
   * cleanup_consumer_ec_i to perform the real work.
   *
   * @return Return false (0) on exit.
   */
  int cleanup_consumer_ec (void);

  /**
   * @brief Cleanup all connections to suppler EC.
   *
   * Cleanup the connection to the supplier event channel. This is a
   * wrapper function which locks the mutex and calls
   * cleanup_supplier_ec_i to perform the real work.
   *
   * @return Return false (0) on exit.
   */
  int cleanup_supplier_ec (void);

  /**
   * @brief Temporarily suspend the connection to the supplier event channel
   * to stop receiving events.
   */
  void suspend_supplier_ec (ACE_ENV_SINGLE_ARG_DECL);

  /**
   * @brief Resume the connection to the supplier ec to start receiving
   * events.
   */
  void resume_supplier_ec (ACE_ENV_SINGLE_ARG_DECL);

private:

  /**
   * @brief The implementation of close().
   */
  void close_i (ACE_ENV_SINGLE_ARG_DECL);

  /**
   * @brief This function is called by close_i() to disconnect all supplier
   * proxies from the gateway.
   */
  void disconnect_supplier_proxy_i (ACE_ENV_SINGLE_ARG_DECL);

  /**
   * @brief This function is called by close_i() to disconnect all consumer
   * proxies from the gateway.
   */
  void disconnect_consumer_proxies_i (ACE_ENV_SINGLE_ARG_DECL);

  /**
   * @brief The implementation of cleanup_consumer_proxies().
   */
  void cleanup_consumer_proxies_i (ACE_ENV_SINGLE_ARG_DECL);

  /**
   * @brief The implementation of update_consumer().
   *
   * @param sub New list of consumer subscriptions passed from update_consumer().
   */
  void update_consumer_i (const RtecEventChannelAdmin::ConsumerQOS& sub
                          ACE_ENV_ARG_DECL);

  /**
   * @brief Create all connections to consumer ec and to supplier ec. This is
   * function is called by update_consumer_i().
   *
   * @param sub The consumer QoS information associated with the connections.
   */
  void open_i (const RtecEventChannelAdmin::ConsumerQOS& sub
               ACE_ENV_ARG_DECL);

  /// Helper method to see if consumer ec is connected
  CORBA::Boolean is_consumer_ec_connected_i (void) const;

  /**
   * @brief Push the @a event to the @a consumer.
   *
   * @param consumer Pointer to the consumer proxy which will receive the event
   * @param event The event that is sent.
   */
  void push_to_consumer (RtecEventChannelAdmin::ProxyPushConsumer_ptr consumer,
                         const RtecEventComm::EventSet& event ACE_ENV_ARG_DECL);

  /**
   * @brief Set the consumer event channel to NIL
   */
  void cleanup_consumer_ec_i (void);

  /**
   * @brief Set the supplier event channel to NIL
   */
  void cleanup_supplier_ec_i (void);

protected:
  /**
   * @brief Initialize the class with a pointer to the EC supplying
   * events to the Gateway and a pointer to the EC consuming events
   * from the Gateway.
   *
   * @param supplier_ec Reference to the EC supplying events
   * @param consumer_ec Reference to the EC consuming events
   * @return 0 in case of success, -1 in case of failure
   */
  int init_i (RtecEventChannelAdmin::EventChannel_ptr supplier_ec,
              RtecEventChannelAdmin::EventChannel_ptr consumer_ec
              ACE_ENV_ARG_DECL);

protected:
  /// Lock to synchronize internal changes
  TAO_SYNCH_MUTEX lock_;

  /**
   * @brief How many threads are running push() we cannot make changes until
   * that reaches 0.
   */
  CORBA::ULong busy_count_;

  /**
   * @brief An update_consumer() message arrived *while* we were doing a
   * push() the modification is stored, if multiple update_consumer messages
   * arrive only the last one is executed.
   */
  int update_posted_;

  /**
   * @brief Consumer QoS Information
   */
  RtecEventChannelAdmin::ConsumerQOS c_qos_;

  /**
   * @brief We have a cleanup outstanding and must wait doing cleanup until all pushes
   * are ready.
   */
  int cleanup_posted_;

  /**
   * @brief Is the supplier ec suspended?
   */
  int supplier_ec_suspended_;

  /**
   * @brief The event channel acting as supplier for this gateway so we can reconnect
   * when the list changes.
   */
  RtecEventChannelAdmin::EventChannel_var supplier_ec_;

  /// The event channel acting as consumer of this gateway
  RtecEventChannelAdmin::EventChannel_var consumer_ec_;

  /// Our RT_Infos for the event channel that is the supplier.
  RtecBase::handle_t supplier_info_;

  /// Our RT_Infos for the event channel that is the consumer.
  RtecBase::handle_t consumer_info_;

  /**
   * @brief This adapter inherits from PushConsumer and forwards
   * all PushConsumer calls to the Gateway.
   *
   * Our consumer personality....
   *
   */
  ACE_PushConsumer_Adapter<TAO_EC_Gateway_IIOP> consumer_;

  /// If it is not 0 then we must deactivate the consumer
  int consumer_is_active_;

  /// Our supplier personality....
  ACE_PushSupplier_Adapter<TAO_EC_Gateway_IIOP> supplier_;

  /// If it is not 0 then we must deactivate the supplier
  int supplier_is_active_;

  /**
   * @typedef Consumer_Map
   */
  typedef ACE_Map_Manager<RtecEventComm::EventSourceID,
                          RtecEventChannelAdmin::ProxyPushConsumer_ptr,
                          ACE_Null_Mutex> Consumer_Map;
  /**
   * @typedef Consumer_Map_Iterator
   */
  typedef ACE_Map_Iterator<RtecEventComm::EventSourceID,
                           RtecEventChannelAdmin::ProxyPushConsumer_ptr,
                           ACE_Null_Mutex> Consumer_Map_Iterator;

  /**
   * @brief We talk to the EC (as a supplier) using either an
   * per-supplier proxy or a generic proxy for the type only
   * subscriptions. We push the events to these proxies
   */
  Consumer_Map consumer_proxy_map_;
  RtecEventChannelAdmin::ProxyPushConsumer_var default_consumer_proxy_;

  /**
   * @brief We talk to the EC (as a consumer) using this proxy. We
   * receive the events from these proxy
   */
  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_proxy_;

  /**
   * @brief The consumer ec control which controls the behaviour in
   * case of a misbehaving consumer ec.
   */
  TAO_ECG_ConsumerEC_Control* ec_control_;

  /// The Gateway IIOP Factory for all the settings
  TAO_EC_Gateway_IIOP_Factory* factory_;

  /// If 1, we use the TTL flags, if 0, we just ignore TTL
  int use_ttl_;

  /**
   * @brief The flag for using the consumer proxy map. With 1 the consumer proxy map
   * is used, meaning that for each unique source id we use a different
   * proxy push consumer, if 0, we only use one proxy push consumer (the
   * default) for all source ids.
   */
  int use_consumer_proxy_map_;

};

#include /**/ "ace/post.h"

#endif /* ACE_EC_GATEWAY_IIOP_H */
