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

#ifndef TAO_NS_WORKER_TASK_H
#define TAO_NS_WORKER_TASK_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Method_Request.h"

class TAO_NS_AdminProperties;
class TAO_NS_QoSProperties;

/**
 * @class TAO_NS_Worker_Task
 *
 * @brief Base Worker Task.
 *
 */
class TAO_Notify_Export TAO_NS_Worker_Task
{
public:
  /// Constuctor
  TAO_NS_Worker_Task (void);

  /// Destructor
  virtual ~TAO_NS_Worker_Task ();

  /// Init AdminProperties
  virtual void init (TAO_NS_AdminProperties& admin_properties);

  /// Exec the request.
  virtual void exec (TAO_NS_Method_Request& method_request) = 0;

  /// Shutdown task
  virtual void shutdown (void);

  /// Update QoS Properties.
  virtual void update_qos_properties (const TAO_NS_QoSProperties& qos_properties);
};

#if defined (__ACE_INLINE__)
#include "Worker_Task.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_WORKER_TASK_H */
