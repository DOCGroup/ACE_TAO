// $Id$
// ==========================================================================
//
// = LIBRARY
//   Orbsvcs
//
// = FILENAME
//   Notify_Resource_Manager.h
//
// = DESCRIPTION
//   Internal "observer" interfaces for Notify.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

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
class TAO_Notify_Export TAO_Notify_RefCounted
{
  // = TITLE
  //   TAO_Notify_RefCounted
  //
  // = DESCRIPTION
  //   @@ Pradeep: what do you mean "often", is it used for something
  //   else sometimes?  Or is it that sometimes refcounting is done
  //   some other way?
  //   Often used interface for ref. counting.
  //
 public:
  virtual CORBA::ULong _incr_refcnt (void) = 0;
  virtual CORBA::ULong _decr_refcnt (void) = 0;
  // Increment and decrement the reference count.
};

// ****************************************************************

class  TAO_Notify_Export TAO_Notify_EventListener : virtual public TAO_Notify_RefCounted
{
  // = TITLE
  //   TAO_Notify_EventListener
  //
  // = DESCRIPTION
  //   All entities interested in receiving events can implement this interface
  //   and subscribe with the Event Manager. During subscription, the Listener
  //   must specify what kind of events it is interested in.
  //

public:
  virtual void dispatch_event (TAO_Notify_Event &event
                               ACE_ENV_ARG_DECL) = 0;
  // Callback methods to supply the event to the listener.

  virtual CORBA::Boolean evaluate_filter (TAO_Notify_Event &event,
                                          CORBA::Boolean eval_parent
                                          ACE_ENV_ARG_DECL) = 0;
  // Evaluates true if this event is acceptable by the listener.
  // The <eval_parent> is a hint to the listener to help it determine
  // if its wise to evaluate the parents filter too. This helps in
  // implementing the "interfilter group operator" logic.

  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL) = 0;
  // Ask the listener to relinquish any bindings and prepare to be
  // disposed.

  virtual TAO_Notify_Worker_Task* event_dispatch_task (void) = 0;
  // The Worker task associated with the event listener for event dispatching

  virtual TAO_Notify_Worker_Task* filter_eval_task (void) = 0;
  // The Worker task associated with the event listener for filter evaluation.
};

// ****************************************************************

class TAO_Notify_Export TAO_Notify_EventSource : virtual public TAO_Notify_RefCounted
{
  // = TITLE
  //   TAO_Notify_EventSource
  //
  // = DESCRIPTION
  //   The event source suppliers events to the Notify Manager.
  //
 public:
  // TODO: add a shutdown method to this interface!!

  virtual CORBA::Boolean evaluate_filter (TAO_Notify_Event &event
                                          ACE_ENV_ARG_DECL) = 0;
  // Evaluates true if this event is acceptable by the Source.

  virtual TAO_Notify_Worker_Task* filter_eval_task (void) = 0;
  // The Worker task associated with the event listener for filter evaluation.
};

// ****************************************************************

class TAO_Notify_Export TAO_Notify_UpdateListener : virtual public TAO_Notify_RefCounted
{
  // = TITLE
  //   TAO_Notify_UpdateListener
  //
  // = DESCRIPTION
  //   All entities interested in receiving subscription and publication
  //   change messages can implement this interface and register it with
  //   the Event Manager to receive updates.
  //

 public:
  virtual void dispatch_update (TAO_Notify_EventType_List& added,
                                TAO_Notify_EventType_List& removed
                                ACE_ENV_ARG_DECL) = 0;
  // Callback method to supply updates.
};

#include "ace/post.h"
#endif /* TAO_NOTIFY_LISTENERS_H */
