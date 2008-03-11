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
        ACE_ERROR ((LM_ERROR, CLINFO
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
      root->create_request_processing_policy (
					      PortableServer::USE_SERVANT_MANAGER);

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
                                      Container_Types::OA_Type t)
  {
    CIAO_TRACE ("Session_Container::install_servant");

    PortableServer::POA_ptr tmp = 0;

    if (t == Container_Types::COMPONENT_t)
      {
        tmp = this->component_poa_.in ();
      }
    else
      {
        tmp = this->facet_cons_poa_.in ();
      }

    PortableServer::ObjectId_var oid = tmp->activate_object (p);

    CORBA::Object_var objref = tmp->id_to_reference (oid.in ());

    return objref._retn ();
  }

  CORBA::Object_ptr
  Session_Container::install_component_servant (PortableServer::Servant p,
                                                PortableServer::ObjectId_out oid)
  {
    CIAO_TRACE ("Session_Container::install_component");
    PortableServer::ObjectId_var id =
      this->component_poa_->activate_object (p);

    CORBA::Object_var objref =
      this->component_poa_->id_to_reference (id.in ());

    oid = id._retn ();

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
                                   const char *nameconst)
  {
    CIAO_TRACE ("Session_Container::ciao_install_home");
    throw CORBA::NO_IMPLEMENT ();
    /*
    HomeFactory hcreator = 0;
    ServantFactory screator = 0;

    if (this->static_config_flag_ == false)
      {
        ACE_DLL executor_dll, servant_dll;

        if (exe_dll_name == 0 || sv_dll_name == 0)
          {
            ACE_CString exception;

            if (exe_dll_name == 0)
              {
                exception = "Null component executor DLL name";
              }

            if (sv_dll_name == 0)
              {
                exception = "Null component servant DLL name";
              }

            ACE_ERROR ((LM_ERROR, CLINFO
                        "Session_Container.cpp -"
                        "Session_Container::ciao_install_home -"
                        "ERROR: %s\n",
                        exception.c_str ()));

            throw Components::Deployment::UnknownImplId ();
          }

        if (executor_dll.open (exe_dll_name,
                               ACE_DEFAULT_SHLIB_MODE,
                               0) != 0)
          {
            ACE_CString error ("Failed to open executor DLL: ");
            error += exe_dll_name;

            ACE_ERROR ((LM_ERROR, CLINFO
                        "Session_Container.cpp -"
                        "Session_Container::ciao_install_home -"
                        "ERROR in opening the executor DLL [%s] \n",
                        exe_dll_name));

            throw Components::Deployment::UnknownImplId ();
          }

        if (servant_dll.open (sv_dll_name,
                              ACE_DEFAULT_SHLIB_MODE,
                              0) != 0)
          {
            ACE_CString error ("Failed to open executor DLL: ");
            error += sv_dll_name;

            ACE_ERROR ((LM_ERROR, CLINFO
                        "Session_Container.cpp -"
                        "Session_Container::ciao_install_home -"
                        "ERROR in opening the servant DLL [%s] \n",
                        sv_dll_name));

            throw Components::Deployment::UnknownImplId ();
          }

        if (exe_entrypt == 0 || sv_entrypt == 0)
          {
            ACE_CString error ("Entry point is null for ");

            if (exe_entrypt == 0)
              {
                ACE_ERROR ((LM_ERROR, CLINFO
                            "Session_Container.cpp -"
                            "Session_Container::ciao_install_home -"
                            "ERROR in opening the executor entry point "
                            "for executor DLL [%s] \n",
                            exe_dll_name));
                error += exe_dll_name;
              }
            else
              {
                ACE_ERROR ((LM_ERROR, CLINFO
                            "Session_Container.cpp -"
                            "Session_Container::ciao_install_home -"
                            "ERROR in opening the servant entry point "
                            "for servant DLL [%s] \n",
                            sv_dll_name));
                error += sv_dll_name;
              }

            throw Components::Deployment::ImplEntryPointNotFound ();
          }

        // We have to do this casting in two steps because the C++
        // standard forbids casting a pointer-to-object (including
        // void*) directly to a pointer-to-function.
        void *void_ptr = executor_dll.symbol (exe_entrypt);
        ptrdiff_t tmp_ptr = reinterpret_cast<ptrdiff_t> (void_ptr);
        hcreator = reinterpret_cast<HomeFactory> (tmp_ptr);

        void_ptr = servant_dll.symbol (sv_entrypt);
        tmp_ptr = reinterpret_cast<ptrdiff_t> (void_ptr);
        screator = reinterpret_cast<ServantFactory> (tmp_ptr);
      }
    else
      {
        if (static_entrypts_maps_ == 0
            || static_entrypts_maps_->home_creator_funcptr_map_ == 0
            || static_entrypts_maps_->home_servant_creator_funcptr_map_ == 0)
          {
            throw Components::Deployment::ImplEntryPointNotFound ();
          }

        ACE_CString exe_entrypt_str (exe_entrypt);
        static_entrypts_maps_->home_creator_funcptr_map_->find (exe_entrypt_str,
                                                                hcreator);

        ACE_CString sv_entrypt_str (sv_entrypt);
        static_entrypts_maps_->home_servant_creator_funcptr_map_->find (
                                                                        sv_entrypt_str,
                                                                        screator
                                                                        );
      }

    if (hcreator == 0 || screator == 0)
      {
        ACE_CString error ("Entry point ");

        if (hcreator == 0)
          {
            error += exe_entrypt;
            error += " invalid in dll ";
            error += exe_dll_name;
          }
        else
          {
            error += sv_entrypt;
            error += " invalid in dll ";
            error += sv_dll_name;
          }
	
	ACE_ERROR ((LM_ERROR, CLINFO
		    "Session_Container::ciao_install_home - Error:%s\n",
		    error.c_str ()));
		    
        throw Components::Deployment::ImplEntryPointNotFound ();
      }

    Components::HomeExecutorBase_var home_executor = hcreator ();

    if (CORBA::is_nil (home_executor.in ()))
      {
	ACE_ERROR ((LM_ERROR, CLINFO
		    "Session_Container::Ciao_install_hoe - Home executor factory failed. \n"));
        throw Components::Deployment::InstallationFailure ();
      }

    PortableServer::Servant home_servant = screator (home_executor.in (),
                                                     this,
                                                     ins_name);

    if (home_servant == 0)
      {
	ACE_ERROR ((LM_ERROR, CLINFO
		    "Session_Container::ciao_install_home - Home servant factory failed.\n"));
        throw Components::Deployment::InstallationFailure ();
      }

    PortableServer::ServantBase_var safe (home_servant);

    CORBA::Object_var objref =
      this->install_servant (home_servant, Container::Component);

    Components::CCMHome_var homeref =
      Components::CCMHome::_narrow (objref.in ());

    return homeref._retn ();
    */
  }

  void
  Session_Container::uninstall_home (Components::CCMHome_ptr homeref)
  {
    CIAO_TRACE ("Session_Container::ciao_uninstall_home");

    this->uninstall (homeref, Container_Types::COMPONENT_t);
  }

  void
  Session_Container::uninstall (CORBA::Object_ptr objref,
                                Container_Types::OA_Type t)
  {
    CIAO_TRACE ("Session_Container::uninstall");

    PortableServer::POA_ptr tmp = 0;

    if (t == Container_Types::COMPONENT_t)
      {
        tmp = this->component_poa_.in ();
      }
    else
      {
        tmp = this->facet_cons_poa_.in ();
      }

    PortableServer::ObjectId_var oid =
      tmp->reference_to_id (objref);

    tmp->deactivate_object (oid.in ());
  }

  void
  Session_Container::uninstall (PortableServer::Servant svt,
                                Container_Types::OA_Type t)
  {
    CIAO_TRACE ("Session_Container::uninstall");
    PortableServer::POA_ptr tmp = 0;

    if (t == Container_Types::COMPONENT_t)
      {
        tmp = this->component_poa_.in ();
      }
    else
      {
        tmp = this->facet_cons_poa_.in ();
      }

    PortableServer::ObjectId_var oid = tmp->servant_to_id (svt);

    tmp->deactivate_object (oid.in ());
  }

  void
  Session_Container::uninstall_component (Components::CCMObject_ptr objref,
                                          PortableServer::ObjectId_out oid)
  {
    CIAO_TRACE ("Session_Container::uninstall_component");

    PortableServer::ObjectId_var id =
      this->component_poa_->reference_to_id (objref);

    this->component_poa_->deactivate_object (id.in ());

    oid = id._retn ();
  }

  void
  Session_Container::add_servant_to_map (PortableServer::ObjectId &,
					 Dynamic_Component_Servant_Base*)
  {
    CIAO_TRACE ("Session_Container::add_servant_to_map");
  }

  void
  Session_Container::deactivate_facet (const PortableServer::ObjectId &)
  {
    CIAO_TRACE ("Session_Container::deactivate_facet");
  }

  void
  Session_Container::delete_servant_from_map (PortableServer::ObjectId &)
  {
    CIAO_TRACE ("Session_Container::delete_servant_from_map");
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

    if (t == Container_Types::COMPONENT_t)
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
