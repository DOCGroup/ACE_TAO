// This may look like C, but it's really -*- C++ -*-
// $Id$: 

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

  void send_request (PortableInterceptor::ClientRequestInfo_ptr ri,
                     CORBA::Environment &);

  void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri,
                      CORBA::Environment &);
  void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri,
                          CORBA::Environment &);

private:

  TAO_ClientRequestInterceptor_List::TYPE &interceptors_;
  ///< Reference to the list of registered interceptors.

  size_t len_;
  ///< Cache the length of the interceptor list so that we don't have
  ///< to compute it at each stage of the current interception.

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

  void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri,
                        CORBA::Environment &);

  void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri,
                   CORBA::Environment &);

  void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri,
                       CORBA::Environment &);

private:

  TAO_ServerRequestInterceptor_List::TYPE &interceptors_;
  ///< Reference to the list of registered interceptors.

  size_t len_;
  ///< Cache the length of the interceptor list so that we don't have
  ///< to compute it at each stage of the current interception.

};

#if defined (__ACE_INLINE__)
#include "tao/PortableInterceptor.i"
#endif /* defined INLINE */

#endif /* TAO_HAS_INTERCEPTORS */

#include "ace/post.h"

#endif /* TAO_PORTABLE_INTERCEPTOR_H */
