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

};

#include "ace/post.h"
#endif /*TAO_INVOCATION_BASE_H*/
