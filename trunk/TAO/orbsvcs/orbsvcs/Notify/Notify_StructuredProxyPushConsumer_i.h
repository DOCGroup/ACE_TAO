/* -*- C++ -*- */
//=============================================================================
/**
 * @file Notify_StructuredProxyPushConsumer_i.h
 *
 * $Id$
 *
 * Implements the CosNotifyChannelAdmin::StructuredProxyPushConsumer
 * interface.
 *
 * @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_NOTIFY_STRUCTUREDPROXYPUSHCONSUMER_I_H
#define TAO_NOTIFY_STRUCTUREDPROXYPUSHCONSUMER_I_H

#include "ace/pre.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Notify_ProxyConsumer_T.h"
#include "notify_export.h"

class TAO_Notify_SupplierAdmin_i;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_Notify_StructuredProxyPushConsumer_i
 *
 * @brief TAO_Notify_StructuredProxyPushConsumer_i
 *
 * Implements CosNotifyChannelAdmin::StructuredProxyPushConsumer
 */
class TAO_Notify_Export TAO_Notify_StructuredProxyPushConsumer_i : public TAO_Notify_ProxyConsumer <POA_CosNotifyChannelAdmin::StructuredProxyPushConsumer>
{

public:
  /// Constructor
  TAO_Notify_StructuredProxyPushConsumer_i (TAO_Notify_SupplierAdmin_i* supplier_admin);

  /// Destructor
  virtual ~TAO_Notify_StructuredProxyPushConsumer_i (void);

  // = interface methods
  virtual void connect_structured_push_supplier (
    CosNotifyComm::StructuredPushSupplier_ptr push_supplier
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventChannelAdmin::AlreadyConnected
  ));

virtual void push_structured_event (
    const CosNotification::StructuredEvent & notification
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventComm::Disconnected
  ));

  virtual void disconnect_structured_push_consumer (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

protected:
  // = Helper methods
  /// Dispatch updates to the supplier.
 virtual void dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed ACE_ENV_ARG_DECL);

  // = Data members
  /// The supplier that we're connected to.
  CosNotifyComm::StructuredPushSupplier_var push_supplier_;

private:
  typedef TAO_Notify_ProxyConsumer <POA_CosNotifyChannelAdmin::StructuredProxyPushConsumer>
  proxy_inherited;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_STRUCTUREDPROXYPUSHCONSUMER_I_H */
