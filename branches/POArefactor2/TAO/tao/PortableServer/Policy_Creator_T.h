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
        const CORBA::Any &value ACE_ENV_ARG_DECL)
          ACE_THROW_SPEC ((CORBA::PolicyError))
      {
        FACTORY *policy_factory =
           ACE_Dynamic_Service<FACTORY>::instance (factory_string);

        if (policy_factory == 0)
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) \n"),
                      ACE_TEXT ("Unable to get %s"),
                      factory_string));
        else
          return policy_factory->create (value ACE_ENV_ARG_PARAMETER);
      }

      static
      POLICYTYPE create (
        const char *factory_string,
        POLICYVALUETYPE value ACE_ENV_ARG_DECL)
      {
        FACTORY *policy_factory =
           ACE_Dynamic_Service<FACTORY>::instance (factory_string);

        if (policy_factory == 0)
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) \n"),
                      ACE_TEXT ("Unable to get %s"),
                      factory_string));
        else
          return policy_factory->create (value ACE_ENV_ARG_PARAMETER);
      }
    };
  }
}

#include /**/ "ace/post.h"

#endif /* TAO_PORTABLESERVER_POLICY_CREATOR_T_H */
