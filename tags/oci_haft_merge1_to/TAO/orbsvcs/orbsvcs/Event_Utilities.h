/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Event_Utilities.h
 *
 *  $Id$
 *
 *  @author Tim Harrison (harrison@cs.wustl.edu)
 */
//=============================================================================


#ifndef ACE_EVENT_UTILITIES_H
#define ACE_EVENT_UTILITIES_H
#include "ace/pre.h"

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "Event/event_export.h"

typedef void (*TAO_EC_Event_Initializer) (RtecEventComm::Event&);

/**
 * @class ACE_ConsumerQOS_Factory
 *
 * @brief Consumer QOS Factory.
 *
 * This class allows easy (free from CORBA IDL constraints)
 * construction of RtecEventChannelAdmin::ConsumerQOS structures.
 * = CORRELATIONS
 * ACE_ConsumerQOS_Factory separates subscriptions into conjunction
 * and disjunction groups.  A group can be thought of as a set of
 * events inside parenthesis: (A+B+C), where A,B, and C are
 * events.
 * The following code would be used to represent (A+B) | (B+C):
 * ACE_ConsumerQOS_Factor factory;
 * factory.start_conjunction_group ();
 * factory.insert (A);
 * factory.insert (B);
 * factory.start_conjunction_group ();
 * factory.insert (B);
 * factory.insert (C);
 * The following code would be used to represent (A|B) | (B|C):
 * ACE_ConsumerQOS_Factor factory;
 * factory.start_disjunction_group ();
 * factory.insert (A);
 * factory.insert (B);
 * factory.start_disjunction_group ();
 * factory.insert (B);
 * factory.insert (C);
 * First, this may not seem to be initially useful, as (A|B) |
 * (B|C) seems the same as A|B|C.  However, this form does have a
 * significant use when deadline timers are specified (described
 * below). Note that groups end with the next call to
 * start_XX_group.  Groups are always OR'd together.  That is,
 * there is no way to directly build (A|B|C) + (D|E|F).  You can
 * always expand the previous statement to the OR of multiple ANDs.
 * = TIMEOUTS
 * There are two types of timeout types defined in
 * Event_Service_Constants.h.
 * ACE_ES_EVENT_INTERVAL_TIMEOUT - the consumer wants to receive a
 * timeout every N seconds.
 * ACE_ES_EVENT_DEADLINE_TIMEOUT - the consumer wants the timeout
 * if and only if some dependencies are not resolved first.
 * Using these timeouts with the correlations discussed above, we
 * can construct four different timer semantics: Interval Timer,
 * Deadline Timer, Interval Correlation, Deadline Correlation:
 * Interval Timer:
 * (A+B+C) | (D+E+F) | (G+H+I) | IntervalTimeout
 * This registers to receive an interval timeout regardless of
 * other dependencies.  Event if events occur, the interval
 * timeout will still be sent.
 * Deadline Timer:
 * (A+B+C) | (D+E+F) | (G+H+I) | DeadlineTimeout
 * This registers to receive the deadline timeout ONLY if no
 * other events occur.  If a single event is sent to the
 * consumer, the timer is cancelled and rescheduled.
 * Deadline Correlation:
 * (A+B+C) | (D+E+F) | (G+H+DeadlineTimeout)
 * If G and H do not occur within DeadlineTimeout time, a
 * deadline timeout is sent.  It is cancelled and rescheduled if G
 * and H occur.
 * Interval Correlation:
 * (A+B+C) | (D+E+F) | (G+H+IntervalTimeout)
 * G+H+IntervalTimeout are sent ONLY after all have occurred.  If
 * G+H occur, they are queued until IntervalTimeout occurs.  If
 * IntervalTimeout occurs, it is queued until G+H occur.
 */
class TAO_RTEvent_Export ACE_ConsumerQOS_Factory
{
public:
  /// Default construction.
  ACE_ConsumerQOS_Factory (TAO_EC_Event_Initializer initializer = 0);

  /// Death and destruction.
  ~ACE_ConsumerQOS_Factory (void);

  /**
   * The Event Channel waits until all the children have accepted at
   * least one event, and then send them all as a single event to the
   * consumer.
   */
  int start_conjunction_group (int nchildren = 0);

  /// The consumer accepts any event that is accepted by at least one
  /// child.
  int start_disjunction_group (int nchildren = 0);

  /// The consumer only accepts events that pass all the filter
  /// expressions defined by the children.
  int start_logical_and_group (int nchildren = 0);

  /// The consumer wants all the events *except* the group that
  /// follows.
  int start_negation (void);

  /// Insert a bitmask filter, this acts as a quick rejection mechanism
  /// for the subsequent filters.
  int start_bitmask (CORBA::ULong source_mask,
                     CORBA::ULong type_mask);

  /**
   * Inser a new filter that only accepts events with the following
   * properties:
   * (event.header.type & type_mask) == type_value
   * (event.header.source & source_mask) == source_value
   */
  int insert_bitmasked_value (CORBA::ULong source_mask,
                              CORBA::ULong type_mask,
                              CORBA::ULong source_value,
                              CORBA::ULong type_value);

  /// Insert a node that accepts any event, useful for bitmask filters.
  int insert_null_terminator (void);

  // = Insert operations add to the current conjunction or disjunction
  // group.  These return 0 on success, -1 on failure.  Before insert
  // is called, a start_XX_group method should be called.  If a
  // start_XX_group method is not called, start_conjunction_group is
  // assumed.

  /// Insert the <subscribe> structure describing the event and
  /// receiving method into the current group.
  int insert (const RtecEventChannelAdmin::Dependency &subscribe);

  /**
   * Insert source/type dependency.  <source> of the event (may be
   * zero), <type> of the event.  <rt_info> describes the method that
   * will handle the <source>/<type> events.
   */
  int insert (RtecEventComm::EventSourceID source,
              RtecEventComm::EventType type,
              RtecBase::handle_t rt_info);

  /// Insert type-only dependency.
  int insert_type (RtecEventComm::EventType type,
                   RtecBase::handle_t rt_info);

  /// Insert source-only dependency.
  int insert_source (RtecEventComm::EventSourceID source,
                     RtecBase::handle_t rt_info);

  /// Register temporal dependency.  <type> designates interval or
  /// deadline timeout that will occur every <interval>.
  int insert_time (RtecEventComm::EventType type,
                   RtecEventComm::Time interval,
                   RtecBase::handle_t rt_info);

  /// This will be inserted as type ACE_ES_EVENT_ACT.
  int insert_act (RtecEventComm::EventData act);

  // = Conversion operators.  The Event Channel takes ConsumerQOS
  // objects.

  /// Allows conversions to ConsumerQOS, which is expected by the
  /// PushSupplierProxy::connect_push_consumer interface.
  const RtecEventChannelAdmin::ConsumerQOS &get_ConsumerQOS (void);

  /// Calls this->get_ConsumerQOS.
  operator const RtecEventChannelAdmin::ConsumerQOS &(void);

  static void debug (const RtecEventChannelAdmin::ConsumerQOS& qos);

private:
  /// The representation to be sent to the channel.
  RtecEventChannelAdmin::ConsumerQOS qos_;

  /// Whether a start_XX_group has been called yet.  This is to make
  /// sure that a designator is placed in the subscription list first.
  int designator_set_;

  /**
   * If not zero this is a user-provided function used to initialize
   * the events.  When the event contains unions this is required to
   * avoid marshaling and demarshaling of default initialized unions
   * that (AFAIK) is not CORBA compliant.
   */
  TAO_EC_Event_Initializer event_initializer_;
};

// ************************************************************

class TAO_RTEvent_Export ACE_SupplierQOS_Factory
{
public:
  /// Default construction.
  ACE_SupplierQOS_Factory (TAO_EC_Event_Initializer initializer = 0,
                           int qos_max_len = 0);

  /**
   * Publish <sid> and <type> that is generate by a method described by
   * <rtinfo>.  The method generates <type> <ncalls> number of times
   * per "iteration."
   */
  int insert (RtecEventComm::EventSourceID sid,
              RtecEventComm::EventType type,
              RtecBase::handle_t rtinfo,
              u_int ncalls);

  /// Allows conversions to SupplierQOS, which is expected by the
  /// PushSupplierProxy::connect_push_supplier interface.
  const RtecEventChannelAdmin::SupplierQOS &get_SupplierQOS (void);

  /// Calls this->get_SupplierQOS.
  operator const RtecEventChannelAdmin::SupplierQOS &(void);

  static void debug (const RtecEventChannelAdmin::SupplierQOS& qos);

private:
  /// Representation needed by channel.
  RtecEventChannelAdmin::SupplierQOS qos_;

  /**
   * If not zero this is a user-provided function used to initialize
   * the events.  When the event contains unions this is required to
   * avoid marshaling and demarshaling of default initialized unions
   * that (AFAIK) is not CORBA compliant.
   */
  TAO_EC_Event_Initializer event_initializer_;
};


#if defined (__ACE_INLINE__)
#include "orbsvcs/Event_Utilities.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_EVENT_UTILITIES_H */
