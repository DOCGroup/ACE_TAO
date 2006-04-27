/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   Event_Supplier.h
 *
 *  $Id$
 *
 *  An example of using the Event_Logging_Service.
 *  
 *
 *
 *  @author D A Hanvey (d.hanvey@qub.ac.uk)
 */
// ============================================================================

#ifndef EVENT_SUPPLIER_H
#define EVENT_SUPPLIER_H

#include "orbsvcs/DsEventLogAdminC.h"
#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/CosNamingC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Supplier : public POA_CosEventComm::PushSupplier
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

  int run (int argc, char* argv[]);
  // Run the test

  // = The CosEventComm::PushSupplier methods

  virtual void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.

private:

  // = Data Members
  CORBA::ORB_var orb_;
  // The ORB that we use.

  CosNaming::NamingContext_var naming_context_;
  // Handle to the name service.

  DsEventLogAdmin::EventLogFactory_var event_log_factory_;
  // The Event Log Factory that generates the events to be consumed.

  CosEventChannelAdmin::ProxyPushConsumer_var consumer_;
  // The proxy that we are connected to.

};

#endif /* EVENT_SUPPLIER_H */
