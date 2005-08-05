/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Schedule.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef EC_SCHEDULE_H
#define EC_SCHEDULE_H

#include "Driver.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/RtecSchedulerS.h"

/**
 * @class EC_Schedule
 *
 * @brief Test the EC scheduling test
 *
 * The EC can be used in conjunction with the scheduling service
 * to analyze the schedulabity of a system and compute priority
 * assignments that guarantee the correct behavior of it.
 * Most of the work is actually done by the scheduler (as it
 * should be), the event channel simply plays two roles:
 * 1) Feed the scheduler with the dependency information between
 * consumers and suppliers based on their QoS requirements,
 * subscriptions and publications.
 * 2) At run-time use the scheduler information to dispatch the
 * events at the right priority.
 * The current version only verifies the first role.
 */
class EC_Schedule : public EC_Driver
{
public:
  /// Constructor
  EC_Schedule (void);

  // = The EC_Driver methods
  /// add some command line args to change the scheduling service to
  /// use.
  virtual int parse_args (int& argc, char* argv[]);
  virtual void print_args (void) const;
  virtual void print_usage (void);

  /// Set the scheduling service attribute
  void initialize_ec_impl (ACE_ENV_SINGLE_ARG_DECL);
  void cleanup_ec (void);
  virtual void modify_attributes (TAO_EC_Event_Channel_Attributes& attr);

  /// Don't run the suppliers, just compute the schedule.
  void execute_test (ACE_ENV_SINGLE_ARG_DECL);

  /// Don't dump the EC_Driver results, they are meaningless.
  void dump_results (void);

  /// This time really connect to the scheduler
  virtual void build_consumer_qos (
      int i,
      RtecEventChannelAdmin::ConsumerQOS& qos,
      int& shutdown_event_type
      ACE_ENV_ARG_DECL_NOT_USED);
  virtual void build_supplier_qos (
      int i,
      RtecEventChannelAdmin::SupplierQOS& qos,
      int& shutdown_event_type
      ACE_ENV_ARG_DECL_NOT_USED);

private:
  /// The scheduler implementation
  POA_RtecScheduler::Scheduler *scheduler_impl_;

  /// The scheduler object reference
  RtecScheduler::Scheduler_var scheduler_;
};

#endif /* EC_SCHEDULE_H */
