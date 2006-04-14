// -*- C++ -*-

// ===================================================================
/**
 *  @file   PICurrent_Guard.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
// ===================================================================

#ifndef TAO_PI_CURRENT_GUARD_H
#define TAO_PI_CURRENT_GUARD_H

#include /**/ "ace/pre.h"

#include "pi_server_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#if TAO_HAS_INTERCEPTORS == 1

/// Forward declarations.
class TAO_ServerRequest;

namespace TAO
{
  class PICurrent_Impl;
  class PICurrent_Copy_Callback;

  /**
   * @class PICurrent_Guard
   *
   * @brief Class used to make copying between request scope current
   *        and thread scope current exception-safe.
   *
   * Since copies between the request scope current and thread scope
   * current must also occur if an exception is thrown, e.g. made
   * available to the send_exception() interception points, the
   * "guard" idiom is used to make this action exception-safe.
   *
   * @note This Guard class is only used on the server side.
   */
  class TAO_PI_Server_Export PICurrent_Guard
  {
  public:

    /// Constructor
    /**
     * This constructor sets up this guard to copy the data held in a
     * given PICurrent when transitioning from that PICurrent's scope
     * to another scope (e.g. request scope to thread scope transition
     * immediately following receive_request_service_contexts() on
     * server side).
     *
     * @param tsc_to_rsc true when copying TSC slot table to RSC slot
     *                        table, i.e. after target operation
     *                        completes.
     */
    PICurrent_Guard (TAO_ServerRequest &server_request,
                     bool tsc_to_rsc);

    /// Destructor
    /**
     * The destructor copies (a logical copy whenever possible) data
     * held in a given PICurrent when transitioning from one PICurrent
     * scope to another immediately before any ending interception
     * points are invoked, and after the starting and intermediate (if
     * any) interception points are invoked.
     */
    ~PICurrent_Guard (void);

  private:

    /// The PICurrent implementation whose slot table will be copied.
    PICurrent_Impl *src_;

    /// The PICurrent implementation whose slot table will be filled
    /// with the contents of another PICurrent's slot table.
    PICurrent_Impl *dest_;

    /// Callback object responsible for performing deep copies of a
    /// PICurrent's slot table.
    PICurrent_Copy_Callback *copy_callback_;

    /// Flag that indicates if the TSC is to be copied to the RSC.
    /**
     * If false, then the RSC must be deep copied upon leaving the
     * request scope and entering the thread scope.  This is necessary
     * since the RSC slot table is no longer available upon leaving
     * the thread scope, meaning that a logical copy is not enough.
     */
    const bool tsc_to_rsc_;
  };
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_PI_CURRENT_GUARD_H */
