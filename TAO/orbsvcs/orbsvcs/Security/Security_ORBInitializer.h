// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     Security_ORBInitializer.h
//
// = AUTHOR
//     Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef TAO_SECURITY_ORB_INITIALIZER_H
#define TAO_SECURITY_ORB_INITIALIZER_H

#include "ace/pre.h"

#include "security_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

#include "Security_PolicyFactory.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/// Security ORB initializer.
class TAO_Security_Export TAO_Security_ORBInitializer
  : public virtual PortableInterceptor::ORBInitializer,
    public virtual TAO_Local_RefCounted_Object
{
public:

  virtual void pre_init (PortableInterceptor::ORBInitInfo_ptr info
                         ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void post_init (PortableInterceptor::ORBInitInfo_ptr info
                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:

  /// Register Security policy factories.
  void register_policy_factories (
         PortableInterceptor::ORBInitInfo_ptr info
         ACE_ENV_ARG_DECL);

private:

  /// PolicyFactory that is used to create all security related
  /// policies capable of being created via ORB::create_policy().
  TAO_Security_PolicyFactory policy_factory_;

};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"

#endif /* TAO_SECURITY_ORB_INITIALIZER_H */
