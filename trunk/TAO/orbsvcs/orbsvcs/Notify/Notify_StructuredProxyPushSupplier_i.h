// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_StructuredProxyPushSupplier_i.h
//
// = DESCRIPTION
//   Implements the POA_CosNotifyChannelAdmin::StructuredProxyPushSupplier
//   interface.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================
#ifndef TAO_NOTIFY_STRUCTUREDPROXYPUSHSUPPLIER_I_H
#define TAO_NOTIFY_STRUCTUREDPROXYPUSHSUPPLIER_I_H

#include "ace/pre.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Notify_ProxySupplier_T.h"

class TAO_Notify_ConsumerAdmin_i;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Notify_Export TAO_Notify_StructuredProxyPushSupplier_i : public TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::StructuredProxyPushSupplier>
{
  // = TITLE
  //   TAO_Notify_StructuredProxyPushSupplier_i
  //
  // = DESCRIPTION
  //   Implements POA_CosNotifyChannelAdmin::StructuredProxyPushSupplier
  //

 public:
  TAO_Notify_StructuredProxyPushSupplier_i (TAO_Notify_ConsumerAdmin_i* consumer_admin);
  // Constructor

  virtual ~TAO_Notify_StructuredProxyPushSupplier_i (void);
  // Destructor

  virtual void connect_structured_push_consumer (
    CosNotifyComm::StructuredPushConsumer_ptr push_consumer,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventChannelAdmin::AlreadyConnected,
    CosEventChannelAdmin::TypeError
  ));

  virtual void disconnect_structured_push_supplier (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void shutdown (CORBA::Environment &ACE_TRY_ENV);
  // Shutdown.

 protected:
  // = Helper methods
  void shutdown_i (CORBA::Environment &ACE_TRY_ENV);
  // Shutdown

  virtual void dispatch_event_i (TAO_Notify_Event &event, CORBA::Environment &ACE_TRY_ENV);
  // Deliver the event to the consumer.

  virtual void dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed, CORBA::Environment &ACE_TRY_ENV);
  // Deliver the update to the consumer.

  // = Data Members
  CosNotifyComm::StructuredPushConsumer_var push_consumer_;
  // The consumer that we're connect to.

 private:
  typedef TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::StructuredProxyPushSupplier>
    proxy_inherited;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_STRUCTUREDPROXYPUSHSUPPLIER_I_H */
