//=============================================================================
/**
 *  @file   Notify_SequenceProxyPushSupplier_i.h
 *
 *  $Id$
 *
 * Implements the POA_CosNotifyChannelAdmin::SequenceProxyPushSupplier
 * interface.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_NOTIFY_SEQUENCEPROXYPUSHSUPPLIER_I_H
#define TAO_NOTIFY_SEQUENCEPROXYPUSHSUPPLIER_I_H
#include "ace/pre.h"

#include "Notify_ProxySupplier_T.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"

class TAO_Notify_ConsumerAdmin_i;

#if defined (_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning (push)
#endif /* _MSC_VER >= 1200 */
#pragma warning (disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_Notify_SequenceProxyPushSupplier_i
 *
 * @brief Implements CosNotifyChannelAdmin::SequenceProxyPushSupplier.
 */
class TAO_Notify_Export TAO_Notify_SequenceProxyPushSupplier_i : public TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::SequenceProxyPushSupplier>,
                                                                 public ACE_Event_Handler
{

public:
  /// Constructor
  TAO_Notify_SequenceProxyPushSupplier_i (TAO_Notify_ConsumerAdmin_i* consumer_admin);

  /// Destructor
  virtual ~TAO_Notify_SequenceProxyPushSupplier_i (void);

  virtual void connect_sequence_push_consumer (
    CosNotifyComm::SequencePushConsumer_ptr push_consumer
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventChannelAdmin::AlreadyConnected,
    CosEventChannelAdmin::TypeError
  ));

  // @@ Pradeep: more indentation problems....
  virtual void disconnect_sequence_push_supplier (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));


  /// Shutdown.
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL);

protected:
  /// Deliver the event to the consumer.
  virtual void dispatch_event_i (TAO_Notify_Event &event ACE_ENV_ARG_DECL);

  /// Deliver the update to the consumer.
  virtual void dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed ACE_ENV_ARG_DECL);

  /// Do what we need to do after the qos policies are set.
  void setup_qos_policies (ACE_ENV_SINGLE_ARG_DECL);

  /// When the pacing interval is used, handle_timeout () is called by
  /// the reactor.
  virtual int handle_timeout (const ACE_Time_Value& current_time,
                              const void* act = 0);

  /// The consumer that we're connect to.
  CosNotifyComm::SequencePushConsumer_var push_consumer_;

private:
  typedef TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::SequenceProxyPushSupplier>
    proxy_inherited;

  /// Cache events until maximum batch size is reached.
  CosNotification::EventBatch event_cache_;

  /// Flag to specify that dispatch_event was called from the timeout
  CORBA::Boolean from_timeout_;

  /// Flag to specify that dispatch_event_i was successfully called from
  /// the timeout handler
  CORBA::Boolean reset_timeout_flag_;

  /// Lock to synchronize the modification of event_cache_
  ACE_Lock* batch_lock_;
};

#if defined (_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning (pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_SEQUENCEPROXYPUSHSUPPLIER_I_H */
