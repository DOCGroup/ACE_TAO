// -*- C++ -*-

/**
 *  @file Reactive_Task.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_REACTIVE_TASK_H
#define TAO_Notify_REACTIVE_TASK_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Worker_Task.h"
#include "orbsvcs/Notify/AdminProperties.h"
#include "orbsvcs/Notify/Timer_Reactor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Notify_Reactive_Task
 *
 * @brief A reactive worker task. Simply executes the command in the
 *        caller's context.
 */
class TAO_Notify_Serv_Export TAO_Notify_Reactive_Task
  : public TAO_Notify_Worker_Task
{
public:
  /// Constructor
  TAO_Notify_Reactive_Task (void);

  /// Destructor
  ~TAO_Notify_Reactive_Task (void) override;

  /// Init the reactive task.
  void init (void);

  /// Shutdown task
  void shutdown (void) override;

  /// Exec the request.
  void execute (TAO_Notify_Method_Request& method_request) override;

  /// The object used by clients to register timers. This method returns a Reactor based Timer.
  TAO_Notify_Timer* timer (void) override;

  /// Returns NULL.
  virtual TAO_Notify_Buffering_Strategy* buffering_strategy (void);

private:
  /// The timer.
  TAO_Notify_Timer_Reactor::Ptr timer_;

  /// Release
  void release (void) override;
};


TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/Notify/Reactive_Task.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_Notify_REACTIVE_TASK_H */
