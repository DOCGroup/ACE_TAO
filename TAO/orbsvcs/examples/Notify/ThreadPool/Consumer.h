/* -*- C++ -*- */
/**
 *  @file Consumer.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_CONSUMER_H
#define TAO_Notify_CONSUMER_H

#include /**/ "ace/pre.h"

#include "ORB_Objects.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "ace/OS_NS_time.h"

/**
 * @class TAO_Notify_ThreadPool_Consumer
 *
 * @brief Consumer
 */

class TAO_Notify_ThreadPool_Consumer
  : public POA_CosNotifyComm::StructuredPushConsumer
{
public:
  explicit TAO_Notify_ThreadPool_Consumer (TAO_Notify_ORB_Objects& orb_objects);

  TAO_Notify_ThreadPool_Consumer (TAO_Notify_ThreadPool_Consumer const&) = delete;
  TAO_Notify_ThreadPool_Consumer (TAO_Notify_ThreadPool_Consumer&&) = delete;
  TAO_Notify_ThreadPool_Consumer& operator= (TAO_Notify_ThreadPool_Consumer const&) = delete;
  TAO_Notify_ThreadPool_Consumer& operator= (TAO_Notify_ThreadPool_Consumer&&) = delete;

  /// Init
  void init (PortableServer::POA_ptr poa,
             CosNotifyChannelAdmin::ConsumerAdmin_ptr admin,
             int proxy_supplier_thread_count,
             int max_events,
             long delay);

  /// Print the consumer throughput
  void dump_throughput ();

protected:
  // = Methods
  ~TAO_Notify_ThreadPool_Consumer () override = default;

  /// Connect the Consumer to the EventChannel.
  /// Creates a new proxy supplier and connects to it.
  void connect ();

  /// Disconnect the supplier.
  void disconnect ();

  /// Deactivate.
  void deactivate ();

  // = ServantBase operations
  PortableServer::POA_ptr _default_POA () override;

  // = NotifyPublish method
  void offer_change (CosNotification::EventTypeSeq const& added,
                     CosNotification::EventTypeSeq const& removed) override;

  // = StructuredPushConsumer methods
  void push_structured_event (CosNotification::StructuredEvent const& notification) override;

  void disconnect_structured_push_consumer () override;

  // = Data members

  /// Lock
  TAO_SYNCH_MUTEX lock_;

  /// ORB Objects.
  TAO_Notify_ORB_Objects& orb_objects_;

  // POA.
  PortableServer::POA_var default_POA_;

  /// The proxy that we are connected to.
  CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxy_supplier_;

  /// The proxy_supplier id.
  CosNotifyChannelAdmin::ProxyID proxy_supplier_id_ {};

  // The Consumer Admin
  CosNotifyChannelAdmin::ConsumerAdmin_var admin_;

  /// ProxySupplier thread count.
  int proxy_supplier_thread_count_ {};

  /// Max events to receive
  int max_events_ {10};

  /// Count the number of events received.
  int events_received_count_ {};

  /// Time when the first sample was received.
  //ACE_UINT64 t_first_;
  ACE_hrtime_t t_first_ {};

  /// Time when the last sample was received.
  //ACE_UINT64 t_last_;
  ACE_hrtime_t t_last_ {};

  /// Delay: Sec of wait in each push.
  ACE_Time_Value delay_ {};
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_CONSUMER_H */
