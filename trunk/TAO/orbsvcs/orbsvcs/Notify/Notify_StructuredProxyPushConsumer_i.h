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
//
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_STRUCTUREDPROXYPUSHCONSUMER_I_H
#define NOTIFY_STRUCTUREDPROXYPUSHCONSUMER_I_H

#include "orbsvcs/orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/orbsvcs/Notify/Notify_ProxyConsumer_i.h"
#include "orbsvcs/orbsvcs/Notify/NotifyPublish_i.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_ORBSVCS_Export TAO_Notify_StructuredProxyPushConsumer_i :
public virtual POA_CosNotifyChannelAdmin::StructuredProxyPushConsumer,
  public virtual TAO_Notify_ProxyConsumer_i,
  public virtual TAO_NotifyPublish_i
{
  // = TITLE
  //   TAO_Notify_StructuredProxyPushConsumer_i
  // = DESCRIPTION
  //
  //

public:
  TAO_Notify_StructuredProxyPushConsumer_i (TAO_Notify_SupplierAdmin_i &supplieradmin);
  // Constructor

  virtual ~TAO_Notify_StructuredProxyPushConsumer_i (void);
  // Destructor

  virtual CosNotifyChannelAdmin::ProxyConsumer_ptr get_ref (CORBA::Environment &ACE_TRY_ENV);
  //

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
 CosNotifyComm::StructuredPushSupplier_var push_supplier_;
 //
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_STRUCTUREDPROXYPUSHCONSUMER_I_H */
