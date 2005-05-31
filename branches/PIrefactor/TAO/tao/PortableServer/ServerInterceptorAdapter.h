// -*- C++ -*-

//=============================================================================
/**
 *  @file    ServerInterceptorAdapter.h
 *
 *  $Id$
 *
 *   This file contains a helper class to simplify the support of
 *   interceptors in TAO_IDL generated skeletons.
 *
 *  @author  Nanbor Wang <nanbor@cs.wustl.edu>
 *  @author  Ossama Othman <ossama@uci.edu>
 *  @author  Kirthika Parameswaran  <kirthika@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_SERVER_INTERCEPTOR_ADAPTER_H
#define TAO_SERVER_INTERCEPTOR_ADAPTER_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#if TAO_HAS_INTERCEPTORS == 1

#include "portableserver_export.h"

#include "tao/Basic_Types.h"
#include "tao/Interceptor_List.h"
#include "tao/ServerRequestInterceptorC.h"

namespace TAO
{
  typedef Interceptor_List< ::PortableInterceptor::ServerRequestInterceptor>
    ServerRequestInterceptor_List;
}

class TAO_ServerRequest;

namespace TAO
{
  class ServerRequestInfo;

  /**
   * @class ServerRequestInterceptor_Adapter
   *
   * @brief ServerRequestInterceptor_Adapter
   *
   * A convenient helper class to invoke registered server request
   * interceptor(s).
   */
  class ServerRequestInterceptor_Adapter
  {
  public:

    /// Constructor.
    ServerRequestInterceptor_Adapter (TAO_ServerRequest & server_request);

    /**
     * @name PortableInterceptor Client Side Interception Points
     *
     * Each of these methods corresponds to a client side interception
     * point.
     */
    //@{
#if TAO_HAS_EXTENDED_FT_INTERCEPTORS == 1
    /// This method implements the "starting" server side interception
    /// point. It will be used as the first interception point and it is
    /// proprietary to TAO.
    /// @@ Will go away once Bug 1369 is fixed
    void tao_ft_interception_point (TAO::ServerRequestInfo * ri ,
                                    CORBA::OctetSeq_out oc
                                    ACE_ENV_ARG_DECL);
#endif /*TAO_HAS_EXTENDED_FT_INTERCEPTORS*/

    /// This method implements the "intermediate" server side
    /// interception point if the above #ifdef is set to 1 and a
    /// starting intercetion point if it is not set to 1.
    ///
    /// @note This method should have been the "starting" interception
    ///       point according to the interceptor spec. This will be
    ///       fixed once Bug 1369 is completely done.
    void receive_request_service_contexts (TAO::ServerRequestInfo * ri
                                           ACE_ENV_ARG_DECL);

    /// This method an "intermediate" server side interception point.
    void receive_request (TAO::ServerRequestInfo * ri
                          ACE_ENV_ARG_DECL);

    /// This method implements one of the "ending" server side
    /// interception points.
    void send_reply (TAO::ServerRequestInfo * ri
                     ACE_ENV_ARG_DECL);

    /// This method implements one of the "ending" server side
    /// interception points.
    void send_exception (TAO::ServerRequestInfo * ri
                         ACE_ENV_ARG_DECL);

    /// This method implements one of the "ending" server side
    /// interception points.
    void send_other (TAO::ServerRequestInfo * ri
                     ACE_ENV_ARG_DECL);
    //@}

    /// Returns true if a LOCATION_FORWARD was generated, and false
    /// otherwise.
    bool location_forwarded (void) const;

  private:

    /// Reference to the list of registered interceptors.
    TAO::ServerRequestInterceptor_List::TYPE & interceptors_;

    /// Cache the length of the interceptor list so that we don't have
    /// to compute it at each stage of the current interception.
    size_t const len_;

    /// The number of interceptors "pushed" onto the logical flow
    /// stack.  This is used when unwinding the flow stack.
    size_t & stack_size_;

    /// True if a PortableInterceptor::ForwardRequest exception was
    /// thrown.
    bool location_forwarded_;

  };

}  // End namespace TAO


#if defined (__ACE_INLINE__)
# include "ServerInterceptorAdapter.inl"
#endif  /* __ACE_INLINE__ */

#endif  /* TAO_HAS_INTERCEPTORS */

#include /**/ "ace/post.h"

#endif /* TAO_SERVER_INTERCEPTOR_ADAPTER_H */
