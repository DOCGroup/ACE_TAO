//=============================================================================
/**
 *  @file   Notify_ProxyPushConsumer_i.h
 *
 *  $Id$
 *
 * Implements the CosNotifyChannelAdmin::ProxyPushConsumer interface and
 * CosEventChannelAdmin::ProxyPushConsumerx
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


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

/**
 * @class TAO_Notify_ProxyPushConsumer_i
 *
 * @brief TAO_Notify_ProxyPushConsumer_i
 *
 * Implements the CosNotifyChannelAdmin::ProxyPushConsumer interface.
 */
class TAO_Notify_Export TAO_Notify_ProxyPushConsumer_i : public TAO_Notify_ProxyConsumer <POA_CosNotifyChannelAdmin::ProxyPushConsumer>
{

public:
  /// Constructor
  TAO_Notify_ProxyPushConsumer_i (TAO_Notify_SupplierAdmin_i* supplier_admin);

  /// Destructor
  virtual ~TAO_Notify_ProxyPushConsumer_i (void);

  // = Interface methods
  void push (const CORBA::Any & data ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ));

  virtual void connect_any_push_supplier (
    CosEventComm::PushSupplier_ptr push_supplier
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventChannelAdmin::AlreadyConnected
  ));

virtual void disconnect_push_consumer (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

 protected:
// = Helper methods
 virtual void dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed ACE_ENV_ARG_DECL);

 // = Data members
 /**
  * True if the supplier supports the NotifySubscribe interface.
  * If it does, we use the <notify_push_supplier_> else we use
  * <cosec_push_supplier_>
  */
 CORBA::Boolean notify_style_supplier_;

 /// The supplier connected to us.
 CosEventComm::PushSupplier_var cosec_push_supplier_;
 CosNotifyComm::PushSupplier_var notify_push_supplier_;

 private:
 typedef TAO_Notify_ProxyConsumer <POA_CosNotifyChannelAdmin::ProxyPushConsumer>
   proxy_inherited;
};

/**
 * @class TAO_Notify_CosEC_ProxyPushConsumer_i
 *
 * @brief TAO_Notify_CosEC_ProxyPushConsumer_i
 *
 * CosEventChannelAdmin::ProxyPushConsumer wrapper implementation.
 * Delegates to TAO_Notify_ProxyPushConsumer_i.
 */
class TAO_Notify_Export TAO_Notify_CosEC_ProxyPushConsumer_i : public virtual POA_CosEventChannelAdmin::ProxyPushConsumer, public virtual PortableServer::RefCountServantBase
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_Notify_CosEC_ProxyPushConsumer_i (TAO_Notify_SupplierAdmin_i* supplieradmi);

  /// Destructor.
  virtual ~TAO_Notify_CosEC_ProxyPushConsumer_i (void);

    /// init.
  void init (ACE_ENV_SINGLE_ARG_DECL);

  /// Suppliers call this method to pass data to connected consumers.
  virtual void push (const CORBA::Any &data
                     ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  /// Disconnects the supplier from the event communication.
  virtual void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));

  /// Connects a push supplier.
  virtual void connect_push_supplier(CosEventComm::PushSupplier_ptr push_supplier
                                     ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosEventChannelAdmin::AlreadyConnected));
 protected:
  // = Data Members
  /// Proxy to delegate to.
  TAO_Notify_ProxyPushConsumer_i notify_proxy_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_PROXYPUSHCONSUMER_I_H */
