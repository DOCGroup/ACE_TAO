/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = DESCRIPTION
//   The TAO event service implementation.
//
// ============================================================================

#ifndef EC_EVENT_SERVICE_H
#define EC_EVENT_SERVICE_H

#include "orbsvcs/RtecEventChannelAdminS.h"
#include "orbsvcs/RtecSchedulerS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Module_Factory;

class Event_Service : public POA_RtecEventChannelAdmin::EventChannel
{
  //
  // = TITLE
  //   This class decorates an Event Channel implementation, but in a
  //   very simple way: destroy() also shutdowns the ORB.
  //
  // = DESCRIPTION
  //   The Event_Channel implementations should not shutdown the ORB
  //   by default, but in this case, where the Event_Channel is
  //   (almost) the only service on the host, it makes more sense to
  //   do so.
  //
public:
  Event_Service (void);
  virtual ~Event_Service (void);

  int run (int argc, char* argv[]);
  // Run the event service.

  // = The RtecEventChannelAdmin::Event_Channel methods
  virtual RtecEventChannelAdmin::ConsumerAdmin_ptr
    for_consumers (CORBA::Environment &);
  virtual RtecEventChannelAdmin::SupplierAdmin_ptr
    for_suppliers (CORBA::Environment &);
  virtual void destroy (CORBA::Environment &);
  virtual RtecEventChannelAdmin::Observer_Handle
    append_observer (RtecEventChannelAdmin::Observer_ptr observer,
                     CORBA::Environment &env);
  virtual void remove_observer (RtecEventChannelAdmin::Observer_Handle,
                                CORBA::Environment &env);

private:
  int parse_args (int argc, char* argv[]);
  // parse the command line args

  enum {
    ES_NEW, // Use the Service Configurator to find factory
    ES_OLD_REACTIVE,   // Reactive dispatching, old EC.
    ES_OLD_MT          // Prioritized dispatching, old EC.
  };

private:
  TAO_Module_Factory *module_factory_;
  // The module factory for the EC.

  POA_RtecScheduler::Scheduler *sched_impl_;
  // The Scheduler implementation.

  POA_RtecEventChannelAdmin::EventChannel *ec_impl_;
  // The Event Channel implementation.

  const char* service_name_;
  // The name we use to bind with the NameService

  const char* ior_file_name_;
  // The name of the file were we output the Event_Service IOR.

  const char* pid_file_name_;
  // The name of a file where the process stores its pid

  int event_service_type_;
  // The type of event service we will use

  int global_scheduler_;
  // Should we use a global scheduler or a local one?

  CORBA::ORB_var orb_;
  // A reference to the ORB, to shut it down properly.
};

#endif /* EC_EVENT_SERVICE_H */
