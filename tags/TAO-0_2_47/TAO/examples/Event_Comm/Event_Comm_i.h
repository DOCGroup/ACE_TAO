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
//    Douglas C. Schmidt (schmidt@cs.wustl.edu) and Pradeep Gore
//    <pradeep@cs.wustl.edu>
//
// ============================================================================

#ifndef _EVENT_COMM_I_H
#define _EVENT_COMM_I_H

#include "ace/Map_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch.h"
#include "ace/SString.h"
#include "Event_CommS.h"

class ShutdownCallback
{
  // = TITLE
  //   Helper callback class to shutdown the application.
public:
  virtual void close (void) = 0;
  // This method is to be called to shutdown the application.
};

class Consumer_i : public POA_Event_Comm::Consumer
{
  // = TITLE
  //   Defines the implementation class for event <Consumers>.
public:
  // = Initialization and termination methods.
  Consumer_i (void);
  // Constructor.

  ~Consumer_i (void);
  // Destructor.

  void set_reactor (ACE_Reactor *reactor);
  // set the <ACE_Reactor> to use when quitting.

  virtual void push (const Event_Comm::Event & event,
                     CORBA::Environment &TAO_TRY_ENV);
  // Pass the <event> to the <Consumer>.

  virtual void disconnect (const char * reason,
                           CORBA::Environment &TAO_TRY_ENV);
  // Disconnect the <Consumer> from the <Notifier>, giving it the
  // <reason>.

  void set (ShutdownCallback *_shutdown);
  // Set the Shutdown callback.

private:
  ShutdownCallback *shutdown;
  // The callback to shutdown the consumer application.
};

// Forward reference.
class Consumer_Entry;

class Notifier_i : public POA_Event_Comm::Notifier
{
  // = TITLE
  //   Defines the implementation class for event <Notifiers>.
public:
  enum
  {
    DEFAULT_SIZE = 1024
    // Default max number of Event_Comm::Consumers.
  };

  Notifier_i (size_t size_hint = Notifier_i::DEFAULT_SIZE);
  // Initialize a Notifier_i object with the specified size hint.

  virtual void disconnect (const char *reason,
                           CORBA::Environment &TAO_TRY_ENV);
  // Disconnect all the receivers, giving them the <reason>.

  virtual void push (const Event_Comm::Event &event,
                     CORBA::Environment &TAO_TRY_ENV);
  // Send the <event> to all the consumers who have subscribed and who
  // match the filtering criteria.

   virtual void subscribe (Event_Comm::Consumer_ptr Consumer,
                           const char * filtering_criteria,
                           CORBA::Environment &TAO_TRY_ENV);
  // Subscribe the <Consumer> to receive events that match
  // <filtering_criteria> applied by the <Notifier>.

 void unsubscribe (Event_Comm::Consumer *consumer,
                    const char *filtering_criteria,
                    CORBA::Environment &TAO_TRY_ENV);
  // Unsubscribe the <Consumer>.

private:
  typedef ACE_Map_Manager <Event_Comm::Consumer_ptr, Consumer_Entry *, ACE_Null_Mutex>
          MAP_MANAGER;
  typedef ACE_Map_Iterator <Event_Comm::Consumer_ptr, Consumer_Entry *, ACE_Null_Mutex>
          MAP_ITERATOR;
  typedef ACE_Map_Entry <Event_Comm::Consumer_ptr, Consumer_Entry *>
          MAP_ENTRY;

  MAP_MANAGER map_;
  // Table that maps a <Event_Comm::Consumer *> to a <Consumer_Entry *>.
};

#endif /* _EVENT_COMM_I_H */
