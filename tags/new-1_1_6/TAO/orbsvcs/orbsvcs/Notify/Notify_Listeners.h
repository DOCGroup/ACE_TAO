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

class TAO_Notify_Export TAO_Notify_RefCounted
{
  // = TITLE
  //   TAO_Notify_RefCounted
  //
  // = DESCRIPTION
  //   Often used interface for ref. counting.
  //
 public:
  virtual CORBA::ULong _incr_refcnt (void) = 0;
  virtual CORBA::ULong _decr_refcnt (void) = 0;
  // Increment and decrement the reference count.
};

/**************************************************************************************************/

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
  virtual void dispatch_event (TAO_Notify_Event &event, CORBA::Environment &ACE_TRY_ENV) = 0;
  // Callback methods to supply the event to the listener.

  virtual CORBA::Boolean evaluate_filter (TAO_Notify_Event &event, CORBA::Boolean eval_parent, CORBA::Environment &ACE_TRY_ENV) = 0;
  // Evaluates true if this event is acceptable by the listener.
  // The <eval_parent> is a hint to the listener to help it determine if its wise to evaluate
  // the parents filter too.This helps in implementing the "interfilter group operator" logic.

  virtual void shutdown (CORBA::Environment &ACE_TRY_ENV) = 0;
  // Ask the listener to relinqish any bindings and prepare to be disposed.
};

/**************************************************************************************************/

class TAO_Notify_Export TAO_Notify_EventSource : virtual public TAO_Notify_RefCounted
{
  // = TITLE
  //   TAO_Notify_EventSource
  //
  // = DESCRIPTION
  //   The event source suppliers events to the Notify Manager.
  //
 public:
  virtual CORBA::Boolean evaluate_filter (TAO_Notify_Event &event, CORBA::Environment &ACE_TRY_ENV) = 0;
  // Evaluates true if this event is acceptable by the Source.
};

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
  virtual void dispatch_update (TAO_Notify_EventType_List& added, TAO_Notify_EventType_List& removed, CORBA::Environment &ACE_TRY_ENV) = 0;
  // Callback method to supply updates.
};

#include "ace/post.h"
#endif /* TAO_NOTIFY_LISTENERS_H */
