// -*- C++ -*-

//=============================================================================
/**
 *  @file Request_Processing_Strategy.h
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
    class TAO_PortableServer_Export Request_Processing_Strategy :
       public virtual Policy_Strategy
    {
    public:
      virtual ~Thread_Strategy (void);

      void init(CORBA::PolicyList *policy_list)
      {
        // dependent on type create the correct strategy.
      }
    };

    class TAO_PortableServer_Export AOM_Only_Request_Processing_Strategy :
       public virtual Request_Processing_Strategy
    {
    public:
      virtual ~AOM_Only_Request_Processing_Strategy (void);

    private:
    };

    class TAO_PortableServer_Export Default_Servant_Request_Processing_Strategy :
       public virtual Request_Processing_Strategy
    {
    public:
      virtual ~Default_Servant_Request_Processing_Strategy (void);
    };

    class TAO_PortableServer_Export Servant_Manager_Request_Processing_Strategy :
       public virtual Request_Processing_Strategy
    {
    public:
      virtual ~Servant_Manager_Request_Processing_Strategy (void);
    };

    class TAO_PortableServer_Export Servant_Activator_Request_Processing_Strategy :
       public virtual Servant_Manager_Request_Processing_Strategy
    {
    public:
      virtual ~Servant_Manager_Request_Processing_Strategy (void);
    };

    class TAO_PortableServer_Export Servant_Locator_Request_Processing_Strategy :
       public virtual Servant_Manager_Request_Processing_Strategy
    {
    public:
      virtual ~Servant_Manager_Request_Processing_Strategy (void);
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_ID_ASSIGNMENT_STRATEGY_H */
