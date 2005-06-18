/* -*- C++ -*- */

// ============================================================================
/**
 *  @file   RTEvent_Consumer.h
 *
 *  $Id$
 *
 *  An example of using the RTEvent_Logging_Service.
 *  The RTEvent_Consumer consumes log-generated events.
 *
 *
 *  @author D A Hanvey (d.hanvey@qub.ac.uk)
 */
// ============================================================================

#ifndef RTEVENT_CONSUMER_H
#define RTEVENT_CONSUMER_H

#include "orbsvcs/RtecEventCommS.h"
#include "orbsvcs/RTEventLogAdminC.h"
#include "orbsvcs/CosNamingC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Consumer : public POA_RtecEventComm::PushConsumer
{
  // = TITLE
  //   Simple consumer object
  //
  // = DESCRIPTION
  //   This class is a consumer of events.
  //   It simply registers for one event type.
  //
public:
  Consumer (void);
  // Constructor

  int run (int argc, char* argv[]);
  // Run the test

  // = The RtecEventComm::PushConsumer methods

  virtual void push (const RtecEventComm::EventSet& events
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // The skeleton methods.

private:
  CORBA::ULong event_count_;
  // Keep track of the number of events received.

  CORBA::ORB_ptr orb_;
  // The orb, just a pointer because the ORB does not outlive the
  // run() method...

  CosNaming::NamingContext_var naming_context_;
  // Handle to the name service.

  RTEventLogAdmin::EventLogFactory_var event_log_factory_;
  // The Event Log Factory that generates the events to be consumed.

  RtecEventChannelAdmin::ProxyPushSupplier_var supplier_;
  // The proxy that we are connected to.

};

#endif /* RTEVENT_CONSUMER_H */
