/* -*- C++ -*- $Id$ */
//=============================================================================
/**
 *  @file   Invocation_Retry_State.h
 *
 *  $Id$
 *
 *  @author Byron Harris (harrisb@ociweb.com)
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

  /**
   * @class Invocation_Retry_State
   *
   * @brief Maintains state of invocation retries.
   */
  class Invocation_Retry_State
  {
  public:
    Invocation_Retry_State(TAO_Stub &stub);

    /**
     * Answer if any profile forward on exception limit
     * parameter is used.
     */
    bool forward_on_exception_limit_used () const;

    /**
     * Attempt to increment the count of profile
     * forwards.
     * @return false if forward on exception is not
     * being used or the limit has been reached.
     */
    bool forward_on_exception_increment (const int ef);

    /**
     * Attempt to increment the count of retries
     * when a server connection is seen as closed
     * during reply.
     */
    bool retry_on_reply_closed_increment ();

    /**
     * Sleep before retry if at starting
     * base profile.
     */
    void sleep_before_retry ();

  private:

    typedef ACE_Array_Map<int, int> Ex_Count_Map;
    Ex_Count_Map ex_count_map_;
    int retry_on_reply_closed_count_;
    TAO_Stub &stub_;
    Invocation_Retry_Params retry_params_;
    bool forward_on_exception_limit_used_;
  };

}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_INVOCATION_RETRY_STATE_H*/
