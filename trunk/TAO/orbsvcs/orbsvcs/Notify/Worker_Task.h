/* -*- C++ -*- */
/**
 *  @file Worker_Task.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_WORKER_TASK_H
#define TAO_Notify_WORKER_TASK_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Method_Request.h"
#include "Refcountable.h"

class TAO_Notify_AdminProperties;
class TAO_Notify_QoSProperties;
class TAO_Notify_Timer;
class TAO_Notify_Buffering_Strategy;

/**
 * @class TAO_Notify_Worker_Task
 *
 * @brief Base Worker Task.
 *
 * Memory Management : The Worker Task should be allocated on the heap and
 * the <shutdown> method should be called to release memory.
 *
 */
class TAO_Notify_Export TAO_Notify_Worker_Task : public TAO_Notify_Refcountable
{
public:
  /// Constuctor
  TAO_Notify_Worker_Task (void);

  /// Update QoS Properties.
  virtual void update_qos_properties (const TAO_Notify_QoSProperties& qos_properties);

  ///= Public method to be implemented by subclasses.
  /// Exec the request.
  virtual void execute (TAO_Notify_Method_Request_No_Copy& method_request ACE_ENV_ARG_DECL) = 0;

  /// Shutdown task
  virtual void shutdown (void) = 0;

  /// The object used by clients to register timers.
  virtual TAO_Notify_Timer* timer (void) = 0;

  virtual TAO_Notify_Buffering_Strategy* buffering_strategy (void) = 0;

protected:
  /// Destructor
  virtual ~TAO_Notify_Worker_Task ();
};

#if defined (__ACE_INLINE__)
#include "Worker_Task.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_Notify_WORKER_TASK_H */
