// This may look like C, but it's really -*- C++ -*-

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

namespace TAO
{
  namespace Portable_Server
  {
    template <class POLICYTYPE, class POLICYVALUE>
    void create_policy (
        POLICYTYPE *& policy,
        POLICYVALUE &value,
        const CORBA::Any& val
        ACE_ENV_ARG_DECL)
      {
        if ((val >>= value) == 0)
          ACE_THROW (CORBA::PolicyError (CORBA::BAD_POLICY_VALUE));

        ACE_NEW_THROW_EX (policy,
                          POLICYTYPE (value),
                          CORBA::NO_MEMORY (TAO::VMCID,
                                            CORBA::COMPLETED_NO));
        ACE_CHECK;
      }
  }
}

#include /**/ "ace/post.h"

#endif /* TAO_PORTABLESERVER_POLICY_CREATOR_T_H */
