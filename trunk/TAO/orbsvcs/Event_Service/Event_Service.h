/* -*- C++ -*- */

// ============================================================================
/**
 * @file Event_Service.h
 *
 * $Id$
 *
 * The TAO event service implementation.
 */
#ifndef EC_EVENT_SERVICE_H
#define EC_EVENT_SERVICE_H
#include /**/ "ace/pre.h"

#include "orbsvcs/RtecEventChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "orbsvcs/RtecSchedulerS.h"
#include "ace/SString.h"

class TAO_Module_Factory;

/**
 * @class Event_Service
 *
 * This class decorates an Event Channel implementation, but in a very simple
 * way: destroy() also shutdowns the ORB.
 *
 * The Event_Channel implementations should not shutdown the ORB by default,
 * but in this case, where the Event_Channel is (almost) the only service on
 * the host, it makes more sense to do so.
 */
class Event_Service : public POA_RtecEventChannelAdmin::EventChannel
{
public:
  Event_Service (void);
  virtual ~Event_Service (void);

  /// Run the event service.
  int run (int argc, ACE_TCHAR* argv[]);

  // = The RtecEventChannelAdmin::Event_Channel methods
  virtual RtecEventChannelAdmin::ConsumerAdmin_ptr
    for_consumers (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException));
  virtual RtecEventChannelAdmin::SupplierAdmin_ptr
    for_suppliers (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException));
  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException));
  virtual RtecEventChannelAdmin::Observer_Handle
    append_observer (RtecEventChannelAdmin::Observer_ptr observer
                     ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
          CORBA::SystemException,
          RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
          RtecEventChannelAdmin::EventChannel::CANT_APPEND_OBSERVER));
  virtual void remove_observer (RtecEventChannelAdmin::Observer_Handle
                                ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((
          CORBA::SystemException,
          RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
          RtecEventChannelAdmin::EventChannel::CANT_REMOVE_OBSERVER));

private:
  /// Parse the command line args
  int parse_args (int argc, ACE_TCHAR* argv[]);

  enum {
    /// Use the Service Configurator to find factory
    ES_NEW,
    /// Reactive dispatching, old EC.
    ES_OLD_REACTIVE,
    /// Prioritized dispatching, old EC.
    ES_OLD_MT
  };

  enum Sched_type_t {SCHED_NONE, SCHED_GLOBAL, SCHED_LOCAL};

private:
  /// The module factory for the EC.
  TAO_Module_Factory *module_factory_;

  /// The Scheduler implementation.
  POA_RtecScheduler::Scheduler *sched_impl_;

  /// The Event Channel implementation.
  POA_RtecEventChannelAdmin::EventChannel *ec_impl_;

  /// The name we use to bind with the NameService
  ACE_CString service_name_;

  /// The name of the file were we output the Event_Service IOR.
  ACE_CString ior_file_name_;

  /// The name of a file where the process stores its pid
  ACE_CString pid_file_name_;

  /// The name of the servant we use when we use persistent IORs
  ACE_CString servant_name_;

  /// Should we use a global scheduler or a local one or none?
  Sched_type_t scheduler_type_;

  /// The type of event service we will use
  int event_service_type_;

  /// A reference to the ORB, to shut it down properly.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* EC_EVENT_SERVICE_H */
