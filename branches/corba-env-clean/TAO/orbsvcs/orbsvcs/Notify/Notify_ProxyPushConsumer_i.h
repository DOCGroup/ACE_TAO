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
//   Implements the CosNotifyChannelAdmin::ProxyPushConsumer interface and
//   CosEventChannelAdmin::ProxyPushConsumerx
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef TAO_NOTIFY_PROXYPUSHCONSUMER_I_H
#define TAO_NOTIFY_PROXYPUSHCONSUMER_I_H

#include "ace/pre.h"
#include "Notify_ProxyConsumer_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyChannelAdminS.h"

class TAO_Notify_SupplierAdmin_i;
class TAO_Notify_Event_Manager;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Notify_Export TAO_Notify_ProxyPushConsumer_i : public TAO_Notify_ProxyConsumer <POA_CosNotifyChannelAdmin::ProxyPushConsumer>
{
  // = TITLE
  //   TAO_Notify_ProxyPushConsumer_i
  //
  // = DESCRIPTION
  //   Implements the CosNotifyChannelAdmin::ProxyPushConsumer interface.
  //

public:
  TAO_Notify_ProxyPushConsumer_i (TAO_Notify_SupplierAdmin_i* supplier_admin);
  // Constructor

  virtual ~TAO_Notify_ProxyPushConsumer_i (void);
  // Destructor

  // = Interface methods
  void push (const CORBA::Any & data TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ));

  virtual void connect_any_push_supplier (
    CosEventComm::PushSupplier_ptr push_supplier
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventChannelAdmin::AlreadyConnected
  ));

virtual void disconnect_push_consumer (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

 protected:
// = Helper methods
 virtual void dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed TAO_ENV_ARG_DECL);

 // = Data members
 CORBA::Boolean notify_style_supplier_;
 // True if the supplier supports the NotifySubscribe interface.
 // If it does, we use the <notify_push_supplier_> else we use
 // <cosec_push_supplier_>

 CosEventComm::PushSupplier_var cosec_push_supplier_;
 CosNotifyComm::PushSupplier_var notify_push_supplier_;
 // The supplier connected to us.

 private:
 typedef TAO_Notify_ProxyConsumer <POA_CosNotifyChannelAdmin::ProxyPushConsumer>
   proxy_inherited;
};

class TAO_Notify_Export TAO_Notify_CosEC_ProxyPushConsumer_i : public virtual POA_CosEventChannelAdmin::ProxyPushConsumer, public virtual PortableServer::RefCountServantBase
{
  // = TITLE
  //   TAO_Notify_CosEC_ProxyPushConsumer_i
  //
  // = DESCRIPTION
  //   CosEventChannelAdmin::ProxyPushConsumer wrapper implementation.
  //   Delegates to TAO_Notify_ProxyPushConsumer_i.
  //
public:
  // = Initialization and termination methods.
  TAO_Notify_CosEC_ProxyPushConsumer_i (TAO_Notify_SupplierAdmin_i* supplieradmi);
  // Constructor.

  virtual ~TAO_Notify_CosEC_ProxyPushConsumer_i (void);
  // Destructor.

  void init (TAO_ENV_SINGLE_ARG_DECL);
    // init.

  virtual void push (const CORBA::Any &data
                     TAO_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // Suppliers call this method to pass data to connected consumers.

  virtual void disconnect_push_consumer (TAO_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // Disconnects the supplier from the event communication.

  virtual void connect_push_supplier(CosEventComm::PushSupplier_ptr push_supplier
                                     TAO_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosEventChannelAdmin::AlreadyConnected));
  // Connects a push supplier.
 protected:
  // = Data Members
  TAO_Notify_ProxyPushConsumer_i notify_proxy_;
  // Proxy to delegate to.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_PROXYPUSHCONSUMER_I_H */
