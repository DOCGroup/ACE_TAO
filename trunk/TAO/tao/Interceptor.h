// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Interceptor.h
//
// = DESCRIPTION
//     This file contains two helper classes to simplify the support of
//     interceptors in tao_idl generated code.  All the helper classes
//     implementations are in Interceptor.i file.
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
//
// ============================================================================

#ifndef TAO_INTERCEPTOR_H
#define TAO_INTERCEPTOR_H
#include "ace/pre.h"

#include "tao/InterceptorS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_INTERCEPTORS)
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

#if defined (TAO_HAS_INTERCEPTORS)
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

  void preinvoke (CORBA::ULong,
                  CORBA::Boolean,
                  CORBA::Object_ptr,
                  const char *,
                  IOP::ServiceContextList &,
                  CORBA::NVList_ptr &,
                  PortableInterceptor::Cookies &,
                  CORBA::Environment &);

  void postinvoke (CORBA::ULong,
                   CORBA::Boolean,
                   CORBA::Object_ptr,
                   const char *,
                   IOP::ServiceContextList &,
                   CORBA::NVList_ptr &,
                   PortableInterceptor::Cookies &,
                   CORBA::Environment &);

  void exception_occurred (CORBA::ULong,
                           CORBA::Boolean,
                           CORBA::Object_ptr,
                           const char *,
                           //IOP::ServiceContextList &,
                           //CORBA::Exception *&,
                           PortableInterceptor::Cookies &,
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

  void preinvoke (CORBA::ULong,
                  CORBA::Boolean,
                  CORBA::Object_ptr,
                  const char *,
                  IOP::ServiceContextList &,
                  CORBA::NVList_ptr &,
                  PortableInterceptor::Cookies &,
                  CORBA::Environment &);

  void postinvoke (CORBA::ULong,
                   CORBA::Boolean,
                   CORBA::Object_ptr,
                   const char *,
                   IOP::ServiceContextList &,
                   CORBA::NVList_ptr &,
                   PortableInterceptor::Cookies &,
                   CORBA::Environment &);

  void exception_occurred (CORBA::ULong,
                           CORBA::Boolean,
                           CORBA::Object_ptr,
                           const char *,
                           //IOP::ServiceContextList &,
                           //CORBA::Exception *&,
                           PortableInterceptor::Cookies &,
                           CORBA::Environment &);

private:
  PortableInterceptor::ServerRequestInterceptor_var interceptor_;
};
#endif /* TAO_HAS_INTERCEPTORS */

#if defined (__ACE_INLINE__)
#include "tao/Interceptor.i"
#endif /* defined INLINE */

#include "ace/post.h"
#endif /* TAO_INTERCEPTOR_H */
