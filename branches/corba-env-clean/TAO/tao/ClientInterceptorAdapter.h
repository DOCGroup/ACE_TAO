// -*- C++ -*-

//=============================================================================
/**
 *  @file    ClientInterceptorAdapter.h
 *
 *  $Id$
 *
 *   This file an adapter class to simplify the support of
 *   interceptors in tao_idl generated stubs.
 *
 *  @author  Nanbor Wang <nanbor@cs.wustl.edu>
 *  @author  Ossama Othman <ossama@uci.edu>
 *  @author  Kirthika Parameswaran  <kirthika@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_CLIENT_INTERCEPTOR_ADAPTER_H
#define TAO_CLIENT_INTERCEPTOR_ADAPTER_H

#include "ace/pre.h"

#include "corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#if TAO_HAS_INTERCEPTORS == 1

#include "ClientRequestInfo.h"
#include "Interceptor_List.h"

// Forward declarations
class TAO_ClientRequestInfo;

/**
 * @class TAO_ClientRequestInterceptor_Adapter
 *
 * @brief TAO_ClientRequestInterceptor_Adapter
 *
 * A convenient helper class to invoke registered client request
 * interceptor(s) in tao_idl generated code.
 */
class TAO_Export TAO_ClientRequestInterceptor_Adapter
{

public:

  TAO_ClientRequestInterceptor_Adapter (
    TAO_ClientRequestInterceptor_List::TYPE &interceptors,
    TAO_GIOP_Invocation *invocation,
    int &_invoke_status);

  ~TAO_ClientRequestInterceptor_Adapter (void);

  /**
   * @name PortableInterceptor Client Side Interception Points
   *
   * Each of these methods corresponds to a client side interception
   * point.  There are no "intermediate" interception points on the
   * client side, only "starting" and "ending" interception points.
   *
   * @todo
   * The "send_poll()" and "receive_other()" (for AMI)
   * PortableInterceptor interception points are missing.
   */
  //@{
  /// This method implements one of the "starting" client side
  /// interception points.
  void send_request (TAO_ClientRequestInfo *ri
                     TAO_ENV_ARG_DECL_NOT_USED);

  /// This method implements one of the "ending" client side
  /// interception point.
  void receive_reply (TAO_ClientRequestInfo *ri
                      TAO_ENV_ARG_DECL_NOT_USED);

  /// This method implements one of the "ending" client side
  /// interception point.
  void receive_exception (TAO_ClientRequestInfo *ri
                          TAO_ENV_ARG_DECL_NOT_USED);

  /// This method implements one of the "ending" client side
  /// interception point.
  void receive_other (TAO_ClientRequestInfo *ri
                      TAO_ENV_ARG_DECL_NOT_USED);
  //@}

protected:

  /// Process the given PortableInterceptor::ForwardRequest exception,
  /// i.e. invoke the receive_other() interception point, in addition
  /// to notifying the Invocation object of the LOCATION_FORWARD.
  void process_forward_request (TAO_ClientRequestInfo *ri,
                                PortableInterceptor::ForwardRequest &exc
                                TAO_ENV_ARG_DECL);

private:

  /// Reference to the list of registered interceptors.
  TAO_ClientRequestInterceptor_List::TYPE &interceptors_;

  /// Pointer to the GIOP invocation object for the current request.
  TAO_GIOP_Invocation *invocation_;

  /// Reference to the invocation status obtained from and returned to
  /// the stub.  Needed to support the
  /// PortableInterceptor::ForwardRequest exception.
  int &invoke_status_;

  /// Cache the length of the interceptor list so that we don't have
  /// to compute it at each stage of the current interception.
  size_t len_;

  /// The number of interceptors "pushed" onto the logical flow
  /// stack.  This is used when unwinding the flow stack.
  size_t stack_size_;

};

#if defined (__ACE_INLINE__)
#include "ClientInterceptorAdapter.inl"
#endif  /* __ACE_INLINE__ */

#endif /* TAO_HAS_INTERCEPTORS */

#include "ace/post.h"

#endif /* TAO_CLIENT_INTERCEPTOR_ADAPTER_H */
