/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   Notify_Supplier.h
 *
 *  $Id$
 *
 *  An example of using the Notify_Logging_Service.
 *  
 *
 *
 *  @author D A Hanvey (d.hanvey@qub.ac.uk)
 */
// ============================================================================

#ifndef NOTIFY_SUPPLIER_H
#define NOTIFY_SUPPLIER_H

#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "orbsvcs/DsNotifyLogAdminC.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/CosNotifyCommS.h"


class Filter_StructuredPushSupplier;

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Supplier 
{
  // = TITLE
  //   Simple supplier object
  //
  // = DESCRIPTION
  //   This class is a supplier of events.
  //
public:
  Supplier (void);
  // Constructor

  ~Supplier ();
  // Destructor.

  int run (int argc, char* argv[]);
  // Run the test

private:

  // = Data Members
  CORBA::ORB_var orb_;
  // The ORB that we use.

  CosNaming::NamingContext_var naming_context_;
  // Handle to the name service.

  DsNotifyLogAdmin::NotifyLogFactory_var notify_log_factory_;
  // The notify log factory from the Log Service.

  DsNotifyLogAdmin::NotifyLog_var notify_log_;

  Filter_StructuredPushSupplier* supplier_1;

  CosNotifyChannelAdmin::SupplierAdmin_var supplier_admin_;
};

class Filter_StructuredPushSupplier : public POA_CosNotifyComm::StructuredPushSupplier, public PortableServer::RefCountServantBase
{
  // = TITLE
  //   Filter_StructuredPushSupplier
  //
  // = DESCRIPTION
  //   Supplier for the filter example.
  //
 public:
  // = Initialization and Termination code
  Filter_StructuredPushSupplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED);
  // Constructor.

  void connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin
                ACE_ENV_ARG_DECL);
  // Connect the Supplier to the EventChannel.
  // Creates a new proxy supplier and connects to it.

  void disconnect (ACE_ENV_SINGLE_ARG_DECL);
  // Disconnect from the supplier.

  virtual void send_event (const CosNotification::StructuredEvent& event
                           ACE_ENV_ARG_DECL);
  // Send one event.

protected:
  // = Data members
  CosNotifyChannelAdmin::StructuredProxyPushConsumer_var proxy_consumer_;
  // The proxy that we are connected to.

  CosNotifyChannelAdmin::ProxyID proxy_consumer_id_;
  // This supplier's id.

  // = Protected Methods
  virtual ~Filter_StructuredPushSupplier ();
  // Destructor

  // = NotifySubscribe
  virtual void subscription_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
        ACE_ENV_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ));

  // = StructuredPushSupplier method
    virtual void disconnect_structured_push_supplier (
        ACE_ENV_SINGLE_ARG_DECL
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
};


#endif /* NOTIFY_SUPPLIER_H */
