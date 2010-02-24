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

#include /**/ "ace/pre.h"

#include "tao/Messaging/messaging_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/LocalObject.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Policy factory for all Messaging related policies.
class TAO_Messaging_PolicyFactory
  : public virtual PortableInterceptor::PolicyFactory,
    public virtual ::CORBA::LocalObject
{
public:
  virtual CORBA::Policy_ptr create_policy (CORBA::PolicyType type,
                                           const CORBA::Any & value);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_MESSAGING_POLICY_FACTORY_H */
