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
#include "ace/pre.h"
#include "ace/config-all.h"

namespace TAO
{
  enum Invocation_Status
    {
      TAO_INVOKE_START,
      /// The request must be restarted, a temporary failure has ocurred.
      TAO_INVOKE_RESTART,
      /// invoke() call successful.
      TAO_INVOKE_SUCCESS,
      TAO_INVOKE_USER_EXCEPTION,
      TAO_INVOKE_SYSTEM_EXCEPTION,
      TAO_INVOKE_FAILURE
    };

  /// Enums for invocation types
  enum Invocation_Type {
    TAO_ONEWAY_INVOCATION,
    TAO_TWOWAY_INVOCATION
    // , TAO_LOCATEREQUEST_INVOCATION
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
    /// @NOTE: This is not supported in TAO now
    TAO_ASYNCHRONOUS_POLLER_INVOCATION
  };
};

#include "ace/post.h"
#endif /*TAO_INVOCATION_BASE_H*/
