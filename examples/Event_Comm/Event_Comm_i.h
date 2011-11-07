/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Event_Comm_i.h
 *
 *  $Id$
 *
 *  Class interface for the implementation of the distributed
 *  event notification mechanism.
 *
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu) and Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef _EVENT_COMM_I_H
#define _EVENT_COMM_I_H

#include "ace/Map_Manager.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch.h"
#include "ace/SString.h"
#include "Event_CommS.h"
#include "ace/Reactor.h"
#include "ace/Null_Mutex.h"

/**
 * @class ShutdownCallback
 *
 * @brief Helper callback class to shutdown the application.
 */
class ShutdownCallback
{
public:
  /// Destructor.
  virtual ~ShutdownCallback (void);

  /// This method is to be called to shutdown the application.
  virtual void close (void) = 0;
};

/**
 * @class Consumer_i
 *
 * @brief Defines the implementation class for event <Consumers>.
 */
class Consumer_i : public POA_Event_Comm::Consumer
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  Consumer_i (void);

  /// Destructor.
  ~Consumer_i (void);

  /// set the <ACE_Reactor> to use when quitting.
  void set_reactor (ACE_Reactor *reactor);

  /// Pass the <event> to the <Consumer>.
  virtual void push (const Event_Comm::Event & event);

  virtual void disconnect (const char * reason);

  // Disconnect the <Consumer> from the <Notifier>, giving it the
  // <reason>.

  /// Set the Shutdown callback.
  void set (ShutdownCallback *_shutdown);

private:
  /// The callback to shutdown the consumer application.
  ShutdownCallback *shutdown;
};

// Forward reference.
class Consumer_Entry;

/**
 * @class Notifier_i
 *
 * @brief Defines the implementation class for event <Notifiers>.
 */
class Notifier_i : public POA_Event_Comm::Notifier
{
public:
  enum
  {
    DEFAULT_SIZE = 1024
    // Default max number of Event_Comm::Consumers.
  };

  /// Initialize a Notifier_i object with the specified size hint.
  Notifier_i (size_t size_hint = Notifier_i::DEFAULT_SIZE);

  /// Disconnect all the receivers, giving them the <reason>.
  virtual void disconnect (const char *reason);

  /// Send the <event> to all the consumers who have subscribed and who
  /// match the filtering criteria.
  virtual void push (const Event_Comm::Event &event);

  /// Subscribe the <Consumer> to receive events that match
  /// <filtering_criteria> applied by the <Notifier>.
   virtual void subscribe (Event_Comm::Consumer_ptr Consumer,
                           const char * filtering_criteria);

  /// Unsubscribe the <Consumer>.
 void unsubscribe (Event_Comm::Consumer *consumer,
                    const char *filtering_criteria);

private:
  typedef ACE_Map_Manager <Event_Comm::Consumer_ptr, Consumer_Entry *, ACE_Null_Mutex>
          MAP_MANAGER;
  typedef ACE_Map_Iterator <Event_Comm::Consumer_ptr, Consumer_Entry *, ACE_Null_Mutex>
          MAP_ITERATOR;
  typedef ACE_Map_Entry <Event_Comm::Consumer_ptr, Consumer_Entry *>
          MAP_ENTRY;

  /// Table that maps a <Event_Comm::Consumer *> to a <Consumer_Entry *>.
  MAP_MANAGER map_;
};

#endif /* _EVENT_COMM_I_H */
