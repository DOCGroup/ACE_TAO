//=============================================================================
/**
 *  @file   Notify_ProxyPushSupplier_i.h
 *
 *  $Id$
 *
 * implements CosNotifyChannelAdmin::ProxyPushSupplier and
 * CosEventChannelAdmin::ProxyPushSupplier
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================

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

/**
 * @class TAO_Notify_ProxyPushSupplier_i
 *
 * @brief TAO_Notify_ProxyPushSupplier_i
 *
 * implements CosNotifyChannelAdmin::ProxyPushSupplier.
 */
class TAO_Notify_Export TAO_Notify_ProxyPushSupplier_i : public TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::ProxyPushSupplier>
{

public:
  /// Constructor
  TAO_Notify_ProxyPushSupplier_i (TAO_Notify_ConsumerAdmin_i* consumeradmin);

  /// Destructor
  virtual ~TAO_Notify_ProxyPushSupplier_i (void);

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

  /// Shutdown.
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL);

 protected:
  /// Shutdown
  void shutdown_i (ACE_ENV_SINGLE_ARG_DECL);

  /// Deliver the event to the consumer.
  virtual void dispatch_event_i (TAO_Notify_Event &event ACE_ENV_ARG_DECL);

  /// Deliver the update to the consumer.
  virtual void dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed ACE_ENV_ARG_DECL);

  // = Data Members.
  /// True if the consumer supports the NotifyPublish interface.
  /// If it does, we use the <notify_push_consumer_> else <cosec_push_consumer_>
  CORBA::Boolean notify_style_consumer_;

  /// The consumer connected to us.
  CosEventComm::PushConsumer_var cosec_push_consumer_;
  CosNotifyComm::PushConsumer_var notify_push_consumer_;

 private:
 typedef TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::ProxyPushSupplier>
   proxy_inherited;
};

/**
 * @class TAO_Notify_CosEC_ProxyPushSupplier_i
 *
 * @brief TAO_Notify_CosEC_ProxyPushSupplier_i
 *
 * implements CosEventChannelAdmin::ProxyPushSupplier by delegating
 * to TAO_Notify_ProxyPushSupplier_i.
 */
class TAO_Notify_Export TAO_Notify_CosEC_ProxyPushSupplier_i : public POA_CosEventChannelAdmin::ProxyPushSupplier, public virtual PortableServer::RefCountServantBase
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_Notify_CosEC_ProxyPushSupplier_i(TAO_Notify_ConsumerAdmin_i* consumeradmin);

  /// Destructor.
  ~TAO_Notify_CosEC_ProxyPushSupplier_i (void);

  /// Init.
  void init (ACE_ENV_SINGLE_ARG_DECL);

  /// Ends the event communication and disposes this object.
  virtual void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  /// Connects the <push_consumer> to the Event Channel.
  virtual void connect_push_consumer(CosEventComm::PushConsumer_ptr push_consumer ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosEventChannelAdmin::AlreadyConnected,
                     CosEventChannelAdmin::TypeError));

 protected:
  // = Data Members
  /// The proxy that we delegate too.
  TAO_Notify_ProxyPushSupplier_i notify_proxy_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_PROXYPUSHSUPPLIER_I_H */
