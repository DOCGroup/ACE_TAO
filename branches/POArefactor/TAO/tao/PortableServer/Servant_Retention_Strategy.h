// -*- C++ -*-

//=============================================================================
/**
 *  @file Servant_Retention_Strategy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_ID_ASSIGNMENT_STRATEGY_H
#define TAO_ID_ASSIGNMENT_STRATEGY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "PolicyFactory.h"
#include "PortableServerC.h"
#include "PolicyStrategy.h"
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
    class TAO_PortableServer_Export Servant_Retention_Strategy :
       public virtual Policy_Strategy
    {
    public:
      virtual ~Thread_Strategy (void);

      void init(CORBA::PolicyList *policy_list)
      {
        // dependent on type create the correct strategy.
      }
    };

    class TAO_PortableServer_Export Retain_Servant_Retention_Strategy :
       public virtual Servant_Retention_Strategy
    {
    public:
      virtual ~Retain_Servant_Retention_Strategy (void);

    private:
      TAO_Active_Object_Map *active_object_map_
    };

    class TAO_PortableServer_Export Non_Retain_Servant_Retention_Strategy :
       public virtual Servant_Retention_Strategy
    {
    public:
      virtual ~Non_Retain_Servant_Retention_Strategy (void);
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_ID_ASSIGNMENT_STRATEGY_H */
