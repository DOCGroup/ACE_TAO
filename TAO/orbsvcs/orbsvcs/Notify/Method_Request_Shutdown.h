/* -*- C++ -*- */
/**
 *  @file Method_Request_Shutdown.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_METHOD_REQUEST_SHUTDOWN_H
#define TAO_NS_METHOD_REQUEST_SHUTDOWN_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Method_Request.h"

class TAO_NS_ThreadPool_Task;

/**
 * @class TAO_NS_Method_Request_Shutdown
 *
 * @brief Shutdown message for the ThreadPool_Task
 *
 */
class TAO_Notify_Export TAO_NS_Method_Request_Shutdown : public TAO_NS_Method_Request
{
public:
  /// Constuctor
  TAO_NS_Method_Request_Shutdown (TAO_NS_ThreadPool_Task* task);

  /// Destructor
  ~TAO_NS_Method_Request_Shutdown ();

  /// Create a copy of this object.
  TAO_NS_Method_Request* copy (void);

  /// Execute the Request
  virtual int execute (ACE_ENV_SINGLE_ARG_DECL);

private:
  // Task to shutdown
  TAO_NS_ThreadPool_Task* task_;
};

#if defined (__ACE_INLINE__)
#include "Method_Request_Shutdown.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_METHOD_REQUEST_SHUTDOWN_H */
