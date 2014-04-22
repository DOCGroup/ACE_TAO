// -*- C++ -*-

//=============================================================================
/**
 *  @file    Collocated_Invocation.h
 *
 *  $Id$
 *
 *
 *  @author Balachandran Natarajan <bala@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_COLLOCATED_INVOCATION_H
#define TAO_COLLOCATED_INVOCATION_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Collocation_Strategy.h"
#include "tao/Invocation_Base.h"
#include "tao/Exception.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Operation_Details;
class TAO_Stub;

namespace TAO
{
  class Profile_Transport_Resolver;
  class Argument;
  class Collocation_Proxy_Broker;

  /**
   * @class Collocated_Invocation
   *
   * @brief Class for Collocated invocations
   *
   * @note Reliable oneways are not handled properly including use of
   * interceptors. This class should technically serve as the base
   * for all invocation types. But now, this is the only class that
   * handles collocated invocations and that too in a brain dead way.
   */
  class TAO_Export Collocated_Invocation : public Invocation_Base
  {
  public:
    /// Constructor used by TAO::Invocation_Adapter
    /**
     * @param t The target on which invocation first started.
     *
     * @param et The effective target in which this invocation is now
     * flowing
     *
     * @param stub Stub for effective target @a et
     *
     * @param detail operation details of the invocation including
     * the service context list
     *
     * @param response_expected flag to indicate whether the
     * operation encapsulated by @a detail returns a response or not.
     */
    Collocated_Invocation (CORBA::Object_ptr t,
                           CORBA::Object_ptr et,
                           TAO_Stub *stub,
                           TAO_Operation_Details &detail,
                           bool response_expected = true);

    /// Invocation on the collocated object mediated through this
    /// class.
    /**
     * Practically this method is a place holder to do the
     * following
     *  - Call the start and end interception points on the client
     *    side.
     *
     *  - Handle location forwards that could be thrown by the
     *    servant.
     *
     */
    Invocation_Status invoke (Collocation_Strategy strat);

  private:

    // Disallow default construction.
    Collocated_Invocation (void);

  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /*TAO_COLLOCATED_INVOCATION_H*/
