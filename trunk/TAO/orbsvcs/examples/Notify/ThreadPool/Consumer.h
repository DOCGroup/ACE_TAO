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

#include "ace/pre.h"

#include "ORB_Objects.h"
#include "tao/RTCORBA/RTCORBA.h"
#include "orbsvcs/orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/orbsvcs/CosNotifyCommC.h"

/**
 * @class TAO_Notify_Consumer
 *
 * @brief Consumer
 *
 */

class TAO_Notify_Consumer : public POA_CosNotifyComm::StructuredPushConsumer, public PortableServer::RefCountServantBase
{
public:
  /// Constuctor
  TAO_Notify_Consumer (TAO_Notify_ORB_Objects& orb_objects);

  /// Init
  void init (PortableServer::POA_var& poa, CosNotifyChannelAdmin::ConsumerAdmin_var& admin, int proxy_supplier_thread_count, int max_events, long delay ACE_ENV_ARG_DECL);

  /// Run
  void run (ACE_ENV_SINGLE_ARG_DECL_NOT_USED);

  /// Print the consumer throughput
  void dump_throughput (void);

protected:
  // = Methods
  /// Destructor
  virtual ~TAO_Notify_Consumer (void);

  /// Connect the Consumer to the EventChannel.
  /// Creates a new proxy supplier and connects to it.
  void connect (ACE_ENV_SINGLE_ARG_DECL);

  /// Disconnect the supplier.
  void disconnect (ACE_ENV_SINGLE_ARG_DECL);

  /// Deactivate.
  void deactivate (ACE_ENV_SINGLE_ARG_DECL);

  // = ServantBase operations
  virtual PortableServer::POA_ptr _default_POA (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  // = NotifyPublish method
  virtual void offer_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ));

  // = StructuredPushSupplier methods
  virtual void push_structured_event (
        const CosNotification::StructuredEvent & notification
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventComm::Disconnected
       ));

  virtual void disconnect_structured_push_consumer (
        ACE_ENV_SINGLE_ARG_DECL
        )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

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

#include "ace/post.h"
#endif /* TAO_Notify_CONSUMER_H */
