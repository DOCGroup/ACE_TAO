// -*- C++ -*-

//=============================================================================
/**
 *  @file    ServerInterceptorAdapter.h
 *
 *  $Id$
 *
 *   This file contains a helper class to simplify the support of
 *   interceptors in tao_idl generated skeletons.
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

#include "tao/PortableInterceptorC.h"
#include "tao/Interceptor_List.h"

class TAO_ServerRequestInfo;

/**
 * @class TAO_ServerRequestInterceptor_Adapter
 *
 * @brief TAO_ServerRequestInterceptor_Adapter
 *
 * A convenient helper class to invoke registered server request
 * interceptor(s) in tao_idl generated code.
 */
class TAO_PortableServer_Export TAO_ServerRequestInterceptor_Adapter
{

public:

  TAO_ServerRequestInterceptor_Adapter (
    TAO_ServerRequestInterceptor_List::TYPE &interceptors,
    size_t &stack_size);

  ~TAO_ServerRequestInterceptor_Adapter (void);

  /**
   * @name PortableInterceptor Client Side Interception Points
   *
   * Each of these methods corresponds to a client side interception
   * point.
   *
   * @todo
   * The "receive_request_service_contexts()" and "send_other()"
   * PortableInterceptor interception points are missing.
   */
  //@{
  /// This method implements the "starting" server side interception
  /// point.
  void receive_request_service_contexts (
    TAO_ServerRequestInfo * ri
    ACE_ENV_ARG_DECL);

  /// This method an "intermediate" server side interception point.
  void receive_request (TAO_ServerRequestInfo * ri
                        ACE_ENV_ARG_DECL);

  /// This method implements one of the "ending" server side
  /// interception points.
  void send_reply (TAO_ServerRequestInfo * ri
                   ACE_ENV_ARG_DECL);

  /// This method implements one of the "ending" server side
  /// interception points.
  void send_exception (TAO_ServerRequestInfo * ri
                       ACE_ENV_ARG_DECL);

  /// This method implements one of the "ending" server side
  /// interception points.
  void send_other (TAO_ServerRequestInfo * ri
                   ACE_ENV_ARG_DECL);
  //@}

  /// Returns true if a LOCATION_FORWARD was generated, and false
  /// otherwise.
  CORBA::Boolean location_forwarded (void) const;

private:

  /// Reference to the list of registered interceptors.
  TAO_ServerRequestInterceptor_List::TYPE & interceptors_;

  /// Cache the length of the interceptor list so that we don't have
  /// to compute it at each stage of the current interception.
  const size_t len_;

  /// The number of interceptors "pushed" onto the logical flow
  /// stack.  This is used when unwinding the flow stack.
  size_t & stack_size_;

  /// True if a PortableInterceptor::ForwardRequest exception was
  /// thrown.
  CORBA::Boolean location_forwarded_;

};

#if defined (__ACE_INLINE__)
# include "ServerInterceptorAdapter.inl"
#endif  /* __ACE_INLINE__ */

#endif  /* TAO_HAS_INTERCEPTORS */

#include /**/ "ace/post.h"

#endif /* TAO_SERVER_INTERCEPTOR_ADAPTER_H */
