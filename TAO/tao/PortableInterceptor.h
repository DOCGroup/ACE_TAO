// This may look like C, but it's really -*- C++ -*-
// $Id$: 

// ============================================================================
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
//     I also put some default no-op implementation of the interceptor
//     servants.
//
// = AUTHOR
//     Nanbor Wang <nanbor@cs.wustl.edu>
//     Kirthika PArameswaran  <kirthika@cs.wustl.edu>
// ============================================================================

#ifndef TAO_PORTABLE_INTERCEPTOR_H
#define TAO_PORTABLE_INTERCEPTOR_H
#include "ace/pre.h"

#if (TAO_HAS_INTERCEPTORS == 1)
#include "tao/Request_Info.h"
#endif /* TAO_HAS_INTERCEPTORS == 1 */

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
class TAO_Export TAO_ClientRequestInterceptor_Adapter
{
  // = TITLE
  //   TAO_ClientRequestInterceptor_Adapter
  //
  // = DESCRIPTION
  //   A convenient helper class to invoke the client-side request
  //   interceptor(s) in tao_idl generated code.  Currently, TAO only
  //   supports registerring of one interceptor.  The class will only
  //   invoke the interceptor if there's one.
  //
  //   In the future, when we extend TAO's interceptor registering
  //   mechanism, this class will be modified to invoke several
  //   interceptors in series.
public:
  TAO_ClientRequestInterceptor_Adapter
    (PortableInterceptor::ClientRequestInterceptor_ptr interceptor);

  ~TAO_ClientRequestInterceptor_Adapter (void);

  CORBA::Boolean valid (void) const;
  // Check if we have a valid interceptor.

  void send_request (PortableInterceptor::ClientRequestInfo_ptr ri,
                     CORBA::Environment &);

  void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri,
                      CORBA::Environment &);
  void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri,
                          CORBA::Environment &);
private:
  PortableInterceptor::ClientRequestInterceptor_var interceptor_;
};

class TAO_Export TAO_ServerRequestInterceptor_Adapter
{
  // = TITLE
  //   TAO_ServerRequestInterceptor_Adapter
  //
  // = DESCRIPTION
  //   A convenient helper class to invoke the server-side request
  //   interceptor(s) in tao_idl generated code.  Currently, TAO only
  //   supports registerring of one interceptor.  The class will only
  //   invoke the interceptor if there's one.
  //
  //   In the future, when we extend TAO's interceptor registering
  //   mechanism, this class will be modified to invoke several
  //   interceptors in series.
public:
  TAO_ServerRequestInterceptor_Adapter
    (PortableInterceptor::ServerRequestInterceptor_ptr interceptor);

  ~TAO_ServerRequestInterceptor_Adapter (void);

  CORBA::Boolean valid (void) const;
  // Check if we have a valid interceptor.

  void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri,
                        CORBA::Environment &);

  void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri,
                   CORBA::Environment &);

  void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri,
                       CORBA::Environment &);
private:
  PortableInterceptor::ServerRequestInterceptor_var interceptor_;
};

#if defined (__ACE_INLINE__)
#include "tao/PortableInterceptor.i"
#endif /* defined INLINE */
#endif /* TAO_HAS_INTERCEPTORS */

#include "ace/post.h"

#endif /* TAO_PORTABLE_INTERCEPTOR_H */
