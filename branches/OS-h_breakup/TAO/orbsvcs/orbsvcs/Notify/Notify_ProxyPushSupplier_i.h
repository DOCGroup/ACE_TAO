// $Id$
// =========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_ProxyPushSupplier_i.h
//
// = DESCRIPTION
//   implements CosNotifyChannelAdmin::ProxyPushSupplier and
//   CosEventChannelAdmin::ProxyPushSupplier
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================
#ifndef TAO_NOTIFY_PROXYPUSHSUPPLIER_I_H
#define TAO_NOTIFY_PROXYPUSHSUPPLIER_I_H

#include "ace/pre.h"
#include "Notify_ProxySupplier_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"

class TAO_Notify_ConsumerAdmin_i;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Notify_Export TAO_Notify_ProxyPushSupplier_i : public TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::ProxyPushSupplier>
{
  // = TITLE
  //   TAO_Notify_ProxyPushSupplier_i
  //
  // = DESCRIPTION
  //   implements CosNotifyChannelAdmin::ProxyPushSupplier.
  //

public:
  TAO_Notify_ProxyPushSupplier_i (TAO_Notify_ConsumerAdmin_i* consumeradmin);
  // Constructor

  virtual ~TAO_Notify_ProxyPushSupplier_i (void);
  // Destructor

  // = Interface methods
  virtual void connect_any_push_consumer (
    CosEventComm::PushConsumer_ptr push_consumer
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventChannelAdmin::AlreadyConnected,
    CosEventChannelAdmin::TypeError
  ));

virtual void disconnect_push_supplier (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL);
  // Shutdown.

 protected:
  void shutdown_i (ACE_ENV_SINGLE_ARG_DECL);
  // Shutdown

 virtual void dispatch_event_i (TAO_Notify_Event &event ACE_ENV_ARG_DECL);
  // Deliver the event to the consumer.

 virtual void dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed ACE_ENV_ARG_DECL);
  // Deliver the update to the consumer.

  // = Data Members.
 CORBA::Boolean notify_style_consumer_;
  // True if the consumer supports the NotifyPublish interface.
  // If it does, we use the <notify_push_consumer_> else <cosec_push_consumer_>

 CosEventComm::PushConsumer_var cosec_push_consumer_;
 CosNotifyComm::PushConsumer_var notify_push_consumer_;
 // The consumer connected to us.

 private:
 typedef TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::ProxyPushSupplier>
   proxy_inherited;
};

class TAO_Notify_Export TAO_Notify_CosEC_ProxyPushSupplier_i : public POA_CosEventChannelAdmin::ProxyPushSupplier, public virtual PortableServer::RefCountServantBase
{
  // = TITLE
  //   TAO_Notify_CosEC_ProxyPushSupplier_i
  //
  // = DESCRIPTION
  //   implements CosEventChannelAdmin::ProxyPushSupplier by delegating
  //   to TAO_Notify_ProxyPushSupplier_i.
  //
public:
  // = Initialization and termination methods.
  TAO_Notify_CosEC_ProxyPushSupplier_i(TAO_Notify_ConsumerAdmin_i* consumeradmin);
  // Constructor.

  ~TAO_Notify_CosEC_ProxyPushSupplier_i (void);
  // Destructor.

  void init (ACE_ENV_SINGLE_ARG_DECL);
  // Init.

  virtual void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // Ends the event communication and disposes this object.

  virtual void connect_push_consumer(CosEventComm::PushConsumer_ptr push_consumer ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosEventChannelAdmin::AlreadyConnected,
                     CosEventChannelAdmin::TypeError));
  // Connects the <push_consumer> to the Event Channel.

 protected:
  // = Data Members
  TAO_Notify_ProxyPushSupplier_i notify_proxy_;
  // The proxy that we delegate too.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_PROXYPUSHSUPPLIER_I_H */
