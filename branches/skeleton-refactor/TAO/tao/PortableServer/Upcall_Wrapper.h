// -*- C++ -*-

//=============================================================================
/**
 *  @file    Upcall_Wrapper.h
 *
 *  $Id$
 *
 *  @author Ossama Othman
 *  @author Jeff Parsons
 *  @author Carlos O'Ryan
 */
//=============================================================================

#ifndef TAO_UPCALL_WRAPPER_H
#define TAO_UPCALL_WRAPPER_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#ifndef ACE_LACKS_PRAGMA_ONCE
# pragma once
#endif /* !ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#if TAO_HAS_INTERCEPTORS == 1
#include "tao/PortableServer/ServerRequestInfo.h"
#include "tao/PortableServer/ServerInterceptorAdapter.h"
#endif  /* TAO_HAS_INTERCEPTORS == 1 */


class TAO_ServantBase;
class TAO_ServerRequest;


namespace PortableServer
{
  typedef ::TAO_ServantBase ServantBase;
}


namespace TAO
{
  class Argument;

  /**
   * @class Upcall_Wrapper
   *
   * @brief Wraps the activities of the _skel operations.
   *
   */
  class TAO_PortableServer_Export Upcall_Wrapper
  {
  public:

    /// Constructor.
    Upcall_Wrapper (TAO::Argument * args[],
                    size_t nargs,
                    TAO_ServerRequest & server_request,
                    void * servant_upcall,
                    PortableServer::ServantBase * servant,
                    CORBA::TypeCode_ptr * exceptions,
                    size_t nexceptions);

    /// Perform pre-upcall operations.
    /**
     * Perform pre-upcall operations.  Operations include operation
     * @c IN and @c INOUT argument demarshaling, and intermediate
     * interception point invocation.
     */
    bool pre_upcall (void);

    /// Perform post-upcall operations.
    /**
     * Perform post-upcall operations.  Operations include operation
     * @c INOUT and @c OUT argument marshaling, and ending
     * interception point invocation.
     */
    bool post_upcall (void);

  private:

    /// Operation argument list.
    /**
     * @note The TAO::Argument corresponding to the return value is
     *       always the first element in the array, regardless of
     *       whether or not the return type is void.
     */
    TAO::Argument ** const args_;

    /// Number of arguments in the operation argument list.
    size_t const nargs_;

    /// Object containing server side messaging operations (e.g. CDR
    /// reply construction, etc).
    TAO_ServerRequest & server_request_;

#if TAO_HAS_INTERCEPTORS == 1

    /// PortableServer::ServerRequestInfo instance specific to the
    /// upcall.
    TAO::ServerRequestInfo request_info_;

    /// Interceptor adapter object that encapsulates the server side
    /// interception points.
    TAO::ServerRequestInterceptor_Adapter interceptor_adapter_;

#endif  /* TAO_HAS_INTECEPTORS == 1 */

  };

}

#ifdef __ACE_INLINE__
# include "tao/PortableServer/Upcall_Wrapper.inl"
#endif  /* __ACE_INLINE_*/

#include /**/ "ace/post.h"

#endif /* TAO_UPCALL_WRAPPER_H */
