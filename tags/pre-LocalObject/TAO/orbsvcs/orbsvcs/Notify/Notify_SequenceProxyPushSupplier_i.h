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
class TAO_Notify_Resource_Manager;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Notify_Export TAO_Notify_SequenceProxyPushSupplier_i : public TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::SequenceProxyPushSupplier>, public PortableServer::RefCountServantBase
{
  // = TITLE
  //   TAO_Notify_SequenceProxyPushSupplier_i
  //
  // = DESCRIPTION
  //   implements CosNotifyChannelAdmin::SequenceProxyPushSupplier
  //

 public:
  TAO_Notify_SequenceProxyPushSupplier_i (TAO_Notify_ConsumerAdmin_i* consumeradmin, TAO_Notify_Resource_Manager* resource_manager);
  // Constructor

  virtual ~TAO_Notify_SequenceProxyPushSupplier_i (void);
  // Destructor

  virtual void connect_sequence_push_consumer (
    CosNotifyComm::SequencePushConsumer_ptr push_consumer,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventChannelAdmin::AlreadyConnected,
    CosEventChannelAdmin::TypeError
  ));

  // @@ Pradeep: more indentation problems....
virtual void disconnect_sequence_push_supplier (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  // @@ Pradeep: please setup your editor to start 'protected',
  // 'public' and 'private' on the first column

 protected:
  virtual void dispatch_event_i (TAO_Notify_Event &event, CORBA::Environment &ACE_TRY_ENV);
  // Deliver the event to the consumer.

  virtual void dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed, CORBA::Environment &ACE_TRY_ENV);
  // Deliver the update to the consumer.

// = Helper methods
 virtual void cleanup_i (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
 // Cleanup all resources used by this object.

 CosNotifyComm::SequencePushConsumer_var push_consumer_;
 // The consumer that we're connect to.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_SEQUENCEPROXYPUSHSUPPLIER_I_H */
