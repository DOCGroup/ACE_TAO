// -*- C++ -*-

// ===================================================================
/**
 *  @file   IORInterceptor_List.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
// ===================================================================

#ifndef TAO_IOR_INTERCEPTOR_LIST_H
#define TAO_IOR_INTERCEPTOR_LIST_H

#include "ace/pre.h"

#include "iorinterceptor_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Interceptor_List.h"

/**
 * @class TAO_IORInterceptor_List
 *
 * @brief Encapsulation for a list of IOR interceptors.
 */
class TAO_IORInterceptor_Export TAO_IORInterceptor_List
  : public TAO_Interceptor_List
{
public:

  /// Define a trait for the underlying portable interceptor array.
  typedef
  ACE_Array_Base<PortableInterceptor::IORInterceptor_ptr> TYPE;

public:

  /// Constructor
  TAO_IORInterceptor_List (void);

  /// Destructor
  ~TAO_IORInterceptor_List (void);

  /// Register an IOR interceptor.
  void add_interceptor (PortableInterceptor::IORInterceptor_ptr
                        ACE_ENV_ARG_DECL);

  /// Return reference to the underlying Portable Interceptor array.
  TYPE & interceptors (void);

protected:

  /// Return the length of the underlying interceptor array.
  virtual size_t length (void);

  /// Set the length of the underlying interceptor array.
  virtual void length (size_t);

  /// Return the interceptor in array element <index>.
  virtual PortableInterceptor::Interceptor_ptr interceptor (size_t);

private:

  /// Dynamic array of registered IOR interceptors.
  TYPE interceptors_;

};


#if defined (__ACE_INLINE__)
#include "IORInterceptor_List.inl"
#endif /* defined INLINE */

#include "ace/post.h"
#endif /*TAO_IOR_INTERCEPTOR_LIST_H*/
