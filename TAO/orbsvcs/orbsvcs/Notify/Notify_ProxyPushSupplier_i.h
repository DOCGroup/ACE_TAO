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
#include "ace/pre.h"
#define TAO_NOTIFY_PROXYPUSHSUPPLIER_I_H

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

class TAO_ORBSVCS_Export TAO_Notify_ProxyPushSupplier_i : public TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::ProxyPushSupplier>, public PortableServer::RefCountServantBase
{
  // = TITLE
  //   TAO_Notify_ProxyPushSupplier_i
  //
  // = DESCRIPTION
  //   implements CosNotifyChannelAdmin::ProxyPushSupplier.
  //

public:
  TAO_Notify_ProxyPushSupplier_i (TAO_Notify_ConsumerAdmin_i* consumeradmin,
                                  TAO_Notify_Resource_Manager* resource_manager);
  // Constructor

  virtual ~TAO_Notify_ProxyPushSupplier_i (void);
  // Destructor

  // = Interface methods
  virtual void connect_any_push_consumer (
    CosEventComm::PushConsumer_ptr push_consumer,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventChannelAdmin::AlreadyConnected,
    CosEventChannelAdmin::TypeError
  ));

virtual void disconnect_push_supplier (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

 protected:
 virtual void dispatch_event_i (TAO_Notify_Event &event, CORBA::Environment &ACE_TRY_ENV);
 // Deliver the event to the consumer.

 virtual void dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed, CORBA::Environment &ACE_TRY_ENV);
 // Deliver the update to the consumer.

  // = Helper methods
 virtual void cleanup_i (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
 // Cleanup all resources used by this object.

  CORBA::Boolean notify_style_consumer_;
  // True if the consumer supports the NotifyPublish interface.
  // If it does, we use the <notify_push_consumer_> else <cosec_push_consumer_>

  CosEventComm::PushConsumer_var cosec_push_consumer_;
  CosNotifyComm::PushConsumer_var notify_push_consumer_;
  // The consumer connected to us.
};

class TAO_ORBSVCS_Export TAO_Notify_CosEC_ProxyPushSupplier_i : public POA_CosEventChannelAdmin::ProxyPushSupplier, public virtual PortableServer::RefCountServantBase
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
  TAO_Notify_CosEC_ProxyPushSupplier_i(TAO_Notify_ConsumerAdmin_i* consumeradmin, TAO_Notify_Resource_Manager* resource_manager);
  // Constructor.

  ~TAO_Notify_CosEC_ProxyPushSupplier_i (void);
  // Destructor.

  void init (CORBA::Environment &ACE_TRY_ENV);
  // Init.

  virtual void disconnect_push_supplier (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // Ends the event communication and disposes this object.

  virtual void connect_push_consumer(CosEventComm::PushConsumer_ptr push_consumer, CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosEventChannelAdmin::AlreadyConnected,
                     CosEventChannelAdmin::TypeError));
  // Connects the <push_consumer> to the Event Channel.

 protected:
  TAO_Notify_ProxyPushSupplier_i notify_proxy_;
  // The proxy that we delegate too.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_PROXYPUSHSUPPLIER_I_H */
