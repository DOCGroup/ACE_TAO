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

#ifndef TAO_SERVANT_RETENTION_STRATEGY_H
#define TAO_SERVANT_RETENTION_STRATEGY_H
#include /**/ "ace/pre.h"

#include "portableserver_export.h"
#include "PortableServerC.h"
#include "Policy_Strategy.h"
#include "Active_Object_Map.h"
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
      virtual ~Servant_Retention_Strategy (void);

      virtual void strategy_init (TAO_POA *poa, CORBA::PolicyList *policy_list)
      {
        // dependent on type create the correct strategy.
      }

      virtual bool is_servant_in_map (PortableServer::Servant servant,
                                      int &wait_occurred_restart_call) = 0;

      virtual void deactivate_object (const PortableServer::ObjectId &id
                                      ACE_ENV_ARG_DECL) = 0;

    };

    class TAO_PortableServer_Export Retain_Servant_Retention_Strategy :
       public virtual Servant_Retention_Strategy
    {
    public:
      virtual ~Retain_Servant_Retention_Strategy (void);

      void deactivate_object (const PortableServer::ObjectId &id
                              ACE_ENV_ARG_DECL);

    protected:
      void
      deactivate_map_entry (TAO_Active_Object_Map::Map_Entry *active_object_map_entry
                            ACE_ENV_ARG_DECL);

      void
      cleanup_servant (
        TAO_Active_Object_Map::Map_Entry *active_object_map_entry
        ACE_ENV_ARG_DECL);

    private:
      TAO_Active_Object_Map *active_object_map_;
    };

    class TAO_PortableServer_Export Non_Retain_Servant_Retention_Strategy :
       public virtual Servant_Retention_Strategy
    {
    public:
      virtual ~Non_Retain_Servant_Retention_Strategy (void);

      virtual void deactivate_object (const PortableServer::ObjectId &id
                                      ACE_ENV_ARG_DECL);
    };
  }
}

#endif /* TAO_HAS_MINIMUM_POA == 0 */

#include /**/ "ace/post.h"
#endif /* TAO_SERVANT_RETENTION_STRATEGY_H */
