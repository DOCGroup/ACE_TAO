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

class TAO_ORBSVCS_Export TAO_Notify_StructuredPushSupplier:public POA_CosNotifyComm::StructuredPushSupplier
{
  // = TITLE
  //   TAO_Notify_StructuredPushSupplier
  // = DESCRIPTION
  //
  //
 public:
  // = Initialization and Termination code
  TAO_Notify_StructuredPushSupplier (void);
  // Constructor.

  ~TAO_Notify_StructuredPushSupplier ();
  // Destructor

  CosNotifyComm::StructuredPushSupplier_ptr get_ref (void);
  // Conver to object ref.

  void open (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin,
             CORBA::Environment& ACE_TRY_ENV);
  // This method connects the consumer to the EC.

  void close (CORBA::Environment &ACE_TRY_ENV);
  // Disconnect from the EC.

  void connect (CORBA::Environment &ACE_TRY_ENV);
  // Connect the CosECConsumer to the EventChannel.
  // Creates a new proxy supplier and connects to it.

  void disconnect (CORBA::Environment &ACE_TRY_ENV);
  // Disconnect from the supplier.

  virtual void send_event (const CosNotification::StructuredEvent& event,
                           CORBA::Environment &ACE_TRY_ENV);
  // Send one event.

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

 protected:
  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;
  // The consumeradmin that we get our proxy consumer from.

  CosNotifyChannelAdmin::ProxyID supplieradmin_proxy_id_;
  // The proxy's id.

  CosNotifyChannelAdmin::StructuredProxyPushConsumer_var consumer_proxy_;
  // The proxy that we are connected to.
};
#endif /* TAO_NOTIFY_STRUCTUREDPUSHSUPPLIER_H */
