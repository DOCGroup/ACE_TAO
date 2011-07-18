// -*- C++ -*-

//=============================================================================
/**
 *  @file Policy_Creator_T.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_POLICY_CREATOR_T_H
#define TAO_PORTABLESERVER_POLICY_CREATOR_T_H

#include /**/ "ace/pre.h"

#include "tao/ORB_Constants.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    template <class POLICYTYPE, typename POLICYVALUE>
    void create_policy (
        POLICYTYPE *& policy,
        POLICYVALUE &value,
        const CORBA::Any& val
        )
    {
      if ((val >>= value) == 0)
        throw CORBA::PolicyError (CORBA::BAD_POLICY_VALUE);

      ACE_NEW_THROW_EX (policy,
                        POLICYTYPE (value),
                        CORBA::NO_MEMORY (TAO::VMCID,
                                          CORBA::COMPLETED_NO));
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_PORTABLESERVER_POLICY_CREATOR_T_H */
