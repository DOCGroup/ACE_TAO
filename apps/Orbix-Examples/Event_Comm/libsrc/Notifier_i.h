/* -*- C++ -*- */
// @(#)Notifier_i.h	1.1	10/18/96


// ============================================================================
//
// = LIBRARY
//    EventComm
// 
// = FILENAME
//    Notifier_i.h
//
// = DESCRIPTION 
//    Class interface for the implementation of the <Notifier>
//
// = AUTHOR
//    Douglas C. Schmidt (schmidt@cs.wustl.edu)
// 
// ============================================================================

#if !defined (_Notifier_i_H)
#define _Notifier_i_H

#include "ace/Map_Manager.h"
#include "ace/Synch.h"
#include "ace/SString.h"
#include "Event_Comm.hh"

#if defined (ACE_HAS_ORBIX)

// Forward reference.
class Notification_Receiver_Entry;

class Notifier_i
  // = TITLE
  //   Defines the implementation class for event <Notifiers>.
  //
  // = DESCRIPTION
{
public:
  enum 
  {
    DEFAULT_SIZE = 1024 // Default max number of Event_Comm::Notification_Receivers.
  };

  Notifier_i (size_t size_hint = Notifier_i::DEFAULT_SIZE);
  // Initialize a Notifier_i object with the specified size hint.

  void send_disconnect (const char *reason,
			CORBA::Environment &IT_env);
  // Disconnect all the receivers, giving them the <reason>.

  void send_notification (const Event_Comm::Notification &notification,
			  CORBA::Environment &IT_env);
  // Send the <Notification> to all the consumers who
  // have subscribed and who match the filtering criteria.

  void subscribe (Event_Comm::Notification_Receiver *notification_receiver,
		  const char *filtering_criteria,
		  CORBA::Environment &IT_env);
  // Subscribe the <Notification_Receiver> to receive events that 
  // match <filtering_criteria> applied by the <Notifier>.

  void unsubscribe (Event_Comm::Notification_Receiver *notification_receiver,
		    const char *filtering_criteria,
		    CORBA::Environment &IT_env);
  // Unsubscribe the <Notification_Receiver>.

private:
  // The following implementation should be replaced 
  // by a standard container class from STL...

  typedef ACE_Map_Manager <ACE_SString, Notification_Receiver_Entry *, ACE_Null_Mutex> MAP_MANAGER;
  typedef ACE_Map_Iterator <ACE_SString, Notification_Receiver_Entry *, ACE_Null_Mutex> MAP_ITERATOR;
  typedef ACE_Map_Entry <ACE_SString, Notification_Receiver_Entry *> MAP_ENTRY;

  MAP_MANAGER map_;
  // Table that maps a <Event_Comm::Notification_Receiver *> to a <Notification_Receiver_Entry *>.
};

#endif /* ACE_HAS_ORBIX */
#endif /* _Notifier_i_H */
