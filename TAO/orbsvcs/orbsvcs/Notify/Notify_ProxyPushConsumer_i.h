/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_ProxyPushConsumer_i.h
//
// = DESCRIPTION
//
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_PROXYPUSHCONSUMER_I_H
#define NOTIFY_PROXYPUSHCONSUMER_I_H

#include "orbsvcs/orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/orbsvcs/Notify/Notify_ProxyConsumer_i.h"
#include "orbsvcs/orbsvcs/Notify/NotifyPublish_i.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Notify_ProxyPushConsumer_i :
  public virtual POA_CosNotifyChannelAdmin::ProxyPushConsumer,
  public virtual TAO_Notify_ProxyConsumer_i,
  public virtual TAO_NotifyPublish_i
{
  // = TITLE
  //   TAO_Notify_ProxyPushConsumer_i
  // = DESCRIPTION
  //
  //

public:
  TAO_Notify_ProxyPushConsumer_i (TAO_Notify_SupplierAdmin_i &supplieradmin);
  // Constructor

  virtual ~TAO_Notify_ProxyPushConsumer_i (void);
  // Destructor

  CosNotifyChannelAdmin::ProxyConsumer_ptr get_ref (CORBA::Environment &ACE_TRY_ENV);

virtual void connect_any_push_supplier (
    CosEventComm::PushSupplier_ptr push_supplier,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventChannelAdmin::AlreadyConnected
  ));

  // = CosEventComm::PushConsumer
  virtual void push (
    const CORBA::Any & data,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventComm::Disconnected
  ));

virtual void disconnect_push_consumer (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

protected:
  CosEventComm::PushSupplier_var push_supplier_;
 //
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* NOTIFY_PROXYPUSHCONSUMER_I_H */
