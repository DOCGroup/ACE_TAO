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

#include "Notify_ProxyConsumer_T.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"

class TAO_Notify_SupplierAdmin_i;
class TAO_Notify_Resource_Manager;
class TAO_Notify_Event_Manager;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_ORBSVCS_Export TAO_Notify_ProxyPushConsumer_i : public TAO_Notify_ProxyConsumer <POA_CosNotifyChannelAdmin::ProxyPushConsumer>, public PortableServer::RefCountServantBase
{
  // = TITLE
  //   TAO_Notify_ProxyPushConsumer_i
  //
  // = DESCRIPTION
  //   Implements the CosNotifyChannelAdmin::ProxyPushConsumer interface.
  //

public:
  TAO_Notify_ProxyPushConsumer_i (TAO_Notify_SupplierAdmin_i* supplieradmin,
                                  TAO_Notify_Resource_Manager* resource_manager);
  // Constructor

  virtual ~TAO_Notify_ProxyPushConsumer_i (void);
  // Destructor

  // = Interface methods
  void push (const CORBA::Any & data, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ));

  virtual void connect_any_push_supplier (
    CosEventComm::PushSupplier_ptr push_supplier,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventChannelAdmin::AlreadyConnected
  ));

virtual void disconnect_push_consumer (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

 protected:
// = Helper methods
 virtual void dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed, CORBA::Environment &ACE_TRY_ENV);

 void cleanup_i (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
 // Cleanup all resources used by this object.

// = Data members
 CORBA::Boolean notify_style_supplier_;
 // True if the supplier supports the NotifySubscribe interface.
 // If it does, we use the <notify_push_supplier_> else we use
 // <cosec_push_supplier_>

 CosEventComm::PushSupplier_var cosec_push_supplier_;
 CosNotifyComm::PushSupplier_var notify_push_supplier_;
 // The supplier connected to us.
};

class TAO_ORBSVCS_Export TAO_Notify_CosEC_ProxyPushConsumer_i : public virtual POA_CosEventChannelAdmin::ProxyPushConsumer, public virtual PortableServer::RefCountServantBase
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
  TAO_Notify_CosEC_ProxyPushConsumer_i (TAO_Notify_SupplierAdmin_i* supplieradmin, TAO_Notify_Resource_Manager* resource_manager);
  // Constructor.

  virtual ~TAO_Notify_CosEC_ProxyPushConsumer_i (void);
  // Destructor.

  void init (CORBA::Environment &ACE_TRY_ENV);
    // init.

  virtual void push (const CORBA::Any &data,
                     CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // Suppliers call this method to pass data to connected consumers.

  virtual void disconnect_push_consumer (CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // Disconnects the supplier from the event communication.

  virtual void connect_push_supplier(CosEventComm::PushSupplier_ptr push_supplier,
                                     CORBA::Environment &ACE_TRY_ENV)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       CosEventChannelAdmin::AlreadyConnected));
  // Connects a push supplier.
 protected:
  TAO_Notify_ProxyPushConsumer_i notify_proxy_;
  // Proxy to delegate to.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_NOTIFY_PROXYPUSHCONSUMER_I_H */
