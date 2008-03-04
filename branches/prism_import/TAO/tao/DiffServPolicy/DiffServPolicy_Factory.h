// -*- C++ -*-

//=============================================================================
/**
 *  @file     DiffServPolicy_Factory.h
 *
 *  $Id$
 *
 *  @author  Jaiganesh Balasubramanian <jai@dre.vanderbilt.edu>
 *  @author  Johnny Willemsen <jwillemsen@remedy.nl>
 */
//=============================================================================


#ifndef TAO_DIFFSERV_POLICY_FACTORY_H
#define TAO_DIFFSERV_POLICY_FACTORY_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"
#include "tao/DiffServPolicy/DiffServPolicy_Export.h"

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

/// Policy factory for all DiffServ related policies.
class TAO_DiffServPolicy_Export TAO_DiffServ_PolicyFactory
  : public virtual PortableInterceptor::PolicyFactory,
    public virtual TAO_Local_RefCounted_Object
{
public:

  CORBA::Policy_ptr create_policy (CORBA::PolicyType type,
                                   const CORBA::Any &value);

  CORBA::Policy_ptr _create_policy (CORBA::PolicyType type);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_DIFFSERV_POLICY_FACTORY_H */
