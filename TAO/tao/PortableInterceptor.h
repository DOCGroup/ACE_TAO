// This may look like C, but it's really -*- C++ -*-
// $Id$

// ========================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    PortableInterceptor.h
//
// = DESCRIPTION
//     This file contains two helper classes to simplify the support of
//     interceptors in tao_idl generated code.  All the helper classes
//     implementations are in PortableInterceptor.i file.
//
//     A series of TAO_INTERCEPTOR* macros are also defined in the file.
//     These macros allows us to disable/enable support of interceptors
//     easily.
//
//
// = AUTHOR
//     Nanbor Wang <nanbor@cs.wustl.edu>
//     Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//     Ossama Othman <ossama@uci.edu>
//
// ========================================================================

#ifndef TAO_PORTABLE_INTERCEPTOR_H
#define TAO_PORTABLE_INTERCEPTOR_H

#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_INTERCEPTORS == 1)
# define TAO_INTERCEPTOR(X) X
# define TAO_INTERCEPTOR_CHECK ACE_TRY_CHECK
# define TAO_INTERCEPTOR_CHECK_RETURN(X) ACE_TRY_CHECK
# define TAO_INTERCEPTOR_THROW(X) ACE_TRY_THROW(X)
# define TAO_INTERCEPTOR_THROW_RETURN(X,Y) ACE_TRY_THROW(X)
#else
# define TAO_INTERCEPTOR(X)
# define TAO_INTERCEPTOR_CHECK ACE_CHECK
# define TAO_INTERCEPTOR_CHECK_RETURN(X) ACE_CHECK_RETURN (X)
# define TAO_INTERCEPTOR_THROW(X) ACE_THROW(X)
# define TAO_INTERCEPTOR_THROW_RETURN(X,Y) ACE_THROW_RETURN(X,Y)
#endif /* TAO_HAS_INTERCEPTORS */

#if (TAO_HAS_INTERCEPTORS == 1)

#include "tao/PortableInterceptorC.h"
#include "tao/Interceptor_List.h"

class TAO_Export TAO_ClientRequestInterceptor_Adapter
{
  // = TITLE
  //   TAO_ClientRequestInterceptor_Adapter
  //
  // = DESCRIPTION
  //   A convenient helper class to invoke registered client request
  //   interceptor(s) in tao_idl generated code.

public:

  TAO_ClientRequestInterceptor_Adapter (
    TAO_ClientRequestInterceptor_List::TYPE &interceptors);

  ~TAO_ClientRequestInterceptor_Adapter (void);

  /**
   * @name PortableInterceptor Client Side Interception Points
   *
   * Each of these methods corresponds to a client side interception
   * point.  There are no "intermediate" interception points on the
   * client side, only "starting" and "ending" interception points.
   *
   * @todo
   * The "send_poll()" and "receive_other()" PortableInterceptor
   * interception points are missing.
   */
  //@{
  /// This method implements one of the "starting" client side
  /// interception points.
  void send_request (PortableInterceptor::ClientRequestInfo_ptr ri,
                     CORBA::Environment &);

  /// This method implements one of the "ending" client side
  /// interception point.
  void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri,
                      CORBA::Environment &);

  /// This method implements one of the "ending" client side
  /// interception point.
  void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri,
                          CORBA::Environment &);

  /// This method implements one of the "ending" client side
  /// interception point.
  void receive_other (PortableInterceptor::ClientRequestInfo_ptr ri,
                      CORBA::Environment &);
  //@}

private:

  /// Reference to the list of registered interceptors.
  TAO_ClientRequestInterceptor_List::TYPE &interceptors_;

  /// Cache the length of the interceptor list so that we don't have
  /// to compute it at each stage of the current interception.
  size_t len_;

  /// The number of interceptors "pushed" onto the logical flow
  /// stack.  This is used when unwinding the flow stack.
  size_t stack_size_;

  /// Flag that denotes if an ending interception point has been
  /// called.
  int ending_intercept_called_;

};

class TAO_Export TAO_ServerRequestInterceptor_Adapter
{
  // = TITLE
  //   TAO_ServerRequestInterceptor_Adapter
  //
  // = DESCRIPTION
  //   A convenient helper class to invoke registered server request
  //   interceptor(s) in tao_idl generated code.

public:

  TAO_ServerRequestInterceptor_Adapter (
    TAO_ServerRequestInterceptor_List::TYPE &interceptors);

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
  /**
   * This method implements the "starting" server side interception
   * point.
   *
   * @todo
   * The "starting" server side interception point should actually be
   * receive_request_service_contexts().  However, that interception
   * point hasn't been implemented yet.
   */
  void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri,
                        CORBA::Environment &);

  /// This method implements one of the "ending" server side
  /// interception points.
  void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri,
                   CORBA::Environment &);

  /// This method implements one of the "ending" server side
  /// interception points.
  void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri,
                       CORBA::Environment &);
  //@}

private:

  /// Reference to the list of registered interceptors.
  TAO_ServerRequestInterceptor_List::TYPE &interceptors_;

  /// Cache the length of the interceptor list so that we don't have
  /// to compute it at each stage of the current interception.
  size_t len_;

  /// The number of interceptors "pushed" onto the logical flow
  /// stack.  This is used when unwinding the flow stack.
  size_t stack_size_;

  /// Flag that denotes if an ending interception point has been
  /// called.
  int ending_intercept_called_;

};

#if defined (__ACE_INLINE__)
#include "tao/PortableInterceptor.i"
#endif /* defined INLINE */

#endif /* TAO_HAS_INTERCEPTORS */

#include "ace/post.h"

#endif /* TAO_PORTABLE_INTERCEPTOR_H */
