/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_StructuredPushSupplier
//
// = DESCRIPTION
//   This class is to be used by clients of the Notification Service.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef TAO_NOTIFY_STRUCTUREDPUSHSUPPLIER_H
#define TAO_NOTIFY_STRUCTUREDPUSHSUPPLIER_H

#include "orbsvcs/orbsvcs/CosNotifyChannelAdminS.h"

class TAO_ORBSVCS_Export TAO_Notify_StructuredPushSupplier:public POA_CosNotifyComm::StructuredPushSupplier, public PortableServer::RefCountServantBase
{
  // = TITLE
  //   TAO_Notify_StructuredPushSupplier
  //
  // = DESCRIPTION
  //
  //
 public:
  // = Initialization and Termination code
  TAO_Notify_StructuredPushSupplier (void);
  // Constructor.

  void init (PortableServer::POA_ptr poa, CORBA::Environment & /*ACE_TRY_ENV*/);
  // Init

  void connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin, CORBA::Environment &ACE_TRY_ENV);
  // Connect the Supplier to the EventChannel.
  // Creates a new proxy supplier and connects to it.

  void disconnect (CORBA::Environment &ACE_TRY_ENV);
  // Disconnect from the supplier.

  virtual void send_event (const CosNotification::StructuredEvent& event,
                           CORBA::Environment &ACE_TRY_ENV);
  // Send one event.


  void deactivate (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Deactivate the object.

  // = ServantBase operations
  PortableServer::POA_ptr _default_POA (CORBA::Environment &env);

  CosNotifyChannelAdmin::ProxyID my_id_;
  // This supplier's id.

protected:
  virtual ~TAO_Notify_StructuredPushSupplier ();
  // Destructor

  // = NotifySubscribe
    virtual void subscription_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ));

    // = StructuredPushSupplier method
    virtual void disconnect_structured_push_supplier (
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  // = Data members
  PortableServer::POA_ptr default_POA_;
  // The default POA.

  CosNotifyChannelAdmin::StructuredProxyPushConsumer_var consumer_proxy_;
  // The proxy that we are connected to.
};
#endif /* TAO_NOTIFY_STRUCTUREDPUSHSUPPLIER_H */
