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
//   implements CosNotifyChannelAdmin::ProxyPushSupplier
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================
#ifndef TAO_NOTIFY_PROXYPUSHSUPPLIER_I_H
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
  //
  //

public:
  TAO_Notify_ProxyPushSupplier_i (TAO_Notify_ConsumerAdmin_i* consumeradmin,
                                  TAO_Notify_Resource_Manager* resource_manager);
  // Constructor

  virtual ~TAO_Notify_ProxyPushSupplier_i (void);
  // Destructor

  // = Notify_Event_Listener methods
  void dispatch_event (const CORBA::Any & data, CORBA::Environment &ACE_TRY_ENV);
  void dispatch_event (const CosNotification::StructuredEvent & notification,
                       CORBA::Environment &ACE_TRY_ENV);
  // Dispatch event to consumer.

  // = Update Listener method
  void dispatch_update (EVENTTYPE_LIST& added, EVENTTYPE_LIST& removed);
  // The event manager invokes this to send publication updates

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

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_NOTIFY_PROXYPUSHSUPPLIER_I_H */
