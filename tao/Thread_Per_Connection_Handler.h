//x -*- C++ -*-

//=============================================================================
/**
 *  @file Thread_Per_Connection_Handler.h
 *
 *  $Id$
 *
 *  Definition of a connection handler for the thread-per-connection
 *  strategy.
 *
 *  @author  Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_THREAD_PER_CONNECTION_HANDLER_H
#define TAO_THREAD_PER_CONNECTION_HANDLER_H

#include /**/ "ace/pre.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/TAO_Export.h"
#include /**/ "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Connection_Handler;
class TAO_ORB_Core;

/**
 * @class TAO_Thread_Per_Connection_Handler
 *
 * @brief Definition for the thread-per-connection strategy.
 *
 * This object acts as an active object, encapsulating the protocol
 * specific handler which the active thread uses to process incoming
 * messages.
 *
 */
class TAO_Export TAO_Thread_Per_Connection_Handler : public ACE_Task_Base
{
public:
  TAO_Thread_Per_Connection_Handler (TAO_Connection_Handler *ch,
                                     TAO_ORB_Core *oc);

  ~TAO_Thread_Per_Connection_Handler (void);

  /// Template hook method that the thread uses...
  /**
   * Please see the documentation in ace/Task.h for details.
   */
  virtual int svc (void);
  virtual int open (void *);
  virtual int close (u_long);

private:
  /// Pointer to protocol specific code that does the bunch of the
  /// job.
  TAO_Connection_Handler *ch_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /*TAO_THREAD_PER_CONNECTION_HANDLER_H*/
