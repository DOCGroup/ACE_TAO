/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   RTEvent_Supplier.h
 *
 *  $Id$
 *
 *  An example of using the RTEvent_Logging_Service.
 *  
 *
 *
 *  @author D A Hanvey (d.hanvey@qub.ac.uk)
 */
// ============================================================================

#ifndef RTEVENT_SUPPLIER_H
#define RTEVENT_SUPPLIER_H

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RTEventLogAdminC.h"
#include "orbsvcs/CosNamingC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Supplier : public POA_RtecEventComm::PushSupplier
{
  // = TITLE
  //   Simple supplier object
  //
  // = DESCRIPTION
  //   This class is a supplier of events.
  //   It simply publishes one event type.
  //
public:
  Supplier (void);
  // Constructor

  int run (int argc, char* argv[]);
  // Run the test

  // = The RtecEventComm::PushSupplier methods

  virtual void disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.

private:

  // = Data Members
  CosNaming::NamingContext_var naming_context_;
  // Handle to the name service.

  RTEventLogAdmin::EventLogFactory_var event_log_factory_;
  // The Event Log Factory that generates the events to be consumed.

  RtecEventChannelAdmin::ProxyPushConsumer_var consumer_;
  // The proxy that we are connected to.
};

#endif /* RTEVENT_SUPPLIER_H */
