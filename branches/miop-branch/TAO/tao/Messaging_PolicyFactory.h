// -*- C++ -*-

//=============================================================================
/**
 * @file Messaging_PolicyFactory.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_MESSAGING_POLICY_FACTORY_H
#define TAO_MESSAGING_POLICY_FACTORY_H

#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if TAO_HAS_CORBA_MESSAGING == 1

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

/// Policy factory for all Messaging related policies.
class TAO_Export TAO_Messaging_PolicyFactory
  : public virtual PortableInterceptor::PolicyFactory,
    public virtual TAO_Local_RefCounted_Object
{
public:

  virtual CORBA::Policy_ptr create_policy (CORBA::PolicyType type,
                                           const CORBA::Any & value
                                           TAO_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CORBA::PolicyError));
};


#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

#include "ace/post.h"

#endif /* TAO_MESSAGING_POLICY_FACTORY_H */
