/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   Event_Consumer.h
 *
 *  $Id$
 *
 *  An example of using the Event_Logging_Service.
 *  The Event_Consumer consumes log-generated events.
 *
 *
 *  @author D A Hanvey (d.hanvey@qub.ac.uk)
 */
// ============================================================================

#ifndef EVENT_CONSUMER_H
#define EVENT_CONSUMER_H

#include "orbsvcs/DsEventLogAdminC.h"
#include "orbsvcs/CosEventCommS.h"
#include "orbsvcs/CosNamingC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Consumer : public POA_CosEventComm::PushConsumer
{
  // = TITLE
  //   Simple consumer object
  //
  // = DESCRIPTION
  //   This class is a consumer of log generated events.
  //
public:
  Consumer (void);
  // Constructor

  int run (int argc, char* argv[]);
  // Run the test

  // = The CosEventComm::PushConsumer methods

  virtual void push (const CORBA::Any &event
                     ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.

private:

  // = Data Members
  CORBA::ULong event_count_;
  // Keep track of the number of events received.

  CORBA::ORB_var orb_;
  // The orb, just a pointer because the ORB does not outlive the
  // run() method...

  CosNaming::NamingContext_var naming_context_;
  // Handle to the name service.

  DsEventLogAdmin::EventLogFactory_var event_log_factory_;
  // The Event Log Factory that generates the events to be consumed.

  CosEventChannelAdmin::ProxyPushSupplier_var supplier_;
  // The proxy that we are connected to.

};

#endif /* EVENT_CONSUMER_H */
