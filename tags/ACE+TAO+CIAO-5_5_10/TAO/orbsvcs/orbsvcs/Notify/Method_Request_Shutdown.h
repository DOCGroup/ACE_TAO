// -*- C++ -*-

/**
 *  @file Method_Request_Shutdown.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_METHOD_REQUEST_SHUTDOWN_H
#define TAO_Notify_METHOD_REQUEST_SHUTDOWN_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/Notify/Method_Request.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_ThreadPool_Task;

/**
 * @class TAO_Notify_Method_Request_Shutdown
 *
 * @brief Shutdown message for the ThreadPool_Task
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_Method_Request_Shutdown : public TAO_Notify_Method_Request_Queueable
{
public:
  /// Constuctor
  TAO_Notify_Method_Request_Shutdown (TAO_Notify_ThreadPool_Task* task);

  /// Destructor
  virtual ~TAO_Notify_Method_Request_Shutdown ();

  /// Create a copy of this object.
  TAO_Notify_Method_Request_Queueable* copy (void);

  /// Execute the Request
  virtual int execute (void);

private:
  // Task to shutdown
  TAO_Notify_ThreadPool_Task* task_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_Notify_METHOD_REQUEST_SHUTDOWN_H */
