/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace ORB
//
// = FILENAME
//    Event_Utilities
//
// = AUTHOR
//    Tim Harrison (harrison@cs.wustl.edu)
//
// ============================================================================

#ifndef ACE_EVENT_UTILITIES_H
#define ACE_EVENT_UTILITIES_H

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/orbsvcs_export.h"

class TAO_ORBSVCS_Export ACE_ConsumerQOS_Factory
{
  // = TITLE
  //    Consumer QOS Factory.
  //
  // = DESCRIPTION
  //
  //    This class allows easy (free from CORBA IDL constraints)
  //    construction of RtecEventChannelAdmin::ConsumerQOS structures.
  //
  // = CORRELATIONS
  //
  //    ACE_ConsumerQOS_Factory separates subscriptions into conjunction
  //    and disjunction groups.  A group can be thought of as a set of
  //    events inside parenthesis: (A+B+C), where A,B, and C are
  //    events.
  //
  //    The following code would be used to represent (A+B) | (B+C):
  //
  //    ACE_ConsumerQOS_Factor factory;
  //    factory.start_conjunction_group ();
  //    factory.insert (A);
  //    factory.insert (B);
  //    factory.start_conjunction_group ();
  //    factory.insert (B);
  //    factory.insert (C);
  //
  //    The following code would be used to represent (A|B) | (B|C):
  //
  //    ACE_ConsumerQOS_Factor factory;
  //    factory.start_disjunction_group ();
  //    factory.insert (A);
  //    factory.insert (B);
  //    factory.start_disjunction_group ();
  //    factory.insert (B);
  //    factory.insert (C);
  //
  //    First, this may not seem to be initially useful, as (A|B) |
  //    (B|C) seems the same as A|B|C.  However, this form does have a
  //    significant use when deadline timers are specified (described
  //    below). Note that groups end with the next call to
  //    start_XX_group.  Groups are always OR'd together.  That is,
  //    there is no way to directly build (A|B|C) + (D|E|F).  You can
  //    always expand the previous statement to the OR of multiple ANDs.
  //
  // = TIMEOUTS
  //
  //    There are two types of timeout types defined in
  //    Event_Service_Constants.h.
  //
  //    ACE_ES_EVENT_INTERVAL_TIMEOUT - the consumer wants to receive a
  //    timeout every N seconds.
  //
  //    ACE_ES_EVENT_DEADLINE_TIMEOUT - the consumer wants the timeout
  //    if and only if some dependencies are not resolved first.
  //
  //    Using these timeouts with the correlations discussed above, we
  //    can construct four different timer semantics: Interval Timer,
  //    Deadline Timer, Interval Correlation, Deadline Correlation:
  //
  //    Interval Timer:
  //
  //      (A+B+C) | (D+E+F) | (G+H+I) | IntervalTimeout
  //
  //      This registers to receive an interval timeout regardless of
  //      other dependencies.  Event if events occur, the interval
  //      timeout will still be sent.
  //
  //    Deadline Timer:
  //
  //      (A+B+C) | (D+E+F) | (G+H+I) | DeadlineTimeout
  //
  //      This registers to receive the deadline timeout ONLY if no
  //      other events occur.  If a single event is sent to the
  //      consumer, the timer is cancelled and rescheduled.
  //
  //    Deadline Correlation:
  //
  //      (A+B+C) | (D+E+F) | (G+H+DeadlineTimeout)
  //
  //      If G and H do not occur within DeadlineTimeout time, a
  //      deadline timeout is sent.  It is cancelled and rescheduled if G
  //      and H occur.
  //
  //    Interval Correlation:
  //
  //      (A+B+C) | (D+E+F) | (G+H+IntervalTimeout)
  //
  //      G+H+IntervalTimeout are sent ONLY after all have occurred.  If
  //      G+H occur, they are queued until IntervalTimeout occurs.  If
  //      IntervalTimeout occurs, it is queued until G+H occur.
public:
  ACE_ConsumerQOS_Factory (void);
  // Default construction.

  ~ACE_ConsumerQOS_Factory (void);
  // Death and destruction.

  int start_conjunction_group (void);
  // This signifies that the consumer needs the AND of all following
  // insert calls (up until the next start_XX_group call).  Returns 0
  // on success, -1 on failure.

  int start_disjunction_group (void);
  // This signifies that the consumer needs the OR of all following
  // insert calls (up until the next start_XX_group call).  Returns 0
  // on success, -1 on failure.

  // = Insert operations add to the current conjunction or disjunction
  // group.  These return 0 on success, -1 on failure.  Before insert
  // is called, a start_XX_group method should be called.  If a
  // start_XX_group method is not called, start_conjunction_group is
  // assumed.

  int insert (const RtecEventChannelAdmin::Dependency &subscribe);
  // Insert the <subscribe> structure describing the event and
  // receiving method into the current group.

  int insert (RtecEventComm::EventSourceID source,
              RtecEventComm::EventType type,
              RtecScheduler::handle_t rt_info);
  // Insert source/type dependency.  <source> of the event (may be
  // zero), <type> of the event.  <rt_info> describes the method that
  // will handle the <source>/<type> events.

  int insert_type (RtecEventComm::EventType type,
                   RtecScheduler::handle_t rt_info);
  // Insert type-only dependency.

  int insert_source (RtecEventComm::EventSourceID source,
                     RtecScheduler::handle_t rt_info);
  // Insert source-only dependency.

  int insert_time (RtecEventComm::EventType type,
                   RtecEventComm::Time interval,
                   RtecScheduler::handle_t rt_info);
  // Register temporal dependency.  <type> designates interval or
  // deadline timeout that will occur every <interval>.

  int insert_act (RtecEventComm::EventData act);
  // This will be inserted as type ACE_ES_EVENT_ACT.

  // = Conversion operators.  The Event Channel takes ConsumerQOS
  // objects.

  const RtecEventChannelAdmin::ConsumerQOS &get_ConsumerQOS (void);
  // Allows conversions to ConsumerQOS, which is expected by the
  // PushSupplierProxy::connect_push_consumer interface.

  operator const RtecEventChannelAdmin::ConsumerQOS &(void);
  // Calls this->get_ConsumerQOS.

  static void debug (const RtecEventChannelAdmin::ConsumerQOS& qos);

private:
  RtecEventChannelAdmin::ConsumerQOS qos_;
  // The representation to be sent to the channel.

  int designator_set_;
  // Whether a start_XX_group has been called yet.  This is to make
  // sure that a designator is placed in the subscription list first.
};

// ************************************************************

class TAO_ORBSVCS_Export ACE_SupplierQOS_Factory
{
public:
  ACE_SupplierQOS_Factory (void);
  // Default construction.

  int insert (RtecEventComm::EventSourceID sid,
              RtecEventComm::EventType type,
              RtecScheduler::handle_t rtinfo,
              u_int ncalls);
  // Publish <sid> and <type> that is generate by a method described by
  // <rtinfo>.  The method generates <type> <ncalls> number of times
  // per "iteration."

  const RtecEventChannelAdmin::SupplierQOS &get_SupplierQOS (void);
  // Allows conversions to SupplierQOS, which is expected by the
  // PushSupplierProxy::connect_push_supplier interface.

  operator const RtecEventChannelAdmin::SupplierQOS &(void);
  // Calls this->get_SupplierQOS.

  static void debug (const RtecEventChannelAdmin::SupplierQOS& qos);

private:
  RtecEventChannelAdmin::SupplierQOS qos_;
  // Representation needed by channel.
};


#if defined (__ACE_INLINE__)
#include "orbsvcs/Event_Utilities.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_EVENT_UTILITIES_H */
