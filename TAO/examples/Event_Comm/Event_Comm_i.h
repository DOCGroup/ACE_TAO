//=============================================================================
/**
 *  @file    Event_Comm_i.h
 *
 *  Class interface for the implementation of the distributed
 *  event notification mechanism.
 *
 *  @author Douglas C. Schmidt (d.schmidt@vanderbilt.edu) and Pradeep Gore <pradeep@cs.wustl.edu>
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
  virtual ~ShutdownCallback () = default;

  /// This method is to be called to shutdown the application.
  virtual void close () = 0;
};

/**
 * @class Consumer_i
 *
 * @brief Defines the implementation class for event <Consumers>.
 */
class Consumer_i : public POA_Event_Comm::Consumer
{
public:
  /// Constructor.
  Consumer_i () = default;

  /// Destructor.
  ~Consumer_i () override = default;

  Consumer_i (Consumer_i const &) = delete;
  Consumer_i &operator= (Consumer_i const &) = delete;
  Consumer_i (Consumer_i &&) = delete;
  Consumer_i &operator= (Consumer_i &&) = delete;

  /// set the ACE_Reactor to use when quitting.
  void set_reactor (ACE_Reactor *reactor);

  /// Pass the q event to the <Consumer>.
  void push (Event_Comm::Event const &event) override;

  // Disconnect the <Consumer> from the <Notifier>, giving it the
  // @a reason.
  void disconnect (char const *reason) override;

  /// Set the Shutdown callback.
  void set (ShutdownCallback *shutdown_callback);

private:
  /// The callback to shutdown the consumer application.
  ShutdownCallback *shutdown_ {};
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
  static constexpr size_t DEFAULT_SIZE = 1024;

  /// Initialize a Notifier_i object with the specified size hint.
  explicit Notifier_i (size_t size_hint = Notifier_i::DEFAULT_SIZE);

  Notifier_i (Notifier_i const &) = delete;
  Notifier_i &operator= (Notifier_i const &) = delete;
  Notifier_i (Notifier_i &&) = delete;
  Notifier_i &operator= (Notifier_i &&) = delete;

  /// Disconnect all the receivers, giving them the @a reason.
  void disconnect (char const *reason) override;

  /// Send the @a event to all the consumers who have subscribed and who
  /// match the filtering criteria.
  void push (Event_Comm::Event const &event) override;

  /// Subscribe the @a consumer to receive events that match
  /// @a filtering_criteria applied by the <Notifier>.
  void subscribe (Event_Comm::Consumer_ptr consumer,
                  char const *filtering_criteria) override;

  /// Unsubscribe the @a Consumer.
  void unsubscribe (Event_Comm::Consumer *consumer,
                    char const *filtering_criteria);

private:
  using MAP_MANAGER = ACE_Map_Manager <Event_Comm::Consumer_ptr,
                                       Consumer_Entry *,
                                       ACE_Null_Mutex>;
  using MAP_ITERATOR = ACE_Map_Iterator <Event_Comm::Consumer_ptr,
                                         Consumer_Entry *,
                                         ACE_Null_Mutex>;
  using MAP_ENTRY = ACE_Map_Entry <Event_Comm::Consumer_ptr, Consumer_Entry *>;

  /// Table that maps a <Event_Comm::Consumer *> to a <Consumer_Entry *>.
  MAP_MANAGER map_;
};

#endif /* _EVENT_COMM_I_H */
