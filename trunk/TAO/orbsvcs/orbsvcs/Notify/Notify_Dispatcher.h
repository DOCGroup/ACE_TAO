/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_Dispatcher.h
//
// = DESCRIPTION
//   Various dispatching strategies are defined here.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef NOTIFY_DISPATCHER_H_
#define NOTIFY_DISPATCHER_H_

#include "ace/Containers.h"
#include "tao/corbafwd.h"
#include "orbsvcs/orbsvcs/CosNotifyCommC.h"

class TAO_Notify_FilterAdmin_i;

class TAO_Notify_Dispatcher
{
  // = TITLE
  //   TAO_Notify_Dispatcher
  // = DESCRIPTION
  //   This is a base class for for different kinds of dispatching schemes.
  //   Dispatchers will have an internel criteria which will determine *if*
  //   (i,e. filtering) and *when* (i,e. qos) events will be forwarded to
  //   contained Dispatchers.

 public:
  // = Initialization and termination
  TAO_Notify_Dispatcher (void);
  // Constructor

  virtual ~TAO_Notify_Dispatcher (void);
  // Destructor

  static TAO_Notify_Dispatcher* create (CORBA::Environment &ACE_TRY_ENV);
  // Factory method to create dispatchers of various types.

  void set_FilterAdmin (TAO_Notify_FilterAdmin_i* filter_admin);
  // Set the filter admin for this dispatcher.

  // = Clients of this class should call these methods to dispatch events.
  virtual void dispatch_event (const CORBA::Any & data,
                               CORBA::Environment &ACE_TRY_ENV) = 0;

  virtual void dispatch_event (const CosNotification::StructuredEvent& event,
                               CORBA::Environment &ACE_TRY_ENV) = 0;

  int add_dispatcher (TAO_Notify_Dispatcher& dispatcher);
  // Add a dispatcher to an internal list of event recipients.

  int remove_dispatcher (TAO_Notify_Dispatcher& dispatcher);
  //Remove a dispatcher from the internal list.

 protected:
  TAO_Notify_FilterAdmin_i* filter_admin_;

  typedef ACE_Unbounded_Set<TAO_Notify_Dispatcher*> DISPATCHER_SET;
  typedef ACE_Unbounded_Set_Iterator<TAO_Notify_Dispatcher*>
    DISPATCHER_SET_ITER;

  DISPATCHER_SET contained_;
  // Dispatchers forward events to their "contained" dispatchers.
};

class Notify_Reactive_Dispatcher : public TAO_Notify_Dispatcher
{
  // = TITLE
  //
  // = DESCRIPTION
  //   Simplest dispatcher - dispatches event in the callers thread.
  //
  virtual void dispatch_event (const CORBA::Any & data,
                               CORBA::Environment &ACE_TRY_ENV);

  virtual void dispatch_event (const CosNotification::StructuredEvent& event,
                               CORBA::Environment &ACE_TRY_ENV);
};

#endif /* NOTIFY_DISPATCHER_H_ */
