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
    for_consumers (void);
  virtual RtecEventChannelAdmin::SupplierAdmin_ptr
    for_suppliers (void);
  virtual void destroy (void);
  virtual RtecEventChannelAdmin::Observer_Handle
    append_observer (RtecEventChannelAdmin::Observer_ptr observer);
  virtual void remove_observer (RtecEventChannelAdmin::Observer_Handle);

private:
  /// Parse the command line args
  int parse_args (int argc, ACE_TCHAR* argv[]);

  enum Sched_type_t {ES_SCHED_NONE, ES_SCHED_GLOBAL, ES_SCHED_LOCAL};

private:
  /// The Scheduler implementation.
  POA_RtecScheduler::Scheduler *sched_impl_;

  /// The Event Channel implementation.
  POA_RtecEventChannelAdmin::EventChannel *ec_impl_;

  /// The name we use to bind with the NameService
  ACE_CString service_name_;

  /// The name of the file were we output the Event_Service IOR.
  ACE_TString ior_file_name_;

  /// The name of a file where the process stores its pid
  ACE_TString pid_file_name_;

  /// The name of the servant we use when we use persistent IORs
  ACE_CString object_id_;

  /// Should we use a global scheduler or a local one or none?
  Sched_type_t scheduler_type_;

  /// A reference to the ORB, to shut it down properly.
  CORBA::ORB_var orb_;

  /// Use BiDirectional GIOP
  bool use_bidir_giop_;

  /// Should we bind to the naming service
  bool bind_to_naming_service_;
};

#include /**/ "ace/post.h"
#endif /* EC_EVENT_SERVICE_H */
