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

class TAO_ORBSVCS_Export TAO_Notify_StructuredProxyPushSupplier_i : public TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::StructuredProxyPushSupplier>, public PortableServer::RefCountServantBase
{
  // = TITLE
  //   TAO_Notify_StructuredProxyPushSupplier_i
  //
  // = DESCRIPTION
  //
  //

 public:
  TAO_Notify_StructuredProxyPushSupplier_i (TAO_Notify_ConsumerAdmin_i* consumeradmin, TAO_Notify_Resource_Manager* resource_manager);
  // Constructor

  virtual ~TAO_Notify_StructuredProxyPushSupplier_i (void);
  // Destructor

  // = Notify_Event_Listener methods
  void dispatch_event (const CORBA::Any & data, CORBA::Environment &ACE_TRY_ENV);
  void dispatch_event (const CosNotification::StructuredEvent & notification,
                       CORBA::Environment &ACE_TRY_ENV);
  // Dispatch event to consumer.

  // = Update Listener method
  void dispatch_update (EVENTTYPE_LIST& added, EVENTTYPE_LIST& removed);
  // The event manager invokes this to send publication updates

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

 protected:
// = Helper methods
 virtual void cleanup_i (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
 // Cleanup all resources used by this object.

 CosNotifyComm::StructuredPushConsumer_var push_consumer_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_NOTIFY_STRUCTUREDPROXYPUSHSUPPLIER_I_H */
