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
//   // Internal types used by Notify
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef TAO_NOTIFY_TYPES_H
#define TAO_NOTIFY_TYPES_H

#include "orbsvcs/CosNotificationS.h"
#include "ace/Containers_T.h"

class TAO_Notify_Event_Listener;
class TAO_Notify_Update_Listener;

class TAO_Notify_EventType
{
  // = TITLE
  //   TAO_Notify_EventType
  //
  // = DESCRIPTION
  //   This type is used to compare different event types.
  //   It is used by the Event Manager as a key to find subscription lists.
  //
 public:
  // = Initialization and termination
  TAO_Notify_EventType (void);
  TAO_Notify_EventType (const char* domain_name, const char* type_name);
  // Constuctor

  ~TAO_Notify_EventType ();
  // Destructor

  u_long hash (void) const;
  // hash value

  void operator=(const CosNotification::EventType& event_type);
  // Assignment from CosNotification::EventType

  int operator==(const TAO_Notify_EventType& notify_event_type) const;
  // == operator

  CosNotification::EventType event_type_;

  protected:
  void recompute_hash (void);
  // Recompute the hash value.

  u_long hash_value_;
  // The hash value computed.
};

// = typedefs
typedef ACE_Unbounded_Set <TAO_Notify_EventType> EVENTTYPE_LIST;
typedef ACE_Unbounded_Set_Iterator<TAO_Notify_EventType> EVENTTYPE_LIST_ITER;

// Type defines
typedef ACE_Unbounded_Set<TAO_Notify_Event_Listener*> EVENT_LISTENER_LIST;
typedef ACE_Unbounded_Set_Iterator<TAO_Notify_Event_Listener*>
EVENT_LISTENER_LIST_ITER;
// A list of proxy suppliers that are looking for the same event type.

typedef ACE_Unbounded_Set<TAO_Notify_Update_Listener*> UPDATE_LISTENER_LIST;
typedef ACE_Unbounded_Set_Iterator<TAO_Notify_Update_Listener*>
UPDATE_LISTENER_LIST_ITER;

#endif /* TAO_NOTIFY_TYPES_H */
