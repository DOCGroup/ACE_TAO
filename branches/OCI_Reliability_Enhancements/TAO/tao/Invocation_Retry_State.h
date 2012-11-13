/* -*- C++ -*- $Id$ */
//=============================================================================
/**
 *  @file   Invocation_Retry_State.h
 *
 *  $Id$
 *
 * Tracks the state of invocation retries after encountering exceptions.
 *
 */
//=============================================================================

#ifndef TAO_INVOCATION_RETRY_STATE_H
#define TAO_INVOCATION_RETRY_STATE_H

#include "tao/Stub.h"
#include "tao/Invocation_Retry_Params.h"

#include "ace/Array_Map.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

  class Invocation_Retry_State
  {
  public:
    Invocation_Retry_State(TAO_Stub &stub);

    bool forward_on_exception_increment (const int ef);

    bool forward_on_exception_limit_used () const;

    void sleep_before_retry ();

  private:

    typedef ACE_Array_Map<int, int> Ex_Count_Map;
    Ex_Count_Map ex_count_map_;
    TAO_Stub &stub_;
    Invocation_Retry_Params retry_params_;
    bool forward_on_exception_limit_used_;
  };

}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_INVOCATION_RETRY_STATE_H*/
