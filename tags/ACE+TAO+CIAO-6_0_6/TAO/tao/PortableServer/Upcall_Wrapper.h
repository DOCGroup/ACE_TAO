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

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* !ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"
#include "tao/orbconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ServantBase;
class TAO_ServerRequest;
class TAO_InputCDR;
class TAO_OutputCDR;

namespace PortableServer
{
  typedef ::TAO_ServantBase ServantBase;
}

namespace TAO
{
  class Argument;
  class Upcall_Command;

  namespace Portable_Server
  {
    class Servant_Upcall;
  }

  /**
   * @class Upcall_Wrapper
   *
   * @brief Wraps the activities of the _skel operations.
   *
   */
  class TAO_PortableServer_Export Upcall_Wrapper
  {
  public:

    /**
     * @note The TAO::Argument corresponding to the return value is
     *       always the first element in the array, regardless of
     *       whether or not the return type is void.
     */

    /// Perform the upcall.
    /**
     * @param server_request Object containing server side messaging
     *                       operations (e.g. CDR reply construction, etc).
     * @param args           Operation argument list.
     * @param nargs          Number of arguments in the operation
     *                       argument list.
     * @param command        @c Command object that performs the
     *                       actual upcall into the servant.
     * @param servant_upcall Object containing information for POA
     *                       that dispatched the servant.
     * @param exceptions     Array of user exceptions the operation
     *                       may raise.
     * @param nexceptions    The number of exceptions in the operation
     *                       user exception array.
     */
    void upcall (TAO_ServerRequest & server_request,
                 TAO::Argument * const args[],
                 size_t nargs,
                 TAO::Upcall_Command & command
#if TAO_HAS_INTERCEPTORS == 1
                 , TAO::Portable_Server::Servant_Upcall *servant_upcall
                 , CORBA::TypeCode_ptr const * exceptions
                 , CORBA::ULong nexceptions
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
                );

  private:


    /// Perform pre-upcall operations.
    /**
     * Perform pre-upcall operations, including operation @c IN and
     * @c INOUT argument demarshaling.
     */
    void pre_upcall (TAO_InputCDR & cdr,
                     TAO::Argument * const * args,
                     size_t nargs);

    /// Perform post-upcall operations.
    /**
     * Perform post-upcall operations, including operation @c INOUT
     * and @c OUT argument marshaling.
     */
    void post_upcall (TAO_ServerRequest& server_request,
                      TAO::Argument * const * args,
                      size_t nargs);
  };

}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_UPCALL_WRAPPER_H */
