// -*- C++ -*-

//=============================================================================
/**
 *  @file Activation_Strategy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_ACTIVATION_STRATEGY_H
#define TAO_ACTIVATION_STRATEGY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "Policy_Strategy.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_MINIMUM_POA == 0)

namespace CORBA
{
  class PolicyError;
  class PolicyList;
}

namespace TAO
{
  namespace Portable_Server
  {
    class TAO_PortableServer_Export Activation_Strategy :
       public virtual Policy_Strategy
    {
    public:
      virtual ~Activation_Strategy (void) {};

      void strategy_init (TAO_POA *poa, CORBA::PolicyList *policy_list)
      {
        // dependent on type create the correct strategy.
      }

      virtual bool allow_implicit_activation (void) const = 0;
    };

    class TAO_PortableServer_Export Implicit_Activation_Strategy :
       public virtual Activation_Strategy
    {
    public:
      virtual ~Implicit_Activation_Strategy (void) {};

      virtual bool allow_implicit_activation (void) const
      {
        return true;
      }

    private:
    };

    class TAO_PortableServer_Export Explicit_Activation_Strategy :
       public virtual Activation_Strategy
    {
    public:
      virtual ~Explicit_Activation_Strategy (void) {};

      virtual bool allow_implicit_activation (void) const
      {
        return false;
      }
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_ACTIVATION_STRATEGY_H */
