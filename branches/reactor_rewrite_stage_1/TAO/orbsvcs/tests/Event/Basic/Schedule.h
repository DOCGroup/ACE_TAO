/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel testsuite
//
// = FILENAME
//   Schedule
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef EC_SCHEDULE_H
#define EC_SCHEDULE_H

#include "Driver.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecSchedulerS.h"

class EC_Schedule : public EC_Driver
{
  //
  // = TITLE
  //   Test the EC scheduling test
  //
  // = DESCRIPTION
  //   The EC can be used in conjunction with the scheduling service
  //   to analyze the schedulabity of a system and compute priority
  //   assignments that guarantee the correct behavior of it.
  //
  //   Most of the work is actually done by the scheduler (as it
  //   should be), the event channel simply plays two roles:
  //
  //   1) Feed the scheduler with the dependency information between
  //      consumers and suppliers based on their QoS requirements,
  //      subscriptions and publications.
  //
  //   2) At run-time use the scheduler information to dispatch the
  //      events at the right priority.
  //
  //   The current version only verifies the first role.
  //
public:
  EC_Schedule (void);
  // Constructor

  // = The EC_Driver methods
  virtual int parse_args (int& argc, char* argv[]);
  virtual void print_args (void) const;
  virtual void print_usage (void);
  // add some command line args to change the scheduling service to
  // use.

  void initialize_ec_impl (TAO_ENV_SINGLE_ARG_DECL);
  void cleanup_ec (void);
  virtual void modify_attributes (TAO_EC_Event_Channel_Attributes& attr);
  // Set the scheduling service attribute

  void execute_test (TAO_ENV_SINGLE_ARG_DECL);
  // Don't run the suppliers, just compute the schedule.

  void dump_results (void);
  // Don't dump the EC_Driver results, they are meaningless.

  virtual void build_consumer_qos (
      int i,
      RtecEventChannelAdmin::ConsumerQOS& qos,
      int& shutdown_event_type
      TAO_ENV_ARG_DECL_NOT_USED);
  virtual void build_supplier_qos (
      int i,
      RtecEventChannelAdmin::SupplierQOS& qos,
      int& shutdown_event_type
      TAO_ENV_ARG_DECL_NOT_USED);
  // This time really connect to the scheduler

private:
  POA_RtecScheduler::Scheduler *scheduler_impl_;
  // The scheduler implementation

  RtecScheduler::Scheduler_var scheduler_;
  // The scheduler object reference
};

#endif /* EC_SCHEDULE_H */
