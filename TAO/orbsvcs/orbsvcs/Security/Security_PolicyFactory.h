// -*- C++ -*-

//=============================================================================
/**
 * @file Security_PolicyFactory.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_SECURITY_POLICY_FACTORY_H
#define TAO_SECURITY_POLICY_FACTORY_H

#include /**/ "ace/pre.h"

#include "security_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/// Policy factory for all security related policies.
class TAO_Security_Export TAO_Security_PolicyFactory
  : public PortableInterceptor::PolicyFactory,
    public CORBA::LocalObject
{
public:

  virtual CORBA::Policy_ptr create_policy (CORBA::PolicyType type,
                                           const CORBA::Any & value
                                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CORBA::PolicyError));
};


#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_SECURITY_POLICY_FACTORY_H */
