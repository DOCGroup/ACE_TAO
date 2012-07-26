// -*- C++ -*-

//=============================================================================
/**
 *  @file     RT_PolicyFactory.h
 *
 *  $Id$
 *
 *  @author  Angelo Corsaro <corsaro@cs.wustl.edu>
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_RT_POLICY_FACTORY_H
#define TAO_RT_POLICY_FACTORY_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/rtcorba_export.h"

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

/// Policy factory for all RTCORBA related policies.
class TAO_RT_PolicyFactory
  : public virtual PortableInterceptor::PolicyFactory,
    public virtual ::CORBA::LocalObject
{
public:
  virtual CORBA::Policy_ptr create_policy (CORBA::PolicyType type,
                                           const CORBA::Any &value);

  virtual CORBA::Policy_ptr _create_policy (CORBA::PolicyType type);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */

#include /**/ "ace/post.h"

#endif /* TAO_RT_POLICY_FACTORY_H */
