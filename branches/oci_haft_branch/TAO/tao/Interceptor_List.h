// -*- C++ -*-

// ===================================================================
/**
 *  @file   Interceptor_List.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================

#ifndef TAO_INTERCEPTOR_LIST_H
#define TAO_INTERCEPTOR_LIST_H

#include /**/ "ace/pre.h"

#include "corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "PortableInterceptorC.h"
#include "ace/Array_Base.h"


/**
 * @class TAO_Interceptor_List
 *
 * @brief Base class for portable interceptor lists.
 *
 * Base class for the various portable interceptor lists used
 * internally by TAO.
 */
class TAO_Export TAO_Interceptor_List
{
public:

  /// Constructor
  TAO_Interceptor_List (void);

  /// Destructor
  virtual ~TAO_Interceptor_List (void);

protected:

  /// Register an in interceptor with interceptor list.
  size_t add_interceptor_i (
      PortableInterceptor::Interceptor_ptr interceptor
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ORBInitInfo::DuplicateName));

  /// Return the length of the underlying interceptor sequence.
  virtual size_t length (void) = 0;

  /// Set the length of the underlying interceptor sequence.
  virtual void length (size_t) = 0;

  /// Return the interceptor in sequence element <index>.
  virtual PortableInterceptor::Interceptor_ptr interceptor (
      size_t index
    ) = 0;

};

#if (TAO_HAS_INTERCEPTORS == 1)
/**
 * @class TAO_ClientRequestInterceptor_List
 *
 * @brief Encapsulation for a list of client request interceptors.
 */
class TAO_Export TAO_ClientRequestInterceptor_List
  : public TAO_Interceptor_List
{
public:

  /// Define a trait for the underlying portable interceptor array.
  typedef
  ACE_Array_Base<PortableInterceptor::ClientRequestInterceptor_ptr> TYPE;

public:

  /// Constructor
  TAO_ClientRequestInterceptor_List (void);

  /// Destructor
  ~TAO_ClientRequestInterceptor_List (void);

  /// Register a client request interceptor.
  void add_interceptor (PortableInterceptor::ClientRequestInterceptor_ptr i
                        ACE_ENV_ARG_DECL);

  /// Return reference to the underlying Portable Interceptor array.
  TYPE & interceptors (void);

protected:

  /// Return the length of the underlying interceptor array.
  virtual size_t length (void);

  /// Set the length of the underlying interceptor array.
  virtual void length (size_t);

  /// Return the interceptor in element <index>.
  virtual PortableInterceptor::Interceptor_ptr interceptor (size_t);

private:

  /// Dynamic array of registered client request interceptors.
  TYPE interceptors_;

};


/**
 * @class TAO_ServerRequestInterceptor_List
 *
 * @brief Encapsulation for a list of server request interceptors.
 */
class TAO_Export TAO_ServerRequestInterceptor_List
  : public TAO_Interceptor_List
{
public:

  /// Define a trait for the underlying portable interceptor array.
  typedef
  ACE_Array_Base<PortableInterceptor::ServerRequestInterceptor_ptr> TYPE;

public:

  /// Constructor
  TAO_ServerRequestInterceptor_List (void);

  /// Destructor
  ~TAO_ServerRequestInterceptor_List (void);

  /// Register a server request interceptor.
  void add_interceptor (PortableInterceptor::ServerRequestInterceptor_ptr i
                        ACE_ENV_ARG_DECL);

  /// Return reference to the underlying Portable Interceptor array.
  TYPE &interceptors (void);

protected:

  /// Return the length of the underlying interceptor array.
  virtual size_t length (void);

  /// Set the length of the underlying interceptor array.
  virtual void length (size_t);

  /// Return the interceptor in array element <index>.
  virtual PortableInterceptor::Interceptor_ptr interceptor (size_t);

private:

  /// Dynamic array of registered server request interceptors.
  TYPE interceptors_;

};
#endif  /* TAO_HAS_INTERCEPTORS == 1 */


#if defined (__ACE_INLINE__)
#include "Interceptor_List.inl"
#endif /* defined INLINE */

#include /**/ "ace/post.h"

#endif /* TAO_INTERCEPTOR_LIST_H */
