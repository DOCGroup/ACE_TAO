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

#include "TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Array_Base.h"
#include "ORBInitInfoC.h"

namespace PortableInterceptor
{
  class Interceptor;
  typedef Interceptor *Interceptor_ptr;
}

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

  /// Constructor.
  TAO_Interceptor_List (void);

  /// Destructor.
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

  /// Return the interceptor in sequence element @a index.
  virtual PortableInterceptor::Interceptor_ptr interceptor (
      size_t index
    ) = 0;

};

#include /**/ "ace/post.h"

#endif /* TAO_INTERCEPTOR_LIST_H */
