/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   TAO_Notify_StructuredProxyPushConsumer_i.h
//
// = DESCRIPTION
//   Implements the POA_CosNotifyChannelAdmin::StructuredProxyPushConsumer
//   interface.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef TAO_NOTIFY_STRUCTUREDPROXYPUSHCONSUMER_I_H
#define TAO_NOTIFY_STRUCTUREDPROXYPUSHCONSUMER_I_H

#include "Notify_ProxyConsumer_T.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"

class TAO_Notify_SupplierAdmin_i;
class TAO_Notify_Resource_Manager;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_ORBSVCS_Export TAO_Notify_StructuredProxyPushConsumer_i : public TAO_Notify_ProxyConsumer <POA_CosNotifyChannelAdmin::StructuredProxyPushConsumer>, public PortableServer::RefCountServantBase
{
  // = TITLE
  //   TAO_Notify_StructuredProxyPushConsumer_i
  //
  // = DESCRIPTION
  //
  //

public:
  TAO_Notify_StructuredProxyPushConsumer_i (TAO_Notify_SupplierAdmin_i* consumeradmin, TAO_Notify_Resource_Manager* resource_manager);
  // Constructor

  virtual ~TAO_Notify_StructuredProxyPushConsumer_i (void);
  // Destructor

  // = Update_Listener method
  void dispatch_update (EVENTTYPE_LIST& added, EVENTTYPE_LIST& removed);
  // Dispatch the update to the supplier.

  // = interface methods
  virtual void connect_structured_push_supplier (
    CosNotifyComm::StructuredPushSupplier_ptr push_supplier,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventChannelAdmin::AlreadyConnected
  ));

virtual void push_structured_event (
    const CosNotification::StructuredEvent & notification,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventComm::Disconnected
  ));

  virtual void disconnect_structured_push_consumer (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

protected:
  // = Helper methods
  void cleanup_i (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Cleanup all resources used by this object.

  // = Data members
  CosNotifyComm::StructuredPushSupplier_ptr push_supplier_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_NOTIFY_STRUCTUREDPROXYPUSHCONSUMER_I_H */
