//=============================================================================
/**
 *  @file   Notify_Resource_Manager.h
 *
 *  $Id$
 *
 * Internal "observer" interfaces for Notify.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_NOTIFY_LISTENERS_H
#define TAO_NOTIFY_LISTENERS_H

#include "ace/pre.h"
#include "tao/corba.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "notify_export.h"

class TAO_Notify_Event;
class TAO_Notify_EventType_List;
class TAO_Notify_Worker_Task;

// @@ Pradeep: this file has three separate classes, that do not seem
// related in anyway, you should move them to their own files.
// @@ Pradeep: it seems like the classes in this file are fundamental
// interfaces used all over the place, you should document them and
// how they interact, in the README file or in the paper or both.

// @@ Pradeep: if this class is what I think it is then it should be
// in its own file..
 /**
  * @class TAO_Notify_RefCounted
  *
  * @brief TAO_Notify_RefCounted
  *
  * @@ Pradeep: what do you mean "often", is it used for something
  * else sometimes?  Or is it that sometimes refcounting is done
  * some other way?
  * Often used interface for ref. counting.
  */
class TAO_Notify_Export TAO_Notify_RefCounted
{
 public:
  /// Increment the reference count.
  virtual CORBA::ULong _incr_refcnt (void) = 0;

  /// Decrement the reference count.
  virtual CORBA::ULong _decr_refcnt (void) = 0;
};

// ****************************************************************

/**
 * @class TAO_Notify_EventListener
 *
 * @brief TAO_Notify_EventListener
 *
 * All entities interested in receiving events can implement this interface
 * and subscribe with the Event Manager. During subscription, the Listener
 * must specify what kind of events it is interested in.
 */
class  TAO_Notify_Export TAO_Notify_EventListener : virtual public TAO_Notify_RefCounted
{

public:
  /// Callback methods to supply the event to the listener.
  virtual void dispatch_event (TAO_Notify_Event &event
                               ACE_ENV_ARG_DECL) = 0;

  /**
   * Evaluates true if this event is acceptable by the listener.
   * The <eval_parent> is a hint to the listener to help it determine
   * if its wise to evaluate the parents filter too. This helps in
   * implementing the "interfilter group operator" logic.
   */
  virtual CORBA::Boolean evaluate_filter (TAO_Notify_Event &event,
                                          CORBA::Boolean eval_parent
                                          ACE_ENV_ARG_DECL) = 0;

  /// Ask the listener to relinquish any bindings and prepare to be
  /// disposed.
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL) = 0;

  /// The Worker task associated with the event listener for event dispatching
  virtual TAO_Notify_Worker_Task* event_dispatch_task (void) = 0;

  /// The Worker task associated with the event listener for filter evaluation.
  virtual TAO_Notify_Worker_Task* filter_eval_task (void) = 0;
};

// ****************************************************************

 /**
  * @class TAO_Notify_EventSource
  *
  * @brief TAO_Notify_EventSource
  *
  * The event source suppliers events to the Notify Manager.
  */
class TAO_Notify_Export TAO_Notify_EventSource : virtual public TAO_Notify_RefCounted
{
 public:
  // TODO: add a shutdown method to this interface!!

  /// Evaluates true if this event is acceptable by the Source.
  virtual CORBA::Boolean evaluate_filter (TAO_Notify_Event &event
                                          ACE_ENV_ARG_DECL) = 0;

  /// The Worker task associated with the event listener for filter evaluation.
  virtual TAO_Notify_Worker_Task* filter_eval_task (void) = 0;
};

// ****************************************************************

/**
 * @class TAO_Notify_UpdateListener
 *
 * @brief TAO_Notify_UpdateListener
 *
 * All entities interested in receiving subscription and publication
 * change messages can implement this interface and register it with
 * the Event Manager to receive updates.
 */
class TAO_Notify_Export TAO_Notify_UpdateListener : virtual public TAO_Notify_RefCounted
{

 public:
  /// Callback method to supply updates.
  virtual void dispatch_update (TAO_Notify_EventType_List& added,
                                TAO_Notify_EventType_List& removed
                                ACE_ENV_ARG_DECL) = 0;
};

#include "ace/post.h"
#endif /* TAO_NOTIFY_LISTENERS_H */
