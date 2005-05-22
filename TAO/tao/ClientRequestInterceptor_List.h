// -*- C++ -*-

// ===================================================================
/**
 *  @file ClientRequestInterceptor_List.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================

#ifndef TAO_CLIENTREQUEST_INTERCEPTOR_LIST_H
#define TAO_CLIENTREQUEST_INTERCEPTOR_LIST_H

#include /**/ "ace/pre.h"

#include "TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbconf.h"

#if (TAO_HAS_INTERCEPTORS == 1)

#include "ace/Array_Base.h"
#include "Interceptor_List.h"
#include "ClientRequestInterceptorC.h"

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
  ACE_Array_Base<PortableInterceptor::ClientRequestInterceptor_var> TYPE;

public:

  /// Constructor
  TAO_ClientRequestInterceptor_List (void);

  /// Destructor.
  virtual ~TAO_ClientRequestInterceptor_List (void);

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

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#if defined (__ACE_INLINE__)
#include "ClientRequestInterceptor_List.inl"
#endif /* defined INLINE */

#include /**/ "ace/post.h"

#endif /* TAO_CLIENTREQUEST_INTERCEPTOR_LIST_H */
