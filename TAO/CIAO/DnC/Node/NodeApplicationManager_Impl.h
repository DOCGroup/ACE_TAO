//$Id$:
/**
 *
 * @file NodeApplicationManager_Impl.h
 *
 * @auther Tao Lu <lu@dre.vanderbilt.edu>
 *
 **/

#ifndef NODEAPPLICATIONMANAGER_IMPL_H
#define NODEAPPLICATIONMANAGER_IMPL_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch_Traits.h"
#include "DnC_DeploymentS.h"

namespace Deployment
{

  /**
   *
   * @class NodeApplicationManager_Impl
   *
   * @brief This class implements the NodeApplicationManager interface.
   *        This interface is very simillar to the old DnC CIAO_Daemon.
   *
   **/
  class NodeApplicationManager_Impl
    : public virtual POA_Deployment::NodeApplicationManager
    {
    public:

      // constructor.
      NodeApplicationManager (NodeApplicationManagerManager_ptr nam_ptr,
		       ACE_ENV_ARG_DECL_WITH_DEFAULTS);

      // Default destructor.
      virtual ~NodeApplicationManager (void);

      /**
       * @method startLaunch
       *
       * @brief
       *
       */
      virtual ::Deployment::Application_ptr
      startLaunch (const Deployment::Properties & configProperty,
		   Deployment::Connections_out providedReference,
		   CORBA::Boolean start
                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
	ACE_THROW_SPEC ((
			 CORBA::SystemException,
			 Deployment::ResourceNotAvailable,
			 Deployment::StartError,
			 Deployment::InvalidProperty
		       ));

      /**
       * @method start
       *
       * @brief
       *
       */
      irtual void destroyApplication (Deployment::Application_ptr app
				      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
	ACE_THROW_SPEC ((
			 CORBA::SystemException,
			 Deployment::StopError
    		       ));

      /**
       * @name internal operations using the ACE_Map methods
       */
      int obj_bind (const char *orb_id, CORBA::Object_ptr obj);
      int obj_unbind (const char *orb_id);

      int na_bind (const char *orb_id, ::Deployment::NodeApplication_ptr na);
      int na_unbind (const char *orb_id);

    protected:

      typedef ACE_Hash_Map_Manager_Ex<const char *,
	                              CORBA::Object_ptr,
                                      ACE_Hash<const char *>,
	                              ACE_Equal_To<const char *>,
                                      TAO_SYNCH_MUTEX> OBJ_Table;
      typedef OBJ_Table::iterator OBJ_Iterator;

      // CIAO's Object_Set<> could be used here as well.
      // It takes o(n) time to do find though.
      typedef ACE_Hash_Map_Manager_Ex<const char *,
	                              ::Deployment::NodeApplication_ptr,
                                      ACE_Hash<const char *>,
	                              ACE_Equal_To<const char *>,
                                      TAO_SYNCH_MUTEX> NodeApplication_Table;
      typedef NodeApplication_Table::iterator NodeApplication_Iterator;

      //Cached Plan
      DeploymentPlan_var plan_;

      //Launched Application(NodeApplication) but not terminated yet.
      NodeApplication_Table nodeapplication_table;

      // Cached NodeManager
      NodeManager_ptr node_manager_;

      // Cached DomainApplication
      DomainApplication_ptr domain_application_;

      // Keep a pointer to the managing ORB serving this servant.
      CORBA::ORB_var orb_;

      // Keep a pointer to the managing POA.
      PortableServer::POA_var poa_;

      // My Canonical name.
      CORBA::String_var name_;

      // Internal object hash table.
      OBJ_Table OBJ_table_;


    }; /* class NodeApplicationManager_Impl */


}; /* namespace Deployment */

#endif /* NODEAPPLICATIONMANAGER_IMPL_H */
