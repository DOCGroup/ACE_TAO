/* -*- C++ -*- */
/**
 *  @file Reactive_Task.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_REACTIVE_TASK_H
#define TAO_NS_REACTIVE_TASK_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Worker_Task.h"
#include "AdminProperties.h"

class TAO_NS_Timer_Reactor;

/**
 * @class TAO_NS_Reactive_Task
 *
 * @brief A reactive worker task. Simply executes the command in the caller's context.
 *
 */
class TAO_Notify_Export TAO_NS_Reactive_Task : public TAO_NS_Worker_Task
{
public:
  /// Constuctor
  TAO_NS_Reactive_Task (void);

  /// Destructor
  ~TAO_NS_Reactive_Task ();

  /// Release
  virtual void release (void);

  /// Init the reactive task.
  void init (TAO_NS_AdminProperties_var& admin_properties ACE_ENV_ARG_DECL);

  /// Shutdown task
  virtual void shutdown (void);

  /// Exec the request.
  virtual void exec (TAO_NS_Method_Request& method_request);

  /// The object used by clients to register timers. This method returns a Reactor based Timer.
  virtual TAO_NS_Timer* timer (void);

  /// Returns NULL.
  virtual TAO_NS_Buffering_Strategy* buffering_strategy (void);

protected:
  /// The timer.
  TAO_NS_Timer_Reactor* timer_;
};

#if defined (__ACE_INLINE__)
#include "Reactive_Task.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_REACTIVE_TASK_H */
