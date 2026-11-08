/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Schedule.h
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
  EC_Schedule ();

  // = The EC_Driver methods
  /// add some command line args to change the scheduling service to
  /// use.
  int parse_args (int& argc, ACE_TCHAR* argv[]) override;
  void print_args () const override;
  void print_usage () override;

  /// Set the scheduling service attribute
  void initialize_ec_impl ();
  void cleanup_ec ();
  void modify_attributes (TAO_EC_Event_Channel_Attributes& attr) override;

  /// Don't run the suppliers, just compute the schedule.
  void execute_test () override;

  /// Don't dump the EC_Driver results, they are meaningless.
  void dump_results () override;

  /// This time really connect to the scheduler
  void build_consumer_qos (
      int i,
      RtecEventChannelAdmin::ConsumerQOS& qos,
      int& shutdown_event_type) override;
  void build_supplier_qos (
      int i,
      RtecEventChannelAdmin::SupplierQOS& qos,
      int& shutdown_event_type) override;

private:
  /// The scheduler implementation
  POA_RtecScheduler::Scheduler *scheduler_impl_;

  /// The scheduler object reference
  RtecScheduler::Scheduler_var scheduler_;
};

#endif /* EC_SCHEDULE_H */
