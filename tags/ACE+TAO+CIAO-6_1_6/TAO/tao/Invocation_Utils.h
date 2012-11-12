// -*- C++ -*-

//=============================================================================
/**
 *  @file    Invocation_Utils.h
 *
 *  $Id$
 *
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_INVOCATION_UTILS_H
#define TAO_INVOCATION_UTILS_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  enum Invocation_Status
    {
      /// Initial state of the FSM in the invocation class.
      TAO_INVOKE_START = 0,
      /// The request must be restarted, a temporary failure has
      /// occured.
      TAO_INVOKE_RESTART,
      /// invoke() call successful. Final state of the FSM.
      TAO_INVOKE_SUCCESS,
      /// Received a user exception from the remote object, and this
      /// is one of the final states.
      TAO_INVOKE_USER_EXCEPTION,
      /// Received a system exception from the remote object or the
      /// PI and it is one of the final states
      TAO_INVOKE_SYSTEM_EXCEPTION,
      /// Invocation failed. It is a final state.
      TAO_INVOKE_FAILURE
    };

  /// Enums for invocation types
  enum Invocation_Type {
    /// A simple oneway invocation
    TAO_ONEWAY_INVOCATION,
    /// A simple twoway invocation
    TAO_TWOWAY_INVOCATION
  };

  /// Enum for invocation modes
  enum Invocation_Mode {
    /// Standard synchronous twoway
    TAO_SYNCHRONOUS_INVOCATION,
    /// Asynchronous twoway with callback model for collecting the
    /// replies.
    TAO_ASYNCHRONOUS_CALLBACK_INVOCATION,
    /// Asynchronous twoway with poller model for collecting the
    /// replies.
    /// @note This is not supported in TAO now
    TAO_ASYNCHRONOUS_POLLER_INVOCATION,
    /// Types of DII
    TAO_DII_INVOCATION,
    TAO_DII_DEFERRED_INVOCATION,
    TAO_DII_ASYNCH_INVOCATION
  };

  enum Forward_Once_Exception
    {
      FOE_NON              = 0x0,
      FOE_OBJECT_NOT_EXIST = 0x1,
      FOE_COMM_FAILURE     = 0x2,
      FOE_TRANSIENT        = 0x4,
      FOE_INV_OBJREF       = 0x8,
      FOE_ALL              = 0xFFFFFFFF
    };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_INVOCATION_UTILS_H */
