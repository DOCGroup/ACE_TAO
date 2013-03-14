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
    Invocation_Retry_State (TAO_Stub &stub);

    ~Invocation_Retry_State ();

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
    bool forward_on_reply_closed_increment ();

    /**
     * Increment to next profile in preparation
     * to retry using that profile.
     * If the next profile is the starting
     * base profile then also call
     * sleep ().
     * @see TAO_Stub::next_profile_retry()
     */
    void next_profile_retry (TAO_Stub &stub) const;

    /**
     * Sleep if profile is the starting
     * base profile.
     */
    void sleep_at_starting_profile (TAO_Stub &stub) const;

    /**
     * Sleep according to the delay value
     * in Invocation_Retry_Params.
     */
    void sleep () const;

  private:

    typedef ACE_Array_Map<int, int> Ex_Count_Map;
    Ex_Count_Map ex_count_map_;
    int forward_on_reply_closed_count_;
    Invocation_Retry_Params retry_params_;
    bool forward_on_exception_limit_used_;
  };

}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_INVOCATION_RETRY_STATE_H*/
