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
# include "tao/PortableServer/Object_Adapter.h"
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

namespace PortableServer
{
  class ServantBase;
}


namespace TAO
{
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
                    TAO::Argument * retval,
                    TAO_ServerRequest & server_request,
                    PortableServer::ServantBase * servant,
                    void * servant_upcall);

    /// Perform pre-upcall operations.
    /**
     * Perform pre-upcall operations.  Operations include operation
     * @c IN and @c INOUT argument demarshaling, and intermediate
     * interception point invocation.
     */
    void pre_upcall (void);

    /// Perform post-upcall operations.
    /**
     * Perform post-upcall operations.  Operations include operation
     * @c INOUT and @c OUT argument marshaling, and ending
     * interception point invocation.
     */
    void post_upcall (void);

  private:

    /// Operation argument list.
    TAO::Argument ** const args_;

    /// Number of arguments in the operation argument list.
    size_t const nargs_;

    /// The return value.
    /**
     * @note Zero if return type is @c void.
     */
    TAO::Argument * const retval_;

    /// Object containing server side messaging operations (e.g. CDR
    /// reply construction, etc).
    TAO_ServerRequest & server_request_;

    /// The servant being invoked in the upcall.
    PortableServer::ServantBase * const servant_;

#if TAO_HAS_INTERCEPTORS == 1

    /// Object containing POA related items specific to the current
    /// upcall.
    /**
     * The actual type of this member is
     * @c TAO_Object_Adapter::Servant_Upcall @c *.  It is statically
     * cast to that type when it is needed.  Since we're using a
     * static_cast\<\> there is no performance penalty.
     *
     * We do this to avoid including "Object_Adapter.h".
     */
    void * const servant_upcall_;

#endif  /* TAO_HAS_INTECEPTORS == 1 */

  };

}

#ifdef __ACE_INLINE__
# include "tao/PortableServer/Upcall_Wrapper.inl"
#endif  /* __ACE_INLINE_*/

#include /**/ "ace/post.h"

#endif /* TAO_UPCALL_WRAPPER_H */
