/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_StructuredPushConsumer.h
//
// = DESCRIPTION
//   This class is to be used by clients of the Notification Service.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_STRUCTUREDPUSHCONSUMER_H
#define NOTIFY_STRUCTUREDPUSHCONSUMER_H

#include "orbsvcs/orbsvcs/CosNotifyChannelAdminS.h"

class TAO_ORBSVCS_Export TAO_Notify_StructuredPushConsumer : public POA_CosNotifyComm::StructuredPushConsumer
{
  // = TITLE
  //   Notify_StructuredPushConsumer
  // = DESCRIPTION
  //
  //
 public:
  // = Initialization and Termination code
  TAO_Notify_StructuredPushConsumer (void);
  // Constructor.

  ~TAO_Notify_StructuredPushConsumer (void);
  // Destructor

  CosNotifyComm::StructuredPushConsumer_ptr get_ref (void);
  // Convert to obj ref.

  virtual void open (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin,
             CORBA::Environment& ACE_TRY_ENV);
  // This method connects the consumer to the EC.

  virtual void close (CORBA::Environment &ACE_TRY_ENV);
  // Disconnect from the EC.

  virtual void connect (CORBA::Environment &ACE_TRY_ENV);
  // Connect the CosECConsumer to the EventChannel.
  // Creates a new proxy supplier and connects to it.

  virtual void disconnect (CORBA::Environment &ACE_TRY_ENV);
  // Disconnect from the supplier.

  // = NotifyPublish method
    virtual void offer_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ));

  // = StructuredPushSupplier methods
  virtual void push_structured_event (
        const CosNotification::StructuredEvent & notification,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventComm::Disconnected
       )) = 0;

  virtual void disconnect_structured_push_consumer (
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
 protected:
  CosNotifyChannelAdmin::ConsumerAdmin_var consumer_admin_;
  // The consumeradmin that we get our proxy supplier from.

  CosNotifyChannelAdmin::ProxyID consumeradmin_proxy_id_;
  // The proxy's id.

  CosNotifyChannelAdmin::StructuredProxyPushSupplier_var supplier_proxy_;
  // The proxy that we are connected to.
};
#endif /* NOTIFY_STRUCTUREDPUSHCONSUMER_H */
