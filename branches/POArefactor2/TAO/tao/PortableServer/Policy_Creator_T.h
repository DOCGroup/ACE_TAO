// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Create_Policy_T.h
 *
 *  $Id$
 *
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_POLICY_CREATOR_T_H
#define TAO_PORTABLESERVER_POLICY_CREATOR_T_H

#include /**/ "ace/pre.h"

#include "ace/Dynamic_Service.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace TAO
{
  namespace Portable_Server
  {
    template <class FACTORY,
              class POLICYTYPE,
              class POLICYVALUETYPE>
    class Policy_Creator
    {
    public:
      static
      POLICYTYPE create (
        const char *factory_string,
        const ACE_Static_Svc_Descriptor &directive,
        const CORBA::Any &value ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::PolicyError))
      {
        FACTORY *policy_factory =
           ACE_Dynamic_Service<FACTORY>::instance (factory_string);

         // For static libraries try to force load
        if (policy_factory == 0)
          {
            ACE_Service_Config::process_directive (
              directive);

           policy_factory =
             ACE_Dynamic_Service<FACTORY>::instance (factory_string);
          }

        if (policy_factory == 0)
          return 0;
        else
          return policy_factory->create (value ACE_ENV_ARG_PARAMETER);
      }

      static
      POLICYTYPE create (
        const char *factory_string,
        const ACE_Static_Svc_Descriptor &directive,
        POLICYVALUETYPE value)
      {
        FACTORY *policy_factory =
           ACE_Dynamic_Service<FACTORY>::instance (factory_string);

         // For static libraries try to force load
        if (policy_factory == 0)
          {
            ACE_Service_Config::process_directive (
              directive);

           policy_factory =
             ACE_Dynamic_Service<FACTORY>::instance (factory_string);
          }

        if (policy_factory == 0)
          return 0;
        else
          return policy_factory->create (value);
      }
    };
  }
}

#include /**/ "ace/post.h"

#endif /* TAO_PORTABLESERVER_POLICY_CREATOR_T_H */
