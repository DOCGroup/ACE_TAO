// $Id$
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

#include "Notify_Types.h"

class TAO_Notify_Event_Listener
{
  // = TITLE
  //   TAO_Notify_Event_Listener
  //
  // = DESCRIPTION
  //   An entities interested in receiving events can implement this interface
  //   and subscribe with the Event Manager. During subscription, the Listener
  //   must specify what kind of events it is interested in.
  //
 public:
  virtual void dispatch_event (const CORBA::Any & data, CORBA::Environment &ACE_TRY_ENV) = 0;
  virtual void dispatch_event (const CosNotification::StructuredEvent & notification, CORBA::Environment &ACE_TRY_ENV) = 0;
  // Callback methods to supply the event to the listener.
};

class TAO_Notify_Update_Listener
{
 public:
  // = TITLE
  //   TAO_Notify_Update_Listener
  //
  // = DESCRIPTION
  //   All entities interested in receiving subscription and publication
  //   change messages can implement this interface and register it with
  //   the Event Manager to receive updates.
  //
  virtual void dispatch_update (EVENTTYPE_LIST& added, EVENTTYPE_LIST& removed) = 0;
  // Callback method to supply updates.
};

#endif /* TAO_NOTIFY_LISTENERS_H */
