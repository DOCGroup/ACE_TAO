// -*- C++ -*-

// ===================================================================
/**
 *  @file ServerRequestInterceptor_List.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================

#ifndef TAO_SERVERREQUEST_INTERCEPTOR_LIST_H
#define TAO_SERVERREQUEST_INTERCEPTOR_LIST_H

#include /**/ "ace/pre.h"

#include "TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbconf.h"

#if (TAO_HAS_INTERCEPTORS == 1)

#include "ace/Array_Base.h"
#include "Interceptor_List.h"
#include "ServerRequestInterceptorC.h"

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
  ACE_Array_Base<PortableInterceptor::ServerRequestInterceptor_var> TYPE;

public:

  /// Constructor
  TAO_ServerRequestInterceptor_List (void);

  /// Destructor.
  virtual ~TAO_ServerRequestInterceptor_List (void);

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
#include "ServerRequestInterceptor_List.inl"
#endif /* defined INLINE */

#include /**/ "ace/post.h"

#endif /* TAO_SERVERREQUEST_INTERCEPTOR_LIST_H */
