/* -*- C++ -*- */
/**
 *  @file Consumer.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_CONSUMER_H
#define TAO_Notify_CONSUMER_H

#include /**/ "ace/pre.h"

#include "ORB_Objects.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "ace/SString.h"
#include "ace/OS_NS_time.h"

/**
 * @class TAO_Notify_ThreadPool_Consumer
 *
 * @brief Consumer
 *
 */

class TAO_Notify_ThreadPool_Consumer
  : public POA_CosNotifyComm::StructuredPushConsumer
{
public:
  /// Constuctor
  TAO_Notify_ThreadPool_Consumer (TAO_Notify_ORB_Objects& orb_objects);

  /// Init
  void init (PortableServer::POA_var& poa, CosNotifyChannelAdmin::ConsumerAdmin_var& admin, int proxy_supplier_thread_count, int max_events, long delay);

  /// Run
  void run (void);

  /// Print the consumer throughput
  void dump_throughput (void);

protected:
  // = Methods
  /// Destructor
  virtual ~TAO_Notify_ThreadPool_Consumer (void);

  /// Connect the Consumer to the EventChannel.
  /// Creates a new proxy supplier and connects to it.
  void connect (void);

  /// Disconnect the supplier.
  void disconnect (void);

  /// Deactivate.
  void deactivate (void);

  // = ServantBase operations
  virtual PortableServer::POA_ptr _default_POA (void);

  // = NotifyPublish method
  virtual void offer_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
      );

  // = StructuredPushSupplier methods
  virtual void push_structured_event (
        const CosNotification::StructuredEvent & notification
      );

  virtual void disconnect_structured_push_consumer (
        );

  // = Data members

  /// Lock
  TAO_SYNCH_MUTEX lock_;

  /// ORB Objects.
  TAO_Notify_ORB_Objects orb_objects_;

  // POA.
  PortableServer::POA_var default_POA_;

  /// The proxy that we are connected to.
  CosNotifyChannelAdmin::StructuredProxyPushSupplier_var proxy_supplier_;

  /// The proxy_supplier id.
  CosNotifyChannelAdmin::ProxyID proxy_supplier_id_;

  // The Consumer Admin
  CosNotifyChannelAdmin::ConsumerAdmin_var admin_;

  /// The Type the Consumer should subscribe to.
  ACE_CString event_type_;

  /// ProxySupplier thread count.
  int proxy_supplier_thread_count_;

  /// Max events to receive
  int max_events_;

  /// Count the number of events received.
  int events_received_count_;

  /// Time when the first sample was received.
  //ACE_UINT64 t_first_;
  ACE_hrtime_t t_first_;

  /// Time when the last sample was received.
  //ACE_UINT64 t_last_;
  ACE_hrtime_t t_last_;

  /// Delay: Sec of wait in each push.
  ACE_Time_Value delay_;
};

#include /**/ "ace/post.h"
#endif /* TAO_Notify_CONSUMER_H */
