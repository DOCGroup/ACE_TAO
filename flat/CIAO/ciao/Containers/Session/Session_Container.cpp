// $Id$

#include "Session_Container.h"

#include <tao/Utils/PolicyList_Destroyer.h>
#include <ciao/CIAO_common.h>
#include <ciao/Containers/Servant_Activator.h>
#include <ccm/ComponentServer/ComponentServer_BaseC.h>

#if !defined (__ACE_INLINE__)
# include "Session_Container.inl"
#endif /* __ACE_INLINE__ */

namespace CIAO
{
  ///////////////////////////////////////////////////////////////

  ACE_Atomic_Op <TAO_SYNCH_MUTEX, unsigned long>
  Session_Container::serial_number_ (0);
  
  Session_Container::Session_Container (CORBA::ORB_ptr o,
                                        Deployment::CIAO_Container_i *container_impl,
                                        bool static_config_flag,
                                        const Static_Config_EntryPoints_Maps* maps,
					const char *name,
					const CORBA::PolicyList *more_policies)
    : Container_i (o, container_impl),
      number_ (0),
      static_config_flag_ (static_config_flag),
      static_entrypts_maps_ (maps),
      sa_ (0)
  {
    this->init (name, more_policies);
  }

  Session_Container::~Session_Container (void)
  {
    if (! CORBA::is_nil (this->component_poa_.in ()))
      {
        this->component_poa_->destroy (1, 1);
      }

    if (! CORBA::is_nil (this->facet_cons_poa_.in ()))
      {
        this->facet_cons_poa_->destroy (1, 1);
      }

    if (! CORBA::is_nil (this->home_servant_poa_.in ()))
      {
        this->home_servant_poa_->destroy (1, 1);
      }

    // delete this->sa_;
  }

  void
  Session_Container::init (const char *name,
                           const CORBA::PolicyList *more_policies)
  {
    CIAO_TRACE ("Session_Container::init");

    char buffer[MAXPATHLEN];

    if (name == 0)
      {
        this->number_ = ++Session_Container::serial_number_;
        ACE_OS::sprintf (buffer,
                         "CIAO::Session_Container-%ld",
                         this->number_);
        name = buffer;
      }

    CORBA::Object_var poa_object =
      this->orb_->resolve_initial_references ("RootPOA");

    if (CORBA::is_nil (poa_object.in ()))
      {
        CIAO_ERROR ((LM_ERROR, CLINFO
		    "CIAO::Session_Container: Unable to initialize the POA.\n"));
	throw Components::CreateFailure ();		   
      }

    PortableServer::POA_var root_poa =
      PortableServer::POA::_narrow (poa_object.in ());

    this->create_component_POA (name,
                                more_policies,
                                root_poa.in ());

    ACE_CString port_poa_name (name);
    port_poa_name += ":Port_POA";
    this->create_facet_consumer_POA (port_poa_name.c_str (),
                                     more_policies,
                                     root_poa.in ());

    PortableServer::POAManager_var poa_manager =
      root_poa->the_POAManager ();

    poa_manager->activate ();
  }

  void
  Session_Container::create_component_POA (const char *name,
                                           const CORBA::PolicyList *p,
                                           PortableServer::POA_ptr root)
  {
    CIAO_TRACE ("Session_Container::create_component_POA");

    // Set up proper poa policies here.  Default policies seems to be
    // fine for session container.  If you add some other default
    // policies here, then you need to "add" more_policies below
    // instead of simply assigning more_policies to the init policy
    // list.
    CORBA::PolicyList policies (0);

    if (p != 0)
      {
        policies = *p;
      }

    PortableServer::POAManager_var poa_manager =
      root->the_POAManager ();

    this->component_poa_ =
      root->create_POA (name,
                        poa_manager.in (),
                        policies);
  }

  void
  Session_Container::create_facet_consumer_POA (const char *name,
                                                const CORBA::PolicyList *p,
                                                PortableServer::POA_ptr root)
  {
    CIAO_TRACE ("Session_Container::create_facet_consumer_POA");

    PortableServer::POAManager_var poa_manager = root->the_POAManager ();
    CORBA::ULong p_length = 0;
    
    if (p != 0)
      {
        p_length = p->length ();
      }

    TAO::Utils::PolicyList_Destroyer policies (p_length + 3);
    policies.length (p_length + 3);

    policies[0] =
      root->create_id_assignment_policy (PortableServer::USER_ID);

    // Servant Manager Policy
    policies[1] =
      root->create_request_processing_policy (PortableServer::USE_SERVANT_MANAGER);

    // Servant Retention Policy
    policies[2] =
      root->create_servant_retention_policy (PortableServer::RETAIN);

    for (CORBA::ULong i = 0; i < p_length; ++i)
      {
        policies[i+3] = (*p)[i];
      }

    this->facet_cons_poa_ =
      root->create_POA (name,
                        poa_manager.in (),
                        policies);

    Servant_Activator_i *sa;
    ACE_NEW_THROW_EX (sa,
                      Servant_Activator_i (this->orb_.in ()),
                      CORBA::NO_MEMORY ());
    this->sa_ = sa;

    this->facet_cons_poa_->set_servant_manager (this->sa_.in ());
  }

  CORBA::Object_ptr
  Session_Container::install_servant (PortableServer::Servant p,
                                      Container_Types::OA_Type t,
                                      PortableServer::ObjectId_out oid)
  {
    CIAO_TRACE ("Session_Container::install_servant");

    PortableServer::POA_ptr tmp = 0;

    if (t == Container_Types::COMPONENT_t ||
        t == Container_Types::HOME_t)
      {
        tmp = this->component_poa_.in ();
      }
    else
      {
        tmp = this->facet_cons_poa_.in ();
      }
    
    PortableServer::ObjectId_var tmp_id = tmp->activate_object (p);

    CORBA::Object_var objref = tmp->id_to_reference (tmp_id.in ());
    oid = tmp_id._retn ();

    return objref._retn ();
  }

  //@@ Apparently we need to be cautious when handling the exception
  //   thrown here. We should make sure that new DnC interfaces
  //   NodeApplication/NodeApplicationManager etc will cache the new
  //   exceptions--> rethrow of new exceptions is needed.
  //                                            --Tao
  Components::CCMHome_ptr
  Session_Container::install_home (const char *primary_artifact,
                                   const char *entry_point,
                                   const char *servant_artifact,
                                   const char *servant_entrypoint,
                                   const char *name)
  {
    CIAO_TRACE ("Session_Container::install_home");
    
    HomeFactory hcreator = 0;
    HomeServantFactory screator = 0;

    if (this->static_config_flag_ == false)
      {
        CIAO_DEBUG ((LM_DEBUG, CLINFO
                    "Session_Container::install_home - "
                    "Loading home [%s] from shared libraries\n",
                    name));
        CIAO_DEBUG ((LM_DEBUG, CLINFO
                    "Session_Container::install_home - "
                    "Executor library [%s] with entrypoint [%s]\n",
                    primary_artifact, entry_point));
        CIAO_DEBUG ((LM_DEBUG, CLINFO
                    "Session_Container::install_home - "
                    "Servant library [%s] with entrypoint [%s]\n",
                    servant_artifact, servant_entrypoint));
        
        ACE_DLL executor_dll, servant_dll;

        if (primary_artifact == 0 || servant_artifact == 0)
          {
            ACE_CString exception;

            if (primary_artifact == 0)
              {
                exception = "Null component executor DLL name";
              }

            if (servant_artifact == 0)
              {
                exception = "Null component servant DLL name";
              }

            CIAO_ERROR ((LM_ERROR, CLINFO
                        "Session_Container.cpp -"
                        "Session_Container::ciao_install_home -"
                        "ERROR: %s\n",
                        exception.c_str ()));

            throw Components::Deployment::UnknownImplId ();
          }
        
        if (executor_dll.open (primary_artifact,
                               ACE_DEFAULT_SHLIB_MODE,
                               0) != 0)
          {
            ACE_CString error ("Failed to open executor DLL: ");
            error += primary_artifact;

            CIAO_ERROR ((LM_ERROR, CLINFO
                        "Session_Container.cpp -"
                        "Session_Container::ciao_install_home -"
                        "ERROR in opening the executor DLL [%s] \n",
                        primary_artifact));

            throw Components::Deployment::UnknownImplId ();
          }
        
        CIAO_DEBUG ((LM_TRACE, CLINFO
                    "Session_Container::install_home - Executor DLL successfully opened\n"));
        
        if (servant_dll.open (servant_artifact,
                              ACE_DEFAULT_SHLIB_MODE,
                              0) != 0)
          {
            ACE_CString error ("Failed to open executor DLL: ");
            error += servant_artifact;

            CIAO_ERROR ((LM_ERROR, CLINFO
                        "Session_Container.cpp -"
                        "Session_Container::ciao_install_home -"
                        "ERROR in opening the servant DLL [%s] \n",
                        servant_artifact));

            throw Components::Deployment::UnknownImplId ();
          }
        
        CIAO_DEBUG ((LM_TRACE, CLINFO
                    "Session_Container::install_home - Servant DLL successfully openend.\n"));
        
        if (entry_point == 0 || servant_entrypoint == 0)
          {
            ACE_CString error ("Entry point is null for ");

            if (entry_point == 0)
              {
                CIAO_ERROR ((LM_ERROR, CLINFO
                            "Session_Container.cpp -"
                            "Session_Container::ciao_install_home -"
                            "ERROR in opening the executor entry point "
                            "for executor DLL [%s] \n",
                            primary_artifact));
                error += primary_artifact;
              }
            else
              {
                CIAO_ERROR ((LM_ERROR, CLINFO
                            "Session_Container.cpp -"
                            "Session_Container::ciao_install_home -"
                            "ERROR in opening the servant entry point "
                            "for servant DLL [%s] \n",
                            servant_artifact));
                error += servant_artifact;
              }

            throw Components::Deployment::ImplEntryPointNotFound ();
          }

        // We have to do this casting in two steps because the C++
        // standard forbids casting a pointer-to-object (including
        // void*) directly to a pointer-to-function.
        void *void_ptr = executor_dll.symbol (entry_point);
        ptrdiff_t tmp_ptr = reinterpret_cast<ptrdiff_t> (void_ptr);
        hcreator = reinterpret_cast<HomeFactory> (tmp_ptr);

        void_ptr = servant_dll.symbol (servant_entrypoint);
        tmp_ptr = reinterpret_cast<ptrdiff_t> (void_ptr);
        screator = reinterpret_cast<HomeServantFactory> (tmp_ptr);
      }
    else
      {
        CIAO_DEBUG ((LM_DEBUG, CLINFO
                    "Session_Container::install_home - Loading statically linked home [%s]\n",
                    name));

        if (static_entrypts_maps_ == 0
            || static_entrypts_maps_->home_creator_funcptr_map_ == 0
            || static_entrypts_maps_->home_servant_creator_funcptr_map_ == 0)
          {
            CIAO_DEBUG ((LM_ERROR, CLINFO,
                        "Session_Container::install_home - ERROR: Static entrypoint "
                        "maps are null or imcomplete.\n"));            
            throw Components::Deployment::ImplEntryPointNotFound ();
          }

        ACE_CString entry_point_str (entry_point);
        static_entrypts_maps_->home_creator_funcptr_map_->find (entry_point_str,
                                                                hcreator);

        ACE_CString servant_entrypoint_str (servant_entrypoint);
        static_entrypts_maps_->home_servant_creator_funcptr_map_->find (servant_entrypoint_str,
                                                                        screator);
      }

    if (hcreator == 0 || screator == 0)
      {
        ACE_CString error ("Entry point ");

        if (hcreator == 0)
          {
            error += entry_point;
            error += " invalid in dll ";
            error += primary_artifact;
          }
        else
          {
            error += servant_entrypoint;
            error += " invalid in dll ";
            error += servant_artifact;
          }
	
	CIAO_ERROR ((LM_ERROR, CLINFO
		    "Session_Container::ciao_install_home - Error:%s\n",
		    error.c_str ()));
		    
        throw Components::Deployment::ImplEntryPointNotFound ();
      }
    
    CIAO_DEBUG ((LM_TRACE, CLINFO,
                "Session_Container::install_home - Loading home executor\n"));
    Components::HomeExecutorBase_var home_executor = hcreator ();

    if (CORBA::is_nil (home_executor.in ()))
      {
	CIAO_ERROR ((LM_ERROR, CLINFO
		    "Session_Container::Ciao_install_hoe - Home executor factory failed. \n"));
        throw Components::Deployment::InstallationFailure ();
      }

    CIAO_DEBUG ((LM_TRACE, CLINFO,
                "Session_Container::install_home - Loading home servant\n"));
    PortableServer::Servant home_servant = screator (home_executor.in (),
                                                     this,
                                                     name);

    if (home_servant == 0)
      {
	CIAO_ERROR ((LM_ERROR, CLINFO
		    "Session_Container::ciao_install_home - Home servant factory failed.\n"));
        throw Components::Deployment::InstallationFailure ();
      }

    PortableServer::ServantBase_var safe (home_servant);

    CIAO_DEBUG ((LM_TRACE, CLINFO,
                "Session_Container::install_home - Installing home servant\n"));
    
    PortableServer::ObjectId_var oid;
    
    CORBA::Object_var objref =
      this->install_servant (home_servant, Container_Types::COMPONENT_t, oid.out ());

    Components::CCMHome_var homeref =
      Components::CCMHome::_narrow (objref.in ());

    CIAO_DEBUG ((LM_TRACE, CLINFO,
                "Session_Container::install_home - Home successfully created with name\n"));
    
    return homeref._retn ();
  }

  Components::CCMObject_ptr
  Session_Container::install_component (const char *primary_artifact,
                                        const char *entry_point,
                                        const char *servant_artifact,
                                        const char *servant_entrypoint,
                                        const char *name)
  {
    CIAO_TRACE ("Session_Container::install_component");
    
    ComponentFactory ccreator = 0;
    ComponentServantFactory screator = 0;

    if (this->static_config_flag_ == false)
      {
        CIAO_DEBUG ((LM_DEBUG, CLINFO
                    "Session_Container::install_component - "
                    "Loading component [%s] from shared libraries\n",
                    name));
        CIAO_DEBUG ((LM_DEBUG, CLINFO
                    "Session_Container::install_component - "
                    "Executor library [%s] with entrypoint [%s]\n",
                    primary_artifact, entry_point));
        CIAO_DEBUG ((LM_DEBUG, CLINFO
                    "Session_Container::install_component - "
                    "Servant library [%s] with entrypoint [%s]\n",
                    servant_artifact, servant_entrypoint));
        
        ACE_DLL executor_dll, servant_dll;

        if (primary_artifact == 0 || servant_artifact == 0)
          {
            ACE_CString exception;

            if (primary_artifact == 0)
              {
                exception = "Null component executor DLL name";
              }

            if (servant_artifact == 0)
              {
                exception = "Null component servant DLL name";
              }

            CIAO_ERROR ((LM_ERROR, CLINFO
                        "Session_Container.cpp -"
                        "Session_Container::ciao_install_component -"
                        "ERROR: %s\n",
                        exception.c_str ()));

            throw Components::Deployment::UnknownImplId ();
          }
        
        if (executor_dll.open (primary_artifact,
                               ACE_DEFAULT_SHLIB_MODE,
                               0) != 0)
          {
            ACE_CString error ("Failed to open executor DLL: ");
            error += primary_artifact;

            CIAO_ERROR ((LM_ERROR, CLINFO
                        "Session_Container.cpp -"
                        "Session_Container::ciao_install_component -"
                        "ERROR in opening the executor DLL [%s] \n",
                        primary_artifact));

            throw Components::Deployment::UnknownImplId ();
          }
        
        CIAO_DEBUG ((LM_TRACE, CLINFO
                    "Session_Container::install_component - Executor DLL successfully opened\n"));
        
        if (servant_dll.open (servant_artifact,
                              ACE_DEFAULT_SHLIB_MODE,
                              0) != 0)
          {
            ACE_CString error ("Failed to open executor DLL: ");
            error += servant_artifact;

            CIAO_ERROR ((LM_ERROR, CLINFO
                        "Session_Container.cpp -"
                        "Session_Container::ciao_install_component -"
                        "ERROR in opening the servant DLL [%s] \n",
                        servant_artifact));

            throw Components::Deployment::UnknownImplId ();
          }
        
        CIAO_DEBUG ((LM_TRACE, CLINFO
                    "Session_Container::install_component - Servant DLL successfully openend.\n"));
        
        if (entry_point == 0 || servant_entrypoint == 0)
          {
            ACE_CString error ("Entry point is null for ");

            if (entry_point == 0)
              {
                CIAO_ERROR ((LM_ERROR, CLINFO
                            "Session_Container.cpp -"
                            "Session_Container::ciao_install_component -"
                            "ERROR in opening the executor entry point "
                            "for executor DLL [%s] \n",
                            primary_artifact));
                error += primary_artifact;
              }
            else
              {
                CIAO_ERROR ((LM_ERROR, CLINFO
                            "Session_Container.cpp -"
                            "Session_Container::ciao_install_component -"
                            "ERROR in opening the servant entry point "
                            "for servant DLL [%s] \n",
                            servant_artifact));
                error += servant_artifact;
              }

            throw Components::Deployment::ImplEntryPointNotFound ();
          }

        // We have to do this casting in two steps because the C++
        // standard forbids casting a pointer-to-object (including
        // void*) directly to a pointer-to-function.
        void *void_ptr = executor_dll.symbol (entry_point);
        ptrdiff_t tmp_ptr = reinterpret_cast<ptrdiff_t> (void_ptr);
        ccreator = reinterpret_cast<ComponentFactory> (tmp_ptr);

        void_ptr = servant_dll.symbol (servant_entrypoint);
        tmp_ptr = reinterpret_cast<ptrdiff_t> (void_ptr);
        screator = reinterpret_cast<ComponentServantFactory> (tmp_ptr);
      }
    else
      {
        CIAO_DEBUG ((LM_DEBUG, CLINFO
                    "Session_Container::install_component - Loading statically linked component [%s]\n",
                    name));

        if (static_entrypts_maps_ == 0
            || static_entrypts_maps_->component_creator_funcptr_map_ == 0
            || static_entrypts_maps_->component_servant_creator_funcptr_map_ == 0)
          {
            CIAO_DEBUG ((LM_ERROR, CLINFO,
                        "Session_Container::install_component - ERROR: Static entrypoint "
                        "maps are null or imcomplete.\n"));            
            throw Components::Deployment::ImplEntryPointNotFound ();
          }

        ACE_CString entry_point_str (entry_point);
        static_entrypts_maps_->component_creator_funcptr_map_->find (entry_point_str,
                                                                     ccreator);

        ACE_CString servant_entrypoint_str (servant_entrypoint);
        static_entrypts_maps_->component_servant_creator_funcptr_map_->find (servant_entrypoint_str,
                                                                             screator);
      }

    if (ccreator == 0 || screator == 0)
      {
        ACE_CString error ("Entry point ");

        if (ccreator == 0)
          {
            error += entry_point;
            error += " invalid in dll ";
            error += primary_artifact;
          }
        else
          {
            error += servant_entrypoint;
            error += " invalid in dll ";
            error += servant_artifact;
          }
	
	CIAO_ERROR ((LM_ERROR, CLINFO
		    "Session_Container::ciao_install_component - Error:%s\n",
		    error.c_str ()));
		    
        throw Components::Deployment::ImplEntryPointNotFound ();
      }
    
    CIAO_DEBUG ((LM_TRACE, CLINFO,
                "Session_Container::install_component - Loading component executor\n"));
    Components::EnterpriseComponent_var component_executor = ccreator ();

    if (CORBA::is_nil (component_executor.in ()))
      {
	CIAO_ERROR ((LM_ERROR, CLINFO
		    "Session_Container::Ciao_install_hoe - Component executor factory failed. \n"));
        throw Components::Deployment::InstallationFailure ();
      }

    CIAO_DEBUG ((LM_TRACE, CLINFO,
                "Session_Container::install_component - Loading component servant\n"));
    PortableServer::Servant component_servant = screator (component_executor.in (),
                                                          name,
                                                          this);

    if (component_servant == 0)
      {
	CIAO_ERROR ((LM_ERROR, CLINFO
		    "Session_Container::ciao_install_component - Component servant factory failed.\n"));
        throw Components::Deployment::InstallationFailure ();
      }

    PortableServer::ServantBase_var safe (component_servant);

    CIAO_DEBUG ((LM_TRACE, CLINFO,
                "Session_Container::install_component - Installing component servant\n"));
    
    PortableServer::ObjectId_var oid;
    
    CORBA::Object_var objref =
      this->install_servant (component_servant, Container_Types::COMPONENT_t, oid.out ());

    Components::CCMObject_var componentref =
      Components::CCMObject::_narrow (objref.in ());

    CIAO_DEBUG ((LM_TRACE, CLINFO,
                "Session_Container::install_component - Component successfully created\n"));
    
    return componentref._retn ();
  }
  
  void 
  Session_Container::uninstall (CORBA::Object_ptr objref, Container_Types::OA_Type y)
  {
    CIAO_TRACE ("Session_Container::uninstall_home");
    throw CORBA::NO_IMPLEMENT ();
  }
  
  void
  Session_Container::uninstall_home (Components::CCMHome_ptr homeref)
  {
    CIAO_TRACE ("Session_Container::uninstall_home");

    this->uninstall (homeref, Container_Types::HOME_t);
  }

  void
  Session_Container::uninstall_component (Components::CCMObject_ptr homeref)
  {
    CIAO_TRACE ("Session_Container::uninstall_component");

    this->uninstall (homeref, Container_Types::COMPONENT_t);
  }

  void
  Session_Container::uninstall_servant (PortableServer::Servant svnt,
                                        Container_Types::OA_Type t,
                                        PortableServer::ObjectId_out oid)
  {
    CIAO_TRACE ("Session_Container::uninstall_servant");

    PortableServer::POA_ptr tmp = 0;

    if ((t == Container_Types::COMPONENT_t) ||
        (t == Container_Types::HOME_t))
      {
        tmp = this->component_poa_.in ();
      }
    else
      {
        tmp = this->facet_cons_poa_.in ();
      }

    PortableServer::ObjectId_var tmp_id;
    tmp_id = tmp->servant_to_id (svnt);
    tmp->deactivate_object (tmp_id);
    
    oid = tmp_id._retn ();
  }

  void
  Session_Container::add_servant_to_map (PortableServer::ObjectId &,
					 Dynamic_Component_Servant_Base*)
  {
    CIAO_TRACE ("Session_Container::add_servant_to_map");
    throw CORBA::NO_IMPLEMENT ();
  }

  void
  Session_Container::delete_servant_from_map (PortableServer::ObjectId &)
  {
    CIAO_TRACE ("Session_Container::delete_servant_from_map");
    throw CORBA::NO_IMPLEMENT ();
  }

  void
  Session_Container::deactivate_facet (const PortableServer::ObjectId &)
  {
    CIAO_TRACE ("Session_Container::deactivate_facet");
  }

  CORBA::Object_ptr
  Session_Container::get_home_objref (PortableServer::Servant)
  {
    CIAO_TRACE ("Session_Container::get_home_objref");
    throw CORBA::NO_IMPLEMENT ();
  }

  CORBA::Object_ptr
  Session_Container::generate_reference (const char *obj_id,
                                         const char *repo_id,
                                         Container_Types::OA_Type t)
  {
    CIAO_TRACE ("Session_Container::generate_reference");

    PortableServer::POA_ptr tmp = 0;

    if (t == Container_Types::COMPONENT_t ||
        t == Container_Types::HOME_t)
      {
        tmp = this->component_poa_.in ();
      }
    else
      {
        tmp = this->facet_cons_poa_.in ();
      }

    PortableServer::ObjectId_var oid =
      PortableServer::string_to_ObjectId (obj_id);

    CORBA::String_var str = PortableServer::ObjectId_to_string (oid.in ());

    CORBA::Object_var objref =
      tmp->create_reference_with_id (oid.in (), repo_id);

    return objref._retn ();
  }
}
