// $Id$

#include "Session_Container.h"

#include "tao/Utils/PolicyList_Destroyer.h"
#include "ciao/Containers/Servant_Activator.h"
#include "ccm/ComponentServer/CCM_ComponentServer_BaseC.h"
#include "ciao/ComponentServer/CIAO_ComponentServerC.h"
#include "ciao/Servants/Servant_Impl_Base.h"
#include "ciao/Logger/Log_Macros.h"

#if !defined (__ACE_INLINE__)
# include "Session_Container.inl"
#endif /* __ACE_INLINE__ */

namespace CIAO
{
  ///////////////////////////////////////////////////////////////

  ACE_Atomic_Op <TAO_SYNCH_MUTEX, unsigned long>
  Session_Container::serial_number_ (0);

  Session_Container::Session_Container (CORBA::ORB_ptr o,
                                        PortableServer::POA_ptr poa,
                                        Deployment::CIAO_Container_i *container_impl,
                                        bool static_config_flag,
                                        const Static_Config_EntryPoints_Maps* maps,
                                        const char *name,
                                        const CORBA::PolicyList *more_policies)
    : Container_i (o, poa, container_impl),
      static_config_flag_ (static_config_flag),
      static_entrypts_maps_ (maps),
      sa_ (0)
  {
    this->init (name, more_policies);
  }

  Session_Container::~Session_Container (void)
  {
    // delete this->sa_;
  }

  void
  Session_Container::init (const char *name,
                           const CORBA::PolicyList *more_policies)
  {
    CIAO_TRACE ("Session_Container::init");

    char buffer[MAXPATHLEN];

    if (!name)
      {
        unsigned long number = ++Session_Container::serial_number_;
        CIAO_DEBUG ((LM_TRACE, CLINFO "Session_Container::init - "
                     "Constructing container name from serial number %u\n",
                     number));
        if (ACE_OS::sprintf (buffer,
                             "CIAO::Session_Container-%ld",
                             number) < 0)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO
                        "CIAO::Session_Container: Unable to create name from number.\n"));
            throw Components::CreateFailure ();
          }

        name = buffer;
      }

    CIAO_DEBUG ((LM_TRACE, CLINFO "Session_Container::init - "
                 "Initializing a container with name %C\n",
                 name));

    if (CORBA::is_nil (this->root_poa_.in ()))
      {
        CIAO_ERROR ((LM_ERROR, CLINFO
                    "CIAO::Session_Container: Unable to initialize the POA.\n"));
        throw Components::CreateFailure ();
      }

    this->create_component_POA (name,
                                more_policies,
                                this->root_poa_.in ());

    ACE_CString port_poa_name (name);
    port_poa_name += ":Port_POA";
    this->create_facet_consumer_POA (port_poa_name.c_str (),
                                     more_policies,
                                     this->root_poa_.in ());

    PortableServer::POAManager_var poa_manager =
      this->root_poa_->the_POAManager ();

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
      root->create_POA (name, poa_manager.in (), policies);
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

    Servant_Activator_i *sa = 0;
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

    PortableServer::POA_ptr tmp = PortableServer::POA::_nil();

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

    if (!this->static_config_flag_)
      {
        CIAO_DEBUG ((LM_DEBUG, CLINFO
                    "Session_Container::install_home - "
                    "Loading home [%C] from shared libraries\n",
                    name));
        CIAO_DEBUG ((LM_DEBUG, CLINFO
                    "Session_Container::install_home - "
                    "Executor library [%C] with entrypoint [%C]\n",
                    primary_artifact, entry_point));
        CIAO_DEBUG ((LM_DEBUG, CLINFO
                    "Session_Container::install_home - "
                    "Servant library [%C] with entrypoint [%C]\n",
                    servant_artifact, servant_entrypoint));

        if (!primary_artifact)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO
                    "Session_Container::install_home - "
                    "ERROR: Null component executor DLL name\n"));
            throw Components::Deployment::UnknownImplId ();
          }

        if (!servant_artifact)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO
                    "Session_Container::install_home - "
                    "ERROR: Null component servant DLL name\n"));
            throw Components::Deployment::UnknownImplId ();
          }

        if (!entry_point)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO
                        "Session_Container::install_home - "
                        "ERROR: Null entry point for executor DLL [%C]\n",
                        primary_artifact));
            throw Components::Deployment::ImplEntryPointNotFound ();
          }

        if (!servant_entrypoint)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO
                        "Session_Container::install_home - "
                        "ERROR: Null entry point for servant DLL [%C]\n",
                        servant_artifact));
            throw Components::Deployment::ImplEntryPointNotFound ();
          }

        ACE_DLL executor_dll;
        if (executor_dll.open (ACE_TEXT_CHAR_TO_TCHAR (primary_artifact),
                               ACE_DEFAULT_SHLIB_MODE,
                               0) != 0)
          {
            const ACE_TCHAR* error = executor_dll.error ();
            CIAO_ERROR ((LM_ERROR, CLINFO
                        "Session_Container::install_home - "
                        "ERROR in opening the executor DLL [%C] with error [%s]\n",
                        primary_artifact, error));

            throw Components::Deployment::UnknownImplId ();
          }
        else
          {
            CIAO_DEBUG ((LM_TRACE, CLINFO
                        "Session_Container::install_home - "
                        "Executor DLL successfully opened\n"));
          }

        ACE_DLL servant_dll;
        if (servant_dll.open (ACE_TEXT_CHAR_TO_TCHAR (servant_artifact),
                              ACE_DEFAULT_SHLIB_MODE,
                              0) != 0)
          {
            const ACE_TCHAR* error = servant_dll.error ();
            CIAO_ERROR ((LM_ERROR, CLINFO
                        "Session_Container::install_home - "
                        "ERROR in opening the servant DLL [%C] with error [%s]\n",
                        servant_artifact, error));

            throw Components::Deployment::UnknownImplId ();
          }
        else
          {
            CIAO_DEBUG ((LM_TRACE, CLINFO
                        "Session_Container::install_home - "
                        "Servant DLL successfully openend.\n"));
          }

        // We have to do this casting in two steps because the C++
        // standard forbids casting a pointer-to-object (including
        // void*) directly to a pointer-to-function.
        void *void_ptr = executor_dll.symbol (ACE_TEXT_CHAR_TO_TCHAR (entry_point));
        ptrdiff_t tmp_ptr = reinterpret_cast<ptrdiff_t> (void_ptr);
        hcreator = reinterpret_cast<HomeFactory> (tmp_ptr);

        void_ptr = servant_dll.symbol (ACE_TEXT_CHAR_TO_TCHAR (servant_entrypoint));
        tmp_ptr = reinterpret_cast<ptrdiff_t> (void_ptr);
        screator = reinterpret_cast<HomeServantFactory> (tmp_ptr);
      }
    else
      {
        CIAO_DEBUG ((LM_DEBUG, CLINFO
                    "Session_Container::install_home - "
                    "Loading statically linked home [%C]\n",
                    name));

        if (static_entrypts_maps_ == 0
            || static_entrypts_maps_->home_creator_funcptr_map_ == 0
            || static_entrypts_maps_->home_servant_creator_funcptr_map_ == 0)
          {
            CIAO_DEBUG ((LM_ERROR, CLINFO
                        "Session_Container::install_home - ERROR: Static entrypoint "
                        "maps are null or imcomplete.\n"));
            throw Components::Deployment::ImplEntryPointNotFound ();
          }

        ACE_CString entry_point_str (entry_point);
        static_entrypts_maps_->home_creator_funcptr_map_->find (entry_point_str,
                                                                hcreator);

        ACE_CString servant_entrypoint_str (servant_entrypoint);
        static_entrypts_maps_->home_servant_creator_funcptr_map_->find (
          servant_entrypoint_str, screator);
      }

    if (!hcreator)
      {
        CIAO_ERROR ((LM_ERROR, CLINFO
                     "Session_Container::install_home - Error: Entry point "
                     "invalid in dll [%C]\n",
                     primary_artifact));
        throw Components::Deployment::ImplEntryPointNotFound ();
      }

    if (!screator)
      {
        CIAO_ERROR ((LM_ERROR, CLINFO
                     "Session_Container::install_home - Error: Entry point "
                     "invalid in dll [%C]\n",
                     servant_artifact));
        throw Components::Deployment::ImplEntryPointNotFound ();
      }

    CIAO_DEBUG ((LM_TRACE, CLINFO
                "Session_Container::install_home - Loading home executor\n"));
    Components::HomeExecutorBase_var home_executor = hcreator ();

    if (CORBA::is_nil (home_executor.in ()))
      {
        CIAO_ERROR ((LM_ERROR, CLINFO
                    "Session_Container::install_home - "
                    "Home executor factory failed.\n"));
        throw Components::Deployment::InstallationFailure ();
      }

    CIAO_DEBUG ((LM_TRACE, CLINFO
                "Session_Container::install_home - Loading home servant\n"));
    PortableServer::Servant home_servant = screator (home_executor.in (),
                                                     this,
                                                     name);

    if (!home_servant)
      {
            CIAO_ERROR ((LM_ERROR, CLINFO
                    "Session_Container::install_home - "
                    "Home servant factory failed.\n"));
        throw Components::Deployment::InstallationFailure ();
      }

    PortableServer::ServantBase_var safe (home_servant);

    CIAO_DEBUG ((LM_TRACE, CLINFO
                "Session_Container::install_home - Installing home servant\n"));

    PortableServer::ObjectId_var oid;

    CORBA::Object_var objref =
      this->install_servant (home_servant, Container_Types::HOME_t, oid.out ());

    Components::CCMHome_var homeref =
      Components::CCMHome::_narrow (objref.in ());

    CIAO_DEBUG ((LM_TRACE, CLINFO
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

    if (!this->static_config_flag_)
      {
        CIAO_DEBUG ((LM_DEBUG, CLINFO
                    "Session_Container::install_component - "
                    "Loading component [%C] from shared libraries\n",
                    name));
        CIAO_DEBUG ((LM_DEBUG, CLINFO
                    "Session_Container::install_component - "
                    "Executor library [%C] with entrypoint [%C]\n",
                    primary_artifact, entry_point));
        CIAO_DEBUG ((LM_DEBUG, CLINFO
                    "Session_Container::install_component - "
                    "Servant library [%C] with entrypoint [%C]\n",
                    servant_artifact, servant_entrypoint));

        if (!primary_artifact)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO
                    "Session_Container::install_component - "
                    "ERROR: Null component executor DLL name\n"));
            throw Components::Deployment::UnknownImplId ();
          }

        if (!servant_artifact)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO
                    "Session_Container::install_component - "
                    "ERROR: Null component servant DLL name\n"));
            throw Components::Deployment::UnknownImplId ();
          }

        if (!entry_point)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO
                        "Session_Container::install_component - "
                        "ERROR: Null entry point for executor DLL [%C]\n",
                        primary_artifact));
            throw Components::Deployment::ImplEntryPointNotFound ();
          }

        if (!servant_entrypoint)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO
                        "Session_Container::install_component - "
                        "ERROR: Null entry point for servant DLL [%C]\n",
                        servant_artifact));
            throw Components::Deployment::ImplEntryPointNotFound ();
          }


        ACE_DLL executor_dll;
        if (executor_dll.open (ACE_TEXT_CHAR_TO_TCHAR (primary_artifact),
                               ACE_DEFAULT_SHLIB_MODE,
                               0) != 0)
          {
            const ACE_TCHAR* error = executor_dll.error ();
            CIAO_ERROR ((LM_ERROR, CLINFO
                        "Session_Container::install_component - "
                        "ERROR in opening the executor DLL [%C] with error [%s]\n",
                        primary_artifact, error));

            throw Components::Deployment::UnknownImplId ();
          }
        else
          {
            CIAO_DEBUG ((LM_TRACE, CLINFO
                        "Session_Container::install_component - Executor DLL successfully opened\n"));
          }

        ACE_DLL servant_dll;
        if (servant_dll.open (ACE_TEXT_CHAR_TO_TCHAR (servant_artifact),
                              ACE_DEFAULT_SHLIB_MODE,
                              0) != 0)
          {
            const ACE_TCHAR* error = servant_dll.error ();
            CIAO_ERROR ((LM_ERROR, CLINFO
                        "Session_Container::install_component - "
                        "ERROR in opening the servant DLL [%C] with error [%s]\n",
                        servant_artifact, error));

            throw Components::Deployment::UnknownImplId ();
          }
        else
          {
            CIAO_DEBUG ((LM_TRACE, CLINFO
                        "Session_Container::install_component - Servant DLL successfully openend.\n"));
          }

        // We have to do this casting in two steps because the C++
        // standard forbids casting a pointer-to-object (including
        // void*) directly to a pointer-to-function.
        void *void_ptr = executor_dll.symbol (ACE_TEXT_CHAR_TO_TCHAR (entry_point));
        ptrdiff_t tmp_ptr = reinterpret_cast<ptrdiff_t> (void_ptr);
        ccreator = reinterpret_cast<ComponentFactory> (tmp_ptr);

        void_ptr = servant_dll.symbol (ACE_TEXT_CHAR_TO_TCHAR (servant_entrypoint));
        tmp_ptr = reinterpret_cast<ptrdiff_t> (void_ptr);
        screator = reinterpret_cast<ComponentServantFactory> (tmp_ptr);
      }
    else
      {
        CIAO_DEBUG ((LM_DEBUG, CLINFO
                    "Session_Container::install_component - "
                    "Loading statically linked component [%C]\n",
                    name));

        if (static_entrypts_maps_ == 0
            || static_entrypts_maps_->component_creator_funcptr_map_ == 0
            || static_entrypts_maps_->component_servant_creator_funcptr_map_ == 0)
          {
            CIAO_DEBUG ((LM_ERROR, CLINFO
                        "Session_Container::install_component - "
                        "ERROR: Static entrypoint "
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

    if (!ccreator)
      {
        CIAO_ERROR ((LM_ERROR, CLINFO
                     "Session_Container::install_home - Error: Entry point "
                     "invalid in dll [%C]\n",
                     primary_artifact));
        throw Components::Deployment::ImplEntryPointNotFound ();
      }

    if (!screator)
      {
        CIAO_ERROR ((LM_ERROR, CLINFO
                     "Session_Container::install_home - Error: Entry point "
                     "invalid in dll [%C]\n",
                     servant_artifact));
        throw Components::Deployment::ImplEntryPointNotFound ();
      }

    CIAO_DEBUG ((LM_TRACE, CLINFO
                "Session_Container::install_component - "
                "Loading component executor\n"));

    Components::EnterpriseComponent_var component_executor = ccreator ();

    if (CORBA::is_nil (component_executor.in ()))
      {
        CIAO_ERROR ((LM_ERROR, CLINFO
                    "Session_Container::install_component - "
                    "Component executor factory failed.\n"));
        throw Components::Deployment::InstallationFailure ();
      }

    CIAO_DEBUG ((LM_TRACE, CLINFO
                "Session_Container::install_component - "
                "Loading component servant\n"));
    PortableServer::Servant component_servant = screator (component_executor.in (),
                                                          this,
                                                          name);

    if (!component_servant)
      {
      CIAO_ERROR ((LM_ERROR, CLINFO
                    "Session_Container::install_component - "
                    "Component servant factory failed.\n"));
        throw Components::Deployment::InstallationFailure ();
      }

    PortableServer::ServantBase_var safe (component_servant);

    CIAO_DEBUG ((LM_TRACE, CLINFO
                "Session_Container::install_component - "
                "Installing component servant\n"));

    PortableServer::ObjectId_var oid;

    CORBA::Object_var objref =
      this->install_servant (component_servant, Container_Types::COMPONENT_t, oid.out ());

    Components::CCMObject_var componentref =
      Components::CCMObject::_narrow (objref.in ());

    CIAO_DEBUG ((LM_TRACE, CLINFO
                "Session_Container::install_component - "
                "Component successfully created\n"));

    return componentref._retn ();
  }

  void
  Session_Container::connect_local_facet (::Components::CCMObject_ptr provider,
                                          const char * provider_port,
                                          ::Components::CCMObject_ptr user,
                                          const char * user_port)
  {
    CIAO_TRACE ("Session_Container::connect_local_facet");

    if (!provider_port || !user_port)
      {
        CIAO_ERROR ((LM_ERROR, CLINFO "Session_Container::connect_local_facet - "
                     "Nil port name provided to connect local facet, throwing exception\n"));
        throw ::Components::InvalidConnection ();
      }

    try
      {
        PortableServer::Servant srv_tmp =
          this->component_poa_->reference_to_servant (provider);

        CIAO_DEBUG ((LM_TRACE, CLINFO "Session_Container::connect_local_facet - "
                     "Successfully fetched provider servant from POA\n"));

        CIAO::Servant_Impl_Base *prov_serv =
          dynamic_cast<CIAO::Servant_Impl_Base *> (srv_tmp);

        if (!prov_serv)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "Session_Container::connect_local_facet - "
                         "Unable to cast to provider servant implementation\n"));
            throw ::Components::InvalidConnection ();
          }

        srv_tmp = this->component_poa_->reference_to_servant (user);
        CIAO_DEBUG ((LM_TRACE, CLINFO "Session_Container::connect_local_facet - "
                     "Successfully fetched user servant from POA\n"));

        CIAO::Servant_Impl_Base *user_serv =
          dynamic_cast<CIAO::Servant_Impl_Base *> (srv_tmp);

        if (!user_serv)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "Session_Container::connect_local_facet - "
                         "Unable to cast to provider servant implementation\n"));
            throw ::Components::InvalidConnection ();
          }

        ::CORBA::Object_ptr exec = prov_serv->get_facet_executor (provider_port);

        // Note:  Spec says that facet executor provided by component MAY BE NIL
        if (!::CORBA::is_nil (exec))
          {
            user_serv->connect (user_port, exec);
          }
        else
          {
            CIAO_DEBUG ((LM_DEBUG, CLINFO "Session_Container::connect_local_facet - "
                         "Got nil facet executor for [%C]\n", provider_port));
          }
      }
    catch (const ::Components::InvalidConnection &)
      {
        throw;
      }
    catch (const ::CORBA::Exception &ex)
      {
        CIAO_ERROR ((LM_ERROR, CLINFO "Session_Container::connect_local_facet - "
                     "Caught exception %C.\n",
                     ex._info ().c_str ()));
        throw;
      }
    catch (...)
      {
        CIAO_ERROR ((LM_ERROR, CLINFO "Session_Container::connect_local_facet - "
                     "Attempting to connect components not managed by this container.\n"));
        throw ::Components::InvalidConnection ();
      }
  }

  void
  Session_Container::disconnect_local_facet (::Components::CCMObject_ptr provider,
                                             const char * provider_port,
                                             ::Components::CCMObject_ptr user,
                                             const char * user_port)
  {
    CIAO_TRACE ("Session_Container::disconnect_local_facet");

    try
      {
        PortableServer::Servant srv_tmp = this->component_poa_->reference_to_servant (provider);
        CIAO_DEBUG ((LM_TRACE, CLINFO "Session_Container::disconnect_local_facet - "
                     "Successfully fetched provider servant from POA\n"));

        CIAO::Servant_Impl_Base *prov_serv =
          dynamic_cast<CIAO::Servant_Impl_Base *> (srv_tmp);

        if (!prov_serv)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "Session_Container::disconnect_local_facet - "
                         "Unable to cast to provider servant implementation\n"));
            throw ::Components::InvalidConnection ();
          }

        srv_tmp = this->component_poa_->reference_to_servant (user);
        CIAO_DEBUG ((LM_TRACE, CLINFO "Session_Container::disconnect_local_facet - "
                     "Successfully fetched user servant from POA\n"));

        CIAO::Servant_Impl_Base *user_serv =
          dynamic_cast<CIAO::Servant_Impl_Base *> (srv_tmp);

        if (!user_serv)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO "Session_Container::disconnect_local_facet - "
                         "Unable to cast to provider servant implementation\n"));
            throw ::Components::InvalidConnection ();
          }

        ::CORBA::Object_ptr exec = prov_serv->get_facet_executor (provider_port);

        // Note:  Spec says that facet executor provided by component MAY BE NIL
        if (!::CORBA::is_nil (exec))
          {
            user_serv->disconnect (user_port, 0);
          }
        else
          {
            throw ::Components::InvalidConnection ();
          }
      }
    catch (const ::Components::InvalidConnection &)
      {
        throw;
      }
    catch (const ::CORBA::Exception &ex)
      {
        CIAO_ERROR ((LM_ERROR, CLINFO "Session_Container::disconnect_local_facet - "
                     "Caught exception %C.\n",
                     ex._info ().c_str ()));
        throw;
      }
    catch (...)
      {
        CIAO_ERROR ((LM_ERROR, CLINFO "Session_Container::disconnect_local_facet - "
                     "Attempting to connect components not managed by this container.\n"));
        throw ::Components::InvalidConnection ();
      }

  }

  void
  Session_Container::activate_component (Components::CCMObject_ptr compref)
  {
    CIAO_TRACE("Session_Container::activate_component");

    try
      {

        CIAO::Servant_Impl_Base * svt = 0;

        try
          {
            svt = dynamic_cast<CIAO::Servant_Impl_Base *> (
                    this->component_poa_->reference_to_servant (compref));
          }
        catch (...)
          {
            throw InvalidComponent ();
          }

        if (!svt)
          {
            throw CIAO::InvalidComponent  ();
          }
        else
          {
            CIAO_DEBUG ((LM_TRACE, CLINFO "Session_Container::activate_component - "
                       "Invoking CCM activate on provided component object reference.\n"));
            svt->activate_component ();
          }
      }
    catch (const CIAO::InvalidComponent &)
      {
        CIAO_ERROR ((LM_ERROR, CLINFO "Session_Container::activate_component - "
                     "Failed to retrieve servant and/or cast to servant pointer.\n"));
        throw;
      }
    catch (const CORBA::Exception &ex)
      {
        CIAO_ERROR ((LM_ERROR, CLINFO "Session_Container::activate_component - "
                     "Caught CORBA exception while activating a component: %C\n",
                     ex._info ().c_str ()));
        throw;
      }
    catch (...)
      {
        CIAO_ERROR ((LM_ERROR, CLINFO "Session_Container::activate_component - "
                     "Caught unknown C++ eception while activating a component.\n"));
        throw;
      }
  }

  void
  Session_Container::passivate_component (Components::CCMObject_ptr compref)
  {
    CIAO_TRACE ("Session_Container::passivate_component");

    try
      {
        CIAO::Servant_Impl_Base * svt = 0;

        try
          {
            svt = dynamic_cast<CIAO::Servant_Impl_Base *>
                    (this->component_poa_->reference_to_servant (compref));
          }
        catch (...)
          {
            throw InvalidComponent ();
          }

        if (!svt)
          {
            throw CIAO::InvalidComponent  ();
          }
        else
          {
            CIAO_DEBUG ((LM_TRACE, CLINFO "Session_Container::passivate_component - "
                         "Invoking CCM passivate on provided component object reference.\n"));
            svt->passivate_component ();
          }
      }
    catch (const CORBA::Exception &ex)
      {
        CIAO_ERROR ((LM_ERROR, CLINFO "Session_Container::passivate_component - "
                     "Caught CORBA exception while passivating a component: %C\n",
                     ex._info ().c_str ()));
        throw;
      }
    catch (...)
      {
        CIAO_ERROR ((LM_ERROR, CLINFO "Session_Container::passivate_component - "
                     "Caught unknown C++ eception while passivating a component.\n"));
        throw;
      }
  }

  void
  Session_Container::uninstall (CORBA::Object_ptr objref, Container_Types::OA_Type y)
  {
    CIAO_TRACE ("Session_Container::uninstall");

    PortableServer::Servant svnt;

    switch (y)
      {
      case Container_Types::COMPONENT_t:
      case Container_Types::HOME_t:
        svnt = this->component_poa_->reference_to_servant (objref);
        break;
      default:
        svnt = this->facet_cons_poa_->reference_to_servant (objref);
        break;
      }

    PortableServer::ObjectId_var oid;
    this->uninstall_servant (svnt, y, oid.out ());
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

    CIAO::Servant_Impl_Base * svnt = dynamic_cast <CIAO::Servant_Impl_Base *> (
      this->component_poa_->reference_to_servant (homeref));

    if (!svnt)
      {
        CIAO_ERROR ((LM_ERROR, CLINFO "Session_Container::uninstall_component - "
                     "Unable to convert provided servant reference to servant implementation."));
        throw ::Components::RemoveFailure ();
      }
    else
      {
        svnt->remove ();
      }
  }

  void
  Session_Container::uninstall_servant (PortableServer::Servant svnt,
                                        Container_Types::OA_Type t,
                                        PortableServer::ObjectId_out oid)
  {
    CIAO_TRACE ("Session_Container::uninstall_servant");

    PortableServer::POA_ptr tmp = PortableServer::POA::_nil();

    if ((t == Container_Types::COMPONENT_t) ||
        (t == Container_Types::HOME_t))
      {
        CIAO_DEBUG ((LM_TRACE, CLINFO "Session_Container::uninstall_servant - "
                     "Removing component or home servant\n"));
        tmp = this->component_poa_.in ();
      }
    else
      {
        CIAO_DEBUG ((LM_TRACE, CLINFO "Session_Container::uninstall_servant - "
                     "Removing facet or consumer servant\n"));
        tmp = this->facet_cons_poa_.in ();
      }

    try
      {
        PortableServer::ObjectId_var tmp_id;
        tmp_id = tmp->servant_to_id (svnt);
        tmp->deactivate_object (tmp_id);

        CIAO_DEBUG ((LM_TRACE, CLINFO "Session_Container::uninstall_servant - "
                     "Servant successfully removed, reference count is %u\n",
                     svnt->_refcount_value () - 1));

        svnt->_remove_ref ();

        oid = tmp_id._retn ();
      }
    catch (const CORBA::Exception &ex)
      {
        CIAO_ERROR ((LM_ERROR, CLINFO "Session_Container::uninstall_servant - "
                     "Caught CORBA exception while uninstalling servant: %C\n",
                     ex._info ().c_str ()));
        throw Components::RemoveFailure ();
      }

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

    PortableServer::POA_ptr tmp = PortableServer::POA::_nil();

    if (t == Container_Types::COMPONENT_t || t == Container_Types::HOME_t)
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
