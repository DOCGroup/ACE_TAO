/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    EventComm
//
// = FILENAME
//    Event_Comm_i.h
//
// = DESCRIPTION
//    Class interface for the implementation of the distributed
//    event notification mechanism.
//
// = AUTHOR
//    Douglas C. Schmidt (schmidt@cs.wustl.edu)
//
// ============================================================================

#if !defined (_EVENT_COMM_I_H)
#define _EVENT_COMM_I_H

#include "ace/Map_Manager.h"
#include "ace/Synch.h"
#include "ace/SString.h"

class Consumer_i
{
  // = TITLE
  //   Defines the implementation class for event <Consumers>.
public:
  // = Initialization and termination methods.
  Consumer_i (void);
  // Constructor.

  ~Consumer_i (void);
  // Destructor.

  virtual void push (const Event_Comm::Event &event,
		     CORBA::Environment &IT_env);
  // Pass the <event> to the <Consumer>.

  virtual void disconnect (const char *reason,
			   CORBA::Environment &IT_env);
  // Disconnect the <Consumer> from the <Notifier>,
  // giving it the <reason>.
};

// Forward reference.
class Consumer_Entry;

class Notifier_i
{
  // = TITLE
  //   Defines the implementation class for event <Notifiers>.
public:
  enum
  {
    DEFAULT_SIZE = 1024 // Default max number of Event_Comm::Consumers.
  };

  Notifier_i (size_t size_hint = Notifier_i::DEFAULT_SIZE);
  // Initialize a Notifier_i object with the specified size hint.

  void disconnect (const char *reason,
		   CORBA::Environment &IT_env);
  // Disconnect all the receivers, giving them the <reason>.

  void push (const Event_Comm::Event &event,
	     CORBA::Environment &IT_env);
  // Send the <event> to all the consumers who have subscribed
  // and who match the filtering criteria.

  void subscribe (Event_Comm::Consumer *consumer,
		  const char *filtering_criteria,
		  CORBA::Environment &IT_env);
  // Subscribe the <Consumer> to receive events that match
  // <filtering_criteria> applied by the <Notifier>.

  void unsubscribe (Event_Comm::Consumer *consumer,
		    const char *filtering_criteria,
		    CORBA::Environment &IT_env);
  // Unsubscribe the <Consumer>.

private:
  typedef ACE_Map_Manager <ACE_SString, Consumer_Entry *, ACE_Null_Mutex>
          MAP_MANAGER;
  typedef ACE_Map_Iterator <ACE_SString, Consumer_Entry *, ACE_Null_Mutex>
          MAP_ITERATOR;
  typedef ACE_Map_Entry <ACE_SString, Consumer_Entry *>
          MAP_ENTRY;

  MAP_MANAGER map_;
  // Table that maps a <Event_Comm::Consumer *> to a <Consumer_Entry
  // *>.
};

#endif /* _EVENT_COMM_I_H */
