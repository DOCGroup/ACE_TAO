// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_SequenceProxyPushSupplier_i.h
//
// = DESCRIPTION
//   Implements the POA_CosNotifyChannelAdmin::SequenceProxyPushSupplier
//   interface.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================
#ifndef TAO_NOTIFY_SEQUENCEPROXYPUSHSUPPLIER_I_H
#define TAO_NOTIFY_SEQUENCEPROXYPUSHSUPPLIER_I_H
#include "ace/pre.h"

#include "Notify_ProxySupplier_T.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"

class TAO_Notify_ConsumerAdmin_i;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Notify_Export TAO_Notify_SequenceProxyPushSupplier_i : public TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::SequenceProxyPushSupplier>
{
  // = TITLE
  //   TAO_Notify_SequenceProxyPushSupplier_i
  //
  // = DESCRIPTION
  //   implements CosNotifyChannelAdmin::SequenceProxyPushSupplier
  //

 public:
  TAO_Notify_SequenceProxyPushSupplier_i (TAO_Notify_ConsumerAdmin_i* consumer_admin);
  // Constructor

  virtual ~TAO_Notify_SequenceProxyPushSupplier_i (void);
  // Destructor

  virtual void connect_sequence_push_consumer (
    CosNotifyComm::SequencePushConsumer_ptr push_consumer
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventChannelAdmin::AlreadyConnected,
    CosEventChannelAdmin::TypeError
  ));

  // @@ Pradeep: more indentation problems....
  virtual void disconnect_sequence_push_supplier (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));


  virtual void shutdown (TAO_ENV_SINGLE_ARG_DECL);
  // Shutdown.

  // @@ Pradeep: please setup your editor to start 'protected',
  // 'public' and 'private' on the first column

 protected:
  virtual void dispatch_event_i (TAO_Notify_Event &event TAO_ENV_ARG_DECL);
  // Deliver the event to the consumer.

  virtual void dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed TAO_ENV_ARG_DECL);
  // Deliver the update to the consumer.

  CosNotifyComm::SequencePushConsumer_var push_consumer_;
  // The consumer that we're connect to.

 private:
  typedef TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::SequenceProxyPushSupplier>
    proxy_inherited;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_SEQUENCEPROXYPUSHSUPPLIER_I_H */
