// -*- C++ -*-

//=============================================================================
/**
 *  @file Lifespan_Strategy.h
 *
 *  $Id$
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_LIFESPAN_STRATEGY_H
#define TAO_LIFESPAN_STRATEGY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "PolicyFactory.h"
#include "PortableServerC.h"
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
    class TAO_PortableServer_Export Lifespan_Strategy :
       public virtual Policy_Strategy
    {
    public:
      virtual ~Lifespan_Strategy (void);

      void init(CORBA::PolicyList *policy_list)
      {
        // dependent on type create the correct strategy.
      }

      void
      create (const char *name,
              const TAO::ObjectKey &key);
    };

    class TAO_PortableServer_Export Transient_Lifespan_Strategy :
       public virtual Transient_Lifespan_Strategy
    {
    public:
      virtual ~Transient_Lifespan_Strategy (void);

    private:
      TAO_Creation_Time creation_time_;
    };

    class TAO_PortableServer_Export Persistent_Lifespan_Strategy :
       public virtual Lifespan_Strategy
    {
    public:
      virtual ~Persistent_Lifespan_Strategy (void);

    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_LIFESPAN_STRATEGY_H */
