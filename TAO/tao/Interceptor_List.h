// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     Interceptor_List.h
//
// = AUTHOR
//     Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef TAO_INTERCEPTOR_LIST_H
#define TAO_INTERCEPTOR_LIST_H

#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_INTERCEPTORS == 1)

#include "tao/PortableInterceptorC.h"
#include "ace/Containers_T.h"


/// Base class for the various portable interceptor lists used
/// internally by TAO. 
class TAO_Export TAO_Interceptor_List
{
public:
  TAO_Interceptor_List (void);
  ///< Constructor

  virtual ~TAO_Interceptor_List (void);
  ///< Destructor.

protected:

  size_t add_interceptor_i (
      PortableInterceptor::Interceptor_ptr interceptor,
      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ORBInitInfo::DuplicateName));
  ///< Register an in interceptor with interceptor list.

  virtual size_t length (void) = 0;
  ///< Return the length of the underlying interceptor sequence.

  virtual void length (size_t) = 0;
  ///< Set the length of the underlying interceptor sequence.

  virtual PortableInterceptor::Interceptor_ptr interceptor (
   size_t index) = 0;
  ///< Return the interceptor in sequence element <index>.

};

class TAO_Export TAO_ClientRequestInterceptor_List
  : public TAO_Interceptor_List
{
public:

  typedef
  ACE_Array_Base<PortableInterceptor::ClientRequestInterceptor_ptr> TYPE;
  ///< Define a trait for the underlying portable interceptor array.

public:

  TAO_ClientRequestInterceptor_List (void);
  ///< Constructor

  ~TAO_ClientRequestInterceptor_List (void);
  ///< ~Destructor

  void add_interceptor (PortableInterceptor::ClientRequestInterceptor_ptr i,
                        CORBA::Environment &ACE_TRY_ENV);
  ///< Register a client request interceptor.

  TYPE &interceptors (void);
  ///< Return reference to the underlying Portable Interceptor array.

protected:

  virtual size_t length (void);
  ///< Return the length of the underlying interceptor array.

  virtual void length (size_t);
  ///< Set the length of the underlying interceptor array.

  virtual PortableInterceptor::Interceptor_ptr interceptor (size_t);
  ///< Return the interceptor in element <index>.

private:

  TYPE interceptors_;
  ///< Dynamic array of registered client request interceptors.

};

/// Override the interceptor list type so the base class
/// add_interceptor() method 
class TAO_Export TAO_ServerRequestInterceptor_List
  : public TAO_Interceptor_List
{
public:

  typedef
  ACE_Array_Base<PortableInterceptor::ServerRequestInterceptor_ptr> TYPE;
  ///< Define a trait for the underlying portable interceptor array.

public:

  TAO_ServerRequestInterceptor_List (void);
  ///< Constructor

  ~TAO_ServerRequestInterceptor_List (void);
  ///< ~Destructor

  void add_interceptor (PortableInterceptor::ServerRequestInterceptor_ptr i,
                        CORBA::Environment &ACE_TRY_ENV);
  ///< Register a server request interceptor.

  TYPE &interceptors (void);
  ///< Return reference to the underlying Portable Interceptor array.

protected:

  virtual size_t length (void);
  ///< Return the length of the underlying interceptor array.

  virtual void length (size_t);
  ///< Set the length of the underlying interceptor array.

  virtual PortableInterceptor::Interceptor_ptr interceptor (size_t);
  ///< Return the interceptor in array element <index>.

private:

  TYPE interceptors_;
  ///< Dynamic array of registered server request interceptors.

};


#if defined (__ACE_INLINE__)
#include "tao/Interceptor_List.inl"
#endif /* defined INLINE */

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include "ace/post.h"

#endif /* TAO_INTERCEPTOR_LIST_H */
