// $Id$

#include "Extension_Container.h"

#include "ciao/Servants/Servant_Impl_Base.h"
#include "ciao/Logger/Log_Macros.h"

#include "ciao/Containers/Extension/ClientContainerInterceptorRegistration_Impl.h"
#include "ciao/Containers/Extension/ServantContainerInterceptorRegistration_Impl.h"
#include "ciao/Containers/Extension/ServerContainerInterceptorRegistration_Impl.h"
#include "ciao/Containers/Extension/StubContainerInterceptorRegistration_Impl.h"

#if !defined (__ACE_INLINE__)
# include "Extension_Container.inl"
#endif /* __ACE_INLINE__ */

namespace CIAO
{
  ///////////////////////////////////////////////////////////////

  Extension_Container_i::Extension_Container_i (
        CORBA::ORB_ptr o,
        PortableServer::POA_ptr poa)
    : Container_i < ::CIAO::Extension_Container> (o, poa),
      client_copi_registration_ (0),
      servant_copi_registration_ (0),
      server_copi_registration_ (0),
      stub_copi_registration_ (0)
  {
  }

  Extension_Container_i::~Extension_Container_i (void)
  {
  }

  void
  Extension_Container_i::init (const char *name)
  {
    CIAO_TRACE ("Extension_Container_i::init");

    // Create the Portable Interceptor Registration objects
    ACE_NEW_THROW_EX (this->client_copi_registration_,
                      ClientContainerInterceptorRegistration_Impl,
                      CORBA::NO_MEMORY ());
    ACE_NEW_THROW_EX (this->servant_copi_registration_,
                      ServantContainerInterceptorRegistration_Impl,
                      CORBA::NO_MEMORY ());
    ACE_NEW_THROW_EX (this->server_copi_registration_,
                      ServerContainerInterceptorRegistration_Impl,
                      CORBA::NO_MEMORY ());
    ACE_NEW_THROW_EX (this->stub_copi_registration_,
                      StubContainerInterceptorRegistration_Impl,
                      CORBA::NO_MEMORY ());

    Container_i < ::CIAO::Extension_Container>::init (name);
  }

  void
  Extension_Container_i::fini (void)
  {
    CIAO_TRACE ("Extension_Container_i::fini");

    ::CORBA::release (this->client_copi_registration_);
    ::CORBA::release (this->servant_copi_registration_);
    ::CORBA::release (this->server_copi_registration_);
    ::CORBA::release (this->stub_copi_registration_);

    Container_i < ::CIAO::Extension_Container>::fini ();
  }

  CORBA::Object_ptr
  Extension_Container_i::install_servant (PortableServer::Servant p,
                                          Container_Types::OA_Type t,
                                          PortableServer::ObjectId_out oid)
  {
    CIAO_TRACE ("Extension_Container_i::install_servant");

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
  Extension_Container_i::install_home (const char *primary_artifact,
                                     const char *entry_point,
                                     const char *servant_artifact,
                                     const char *servant_entrypoint,
                                     const char *name)
  {
    CIAO_TRACE ("Extension_Container_i::install_home");

    HomeFactory hcreator = 0;
    HomeServantFactory screator = 0;

    CIAO_DEBUG (6,
                (LM_DEBUG,
                  CLINFO
                  "Extension_Container_i::install_home - "
                  "Loading home [%C] from shared libraries\n",
                  name));

    CIAO_DEBUG (6,
                (LM_DEBUG,
                  CLINFO
                  "Extension_Container_i::install_home - "
                  "Executor library [%C] with entrypoint [%C]\n",
                  primary_artifact,
                  entry_point));

    CIAO_DEBUG (6,
                (LM_DEBUG,
                  CLINFO
                  "Extension_Container_i::install_home - "
                  "Servant library [%C] with entrypoint [%C]\n",
                  servant_artifact,
                  servant_entrypoint));

    if (!primary_artifact)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                      CLINFO
                      "Extension_Container_i::install_home - "
                      "ERROR: Null component executor DLL name\n"));

        throw Components::Deployment::UnknownImplId ();
      }

    if (!servant_artifact)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                      CLINFO
                      "Extension_Container_i::install_home - "
                      "ERROR: Null component servant DLL name\n"));

        throw Components::Deployment::UnknownImplId ();
      }

    if (!entry_point)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                      CLINFO
                      "Extension_Container_i::install_home - "
                      "ERROR: Null entry point for "
                      "executor DLL [%C]\n",
                      primary_artifact));

        throw Components::Deployment::ImplEntryPointNotFound ();
      }

    if (!servant_entrypoint)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                      CLINFO
                      "Extension_Container_i::install_home - "
                      "ERROR: Null entry point for "
                      "servant DLL [%C]\n",
                      servant_artifact));

        throw Components::Deployment::ImplEntryPointNotFound ();
      }

    ACE_DLL executor_dll;
    if (executor_dll.open (ACE_TEXT_CHAR_TO_TCHAR (primary_artifact),
                            ACE_DEFAULT_SHLIB_MODE,
                            false) != 0)
      {
        const ACE_TCHAR* error = executor_dll.error ();

        CIAO_ERROR (1,
                    (LM_ERROR,
                      CLINFO
                      "Extension_Container_i::install_home - "
                      "ERROR in opening the executor "
                      "DLL [%C] with error [%s]\n",
                      primary_artifact,
                      error));

        throw Components::Deployment::UnknownImplId ();
      }
    else
      {
        CIAO_DEBUG (9,
                    (LM_TRACE,
                     CLINFO
                    "Extension_Container_i::install_home - "
                    "Executor DLL [%C] successfully opened\n",
                    primary_artifact));
      }

    ACE_DLL servant_dll;

    if (servant_dll.open (ACE_TEXT_CHAR_TO_TCHAR (servant_artifact),
                          ACE_DEFAULT_SHLIB_MODE,
                          false) != 0)
      {
        const ACE_TCHAR* error = servant_dll.error ();

        CIAO_ERROR (1,
                    (LM_ERROR,
                      CLINFO
                      "Extension_Container_i::install_home - "
                      "ERROR in opening the servant "
                      "DLL [%C] with error [%s]\n",
                      servant_artifact,
                      error));

        throw Components::Deployment::UnknownImplId ();
      }
    else
      {
        CIAO_DEBUG (9,
                    (LM_TRACE,
                     CLINFO
                     "Extension_Container_i::install_home - "
                     "Servant DLL [%C] successfully openend\n",
                     servant_artifact));
      }

    // We have to do this casting in two steps because the C++
    // standard forbids casting a pointer-to-object (including
    // void*) directly to a pointer-to-function.
    void *void_ptr =
      executor_dll.symbol (ACE_TEXT_CHAR_TO_TCHAR (entry_point));
    ptrdiff_t tmp_ptr = reinterpret_cast<ptrdiff_t> (void_ptr);
    hcreator = reinterpret_cast<HomeFactory> (tmp_ptr);

    void_ptr =
      servant_dll.symbol (ACE_TEXT_CHAR_TO_TCHAR (servant_entrypoint));
    tmp_ptr = reinterpret_cast<ptrdiff_t> (void_ptr);
    screator = reinterpret_cast<HomeServantFactory> (tmp_ptr);

    if (!hcreator)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Extension_Container_i::install_home "
                     "- Error: Entry point [%C] "
                     "invalid in dll [%C]\n",
                     entry_point,
                     primary_artifact));

        throw Components::Deployment::ImplEntryPointNotFound ();
      }

    if (!screator)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Extension_Container_i::install_home"
                     " - Error: Entry point [%C] "
                     "invalid in dll [%C]\n",
                     servant_entrypoint,
                     servant_artifact));

        throw Components::Deployment::ImplEntryPointNotFound ();
      }

    CIAO_DEBUG (9,
                (LM_TRACE,
                 CLINFO
                 "Extension_Container_i::install_home"
                 " - Loading home executor\n"));

    Components::HomeExecutorBase_var home_executor =
      hcreator ();

    if (CORBA::is_nil (home_executor.in ()))
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Extension_Container_i::install_home - "
                     "Home executor factory failed.\n"));

        throw Components::Deployment::InstallationFailure ();
      }

    CIAO_DEBUG (9,
                (LM_TRACE,
                 CLINFO
                 "Extension_Container_i::install_home"
                 " - Loading home servant\n"));

    PortableServer::Servant home_servant =
      screator (home_executor.in (),
                this,
                name);

    if (home_servant == 0)
      {
            CIAO_ERROR (1,
                        (LM_ERROR,
                         CLINFO
                         "Extension_Container_i::install_home - "
                         "Home servant factory failed.\n"));

        throw Components::Deployment::InstallationFailure ();
      }

    PortableServer::ServantBase_var safe (home_servant);

    CIAO_DEBUG (9,
                (LM_TRACE,
                 CLINFO
                 "Extension_Container_i::install_home "
                 "- Installing home servant\n"));

    PortableServer::ObjectId_var oid;

    CORBA::Object_var objref =
      this->install_servant (home_servant,
                             Container_Types::HOME_t,
                             oid.out ());

    Components::CCMHome_var homeref =
      Components::CCMHome::_narrow (objref.in ());

    CIAO_DEBUG (9,
                (LM_TRACE,
                 CLINFO
                 "Extension_Container_i::install_home - "
                 "Home successfully created with name\n"));

    return homeref._retn ();
  }

  Components::CCMObject_ptr
  Extension_Container_i::install_component (const char *primary_artifact,
                                          const char *entry_point,
                                          const char *servant_artifact,
                                          const char *servant_entrypoint,
                                          const char *name)
  {
    CIAO_TRACE ("Extension_Container_i::install_component");

    ComponentFactory ccreator = 0;
    ComponentServantFactory screator = 0;

    CIAO_DEBUG (6,
                (LM_DEBUG,
                  CLINFO
                  "Extension_Container_i::install_component - "
                  "Loading component [%C] from shared libraries\n",
                  name));

    CIAO_DEBUG (6,
                (LM_DEBUG,
                  CLINFO
                  "Extension_Container_i::install_component - "
                  "Executor library [%C] with entrypoint [%C]\n",
                  primary_artifact,
                  entry_point));

    CIAO_DEBUG (6,
                (LM_DEBUG,
                  CLINFO
                  "Extension_Container_i::install_component - "
                  "Servant library [%C] with entrypoint [%C]\n",
                  servant_artifact,
                  servant_entrypoint));

    if (primary_artifact == 0)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                "Extension_Container_i::install_component - "
                "ERROR: Null component executor DLL name\n"));
        throw Components::Deployment::UnknownImplId ();
      }

    if (servant_artifact == 0)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                      CLINFO
                      "Extension_Container_i::install_component - "
                      "ERROR: Null component servant DLL name\n"));

        throw Components::Deployment::UnknownImplId ();
      }

    if (entry_point == 0)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                      CLINFO
                      "Extension_Container_i::install_component - "
                      "ERROR: Null entry point "
                      "for executor DLL [%C]\n",
                      primary_artifact));

        throw Components::Deployment::ImplEntryPointNotFound ();
      }

    if (servant_entrypoint == 0)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                      CLINFO
                      "Extension_Container_i::install_component - "
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

        CIAO_ERROR (1,
                    (LM_ERROR,
                      CLINFO
                      "Extension_Container_i::install_component - "
                      "ERROR in opening the executor "
                      "DLL [%C] with error [%s]\n",
                      primary_artifact,
                      error));

        throw Components::Deployment::UnknownImplId ();
      }
    else
      {
        CIAO_DEBUG (9,
                    (LM_TRACE,
                     CLINFO
                     "Extension_Container_i::install_component"
                     " - Executor [%C] DLL successfully opened\n",
                     primary_artifact));
      }

    ACE_DLL servant_dll;

    if (servant_dll.open (ACE_TEXT_CHAR_TO_TCHAR (servant_artifact),
                          ACE_DEFAULT_SHLIB_MODE,
                          0) != 0)
      {
        const ACE_TCHAR* error = servant_dll.error ();

        CIAO_ERROR (1,
                    (LM_ERROR,
                      CLINFO
                      "Extension_Container_i::install_component - "
                      "ERROR in opening the servant DLL"
                      " [%C] with error [%s]\n",
                      servant_artifact,
                      error));

        throw Components::Deployment::UnknownImplId ();
      }
    else
      {
        CIAO_DEBUG (9,
                    (LM_TRACE,
                     CLINFO
                     "Extension_Container_i::install_component "
                     "- Servant DLL [%C] successfully openend\n",
                     servant_artifact));
      }

    // We have to do this casting in two steps because the C++
    // standard forbids casting a pointer-to-object (including
    // void*) directly to a pointer-to-function.
    void *void_ptr =
      executor_dll.symbol (ACE_TEXT_CHAR_TO_TCHAR (entry_point));
    ptrdiff_t tmp_ptr = reinterpret_cast<ptrdiff_t> (void_ptr);
    ccreator = reinterpret_cast<ComponentFactory> (tmp_ptr);

    void_ptr =
      servant_dll.symbol (ACE_TEXT_CHAR_TO_TCHAR (servant_entrypoint));
    tmp_ptr = reinterpret_cast<ptrdiff_t> (void_ptr);
    screator = reinterpret_cast<ComponentServantFactory> (tmp_ptr);

    if (ccreator == 0)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Extension_Container_i::install_home "
                     "- Error: Entry point [%C] "
                     "invalid in dll [%C]\n",
                     entry_point,
                     primary_artifact));

        throw Components::Deployment::ImplEntryPointNotFound ();
      }

    if (screator == 0)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Extension_Container_i::install_home "
                     "- Error: Entry point [%C] "
                     "invalid in dll [%C]\n",
                     servant_entrypoint,
                     servant_artifact));

        throw Components::Deployment::ImplEntryPointNotFound ();
      }

    CIAO_DEBUG (9,
                (LM_TRACE,
                 CLINFO
                 "Extension_Container_i::install_component - "
                 "Loading component executor\n"));

    Components::EnterpriseComponent_var component_executor =
      ccreator ();

    if (CORBA::is_nil (component_executor.in ()))
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Extension_Container_i::install_component - "
                     "Component executor factory failed.\n"));

        throw Components::Deployment::InstallationFailure ();
      }

    CIAO_DEBUG (9,
                (LM_TRACE,
                 CLINFO
                 "Extension_Container_i::install_component - "
                 "Loading component servant\n"));

    PortableServer::Servant component_servant =
      screator (component_executor.in (),
                this,
                name);

    if (component_servant == 0)
      {
      CIAO_ERROR (1,
                  (LM_ERROR,
                   CLINFO
                   "Extension_Container_i::install_component - "
                   "Component servant factory failed.\n"));

        throw Components::Deployment::InstallationFailure ();
      }

    PortableServer::ServantBase_var safe (component_servant);

    CIAO_DEBUG (9,
                (LM_TRACE,
                 CLINFO
                 "Extension_Container_i::install_component - "
                 "Installing component servant\n"));

    PortableServer::ObjectId_var oid;

    CORBA::Object_var objref =
      this->install_servant (component_servant,
                             Container_Types::COMPONENT_t,
                             oid.out ());

    Components::CCMObject_var componentref =
      Components::CCMObject::_narrow (objref.in ());

    CIAO_DEBUG (9, (LM_TRACE, CLINFO
                "Extension_Container_i::install_component - "
                "Component successfully created\n"));

    return componentref._retn ();
  }

  void
  Extension_Container_i::connect_local_facet (
    ::Components::CCMObject_ptr provider,
    const char * provider_port,
    ::Components::CCMObject_ptr user,
    const char * user_port)
  {
    CIAO_TRACE ("Extension_Container_i::connect_local_facet");

    if (!provider_port || !user_port)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Extension_Container_i::connect_local_facet - "
                     "Nil port name provided to connect local "
                     "facet, throwing exception\n"));

        throw ::Components::InvalidConnection ();
      }

    try
      {
        PortableServer::ServantBase_var provider_tmp =
          this->component_poa_->reference_to_servant (provider);

        CIAO_DEBUG (9,
                    (LM_TRACE,
                     CLINFO
                     "Extension_Container_i::connect_local_facet - "
                     "Successfully fetched provider servant"
                     " [%C] from POA\n",
                     provider_port));

        CIAO::Connector_Servant_Impl_Base *prov_serv =
          dynamic_cast<CIAO::Connector_Servant_Impl_Base *> (provider_tmp.in ());

        if (!prov_serv)
          {
            CIAO_ERROR (1,
                        (LM_ERROR,
                         CLINFO
                         "Extension_Container_i::connect_local_facet - "
                         "Unable to cast to provider servant "
                         "implementation\n"));
            throw ::Components::InvalidConnection ();
          }

        PortableServer::ServantBase_var user_tmp =
          this->component_poa_->reference_to_servant (user);

        CIAO_DEBUG (9, (LM_TRACE, CLINFO "Extension_Container_i::connect_local_facet - "
                     "Successfully fetched user servant [%C] from POA\n", user_port));

        CIAO::Connector_Servant_Impl_Base *user_serv =
          dynamic_cast<CIAO::Connector_Servant_Impl_Base *> (user_tmp.in ());

        if (user_serv == 0)
          {
            CIAO_ERROR (1,
                        (LM_ERROR,
                         CLINFO
                         "Extension_Container_i::connect_local_facet - "
                         "Unable to cast to user "
                         "servant implementation\n"));

            throw ::Components::InvalidConnection ();
          }

        ::CORBA::Object_var exec =
          prov_serv->get_facet_executor (provider_port);

        // Note: Spec says that facet executor provided by component MAY BE NIL
        if (!::CORBA::is_nil (exec.in ()))
          {
            CIAO_DEBUG (6,
                        (LM_DEBUG,
                         CLINFO
                         "Extension_Container_i::connect_local_facet - "
                         "Create connection between [%C]"
                         " and [%C]\n",
                         user_port,
                         provider_port));

            ::Components::Cookie_var cookie =
              user_serv->connect (user_port, exec.in ());
          }
        else
          {
            CIAO_DEBUG (6,
                        (LM_DEBUG,
                         CLINFO
                         "Extension_Container_i::connect_local_facet - "
                         "Got nil facet executor for [%C]\n",
                         provider_port));
          }
      }
    catch (const ::Components::InvalidConnection &ex)
      {
        throw ex;
      }
    catch (const ::Components::InvalidName &ex)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Extension_Container_i::connect_local_facet - "
                     "Caught exception InvalidName while connecting "
                     "<%C> to <%C>\n",
                     provider_port,
                     user_port));

        throw ex;
      }
    catch (const ::CORBA::Exception &ex)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Extension_Container_i::connect_local_facet - "
                     "Caught exception %C.\n",
                     ex._info ().c_str ()));
        throw;
      }
    catch (...)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Extension_Container_i::connect_local_facet - "
                     "Attempting to connect components not "
                     "managed by this container.\n"));

        throw ::Components::InvalidConnection ();
      }
  }

  void
  Extension_Container_i::disconnect_local_facet (
    ::Components::CCMObject_ptr provider,
    const char * provider_port,
    ::Components::CCMObject_ptr user,
    const char * user_port)
  {
    CIAO_TRACE ("Extension_Container_i::disconnect_local_facet");

    try
      {
        PortableServer::ServantBase_var srv_tmp =
          this->component_poa_->reference_to_servant (provider);

        CIAO_DEBUG (9,
                    (LM_TRACE,
                     CLINFO
                     "Extension_Container_i::disconnect_local_facet - "
                     "Successfully fetched provider servant from POA\n"));

        CIAO::Connector_Servant_Impl_Base *prov_serv =
          dynamic_cast<CIAO::Connector_Servant_Impl_Base *> (srv_tmp.in ());

        if (prov_serv == 0)
          {
            CIAO_ERROR (1,
                        (LM_ERROR,
                         CLINFO
                         "Extension_Container_i::disconnect_local_facet - "
                         "Unable to cast to provider servant "
                         "implementation\n"));

            throw ::Components::InvalidConnection ();
          }

        srv_tmp = this->component_poa_->reference_to_servant (user);

        CIAO_DEBUG (9,
                    (LM_TRACE,
                     CLINFO
                     "Extension_Container_i::disconnect_local_facet - "
                     "Successfully fetched user servant from POA\n"));

        CIAO::Connector_Servant_Impl_Base *user_serv =
          dynamic_cast<CIAO::Connector_Servant_Impl_Base *> (srv_tmp.in ());

        if (user_serv == 0)
          {
            CIAO_ERROR (1,
                        (LM_ERROR,
                         CLINFO
                         "Extension_Container_i::disconnect_local_facet - "
                         "Unable to cast to user servant "
                         "implementation\n"));

            throw ::Components::InvalidConnection ();
          }

        ::CORBA::Object_var exec =
          prov_serv->get_facet_executor (provider_port);

        // Note: Spec says that facet executor provided by component MAY BE NIL
        if (!::CORBA::is_nil (exec.in ()))
          {
            try
              {
                ::CORBA::Object_var port = user_serv->disconnect (user_port, 0);
              }
            catch (const ::Components::CookieRequired&ex)
              {
                CIAO_ERROR (6,
                            (LM_ERROR,
                            CLINFO
                            "Extension_Container_i::disconnect_local_facet - "
                            "Ingnoring exception %C.\n",
                            ex._info ().c_str ()));

                // mcorino@remedy.nl - Ignore this for now; disconnect behaviour will be fixed properly
              }
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
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Extension_Container_i::disconnect_local_facet - "
                     "Caught exception %C.\n",
                     ex._info ().c_str ()));

        throw;
      }
    catch (...)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Extension_Container_i::disconnect_local_facet - "
                     "Attempting to connect components not "
                     "managed by this container.\n"));

        throw ::Components::InvalidConnection ();
      }
  }

  void
  Extension_Container_i::set_attributes (CORBA::Object_ptr compref,
                                     const ::Components::ConfigValues & values)
  {
    CIAO_TRACE("Extension_Container_i::set_attributes");

    try
      {
        PortableServer::ServantBase_var svt;

        try
          {
            svt = this->component_poa_->reference_to_servant (compref);
          }
        catch (CORBA::Exception &ex)
          {
            CIAO_ERROR (1, (LM_ERROR, CLINFO
                            "Extension_Container_i::set_attributes - "
                            "Caught CORBA exception while retrieving servant: %C",
                            ex._info ().c_str ()));
            throw CIAO::InvalidComponent ();
          }
        catch (...)
          {
            CIAO_ERROR (1, (LM_EMERGENCY, "ex in ref to servant\n"));
            throw CIAO::InvalidComponent ();
          }

        if (!svt)
          {
            CIAO_ERROR (1, (LM_EMERGENCY, "invalid servant reference\n"));
            throw CIAO::InvalidComponent  ();
          }
        else
          {
            CIAO::Connector_Servant_Impl_Base * comp = 0;
            CIAO::Home_Servant_Impl_Base *home = 0;

            if ((comp = dynamic_cast <CIAO::Connector_Servant_Impl_Base *> (svt.in ())))
              {
                CIAO_DEBUG (9,
                            (LM_TRACE,
                             CLINFO
                             "Extension_Container_i::set_attributes - "
                             "Configuring attribute values on "
                             "component object reference.\n"));

                comp->set_attributes (values);
              }
            else if ((home = dynamic_cast <CIAO::Home_Servant_Impl_Base *> (svt.in ())))
              {
                CIAO_DEBUG (9,
                            (LM_TRACE,
                             CLINFO
                             "Extension_Container_i::set_attributes - "
                             "Configuring attribute values on "
                             "home object reference.\n"));

                home->set_attributes (values);
              }
            else
              {
                CIAO_ERROR (1, (LM_EMERGENCY, "not home or component\n"));
                throw CIAO::InvalidComponent ();
              }
          }
      }
    catch (const CIAO::InvalidComponent &)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Extension_Container_i::set_attributes - "
                     "Failed to retrieve servant and/or cast "
                     "to servant pointer.\n"));
        throw;
      }
    catch (const CORBA::Exception &ex)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Extension_Container_i::set_attributes - "
                     "Caught CORBA exception while configuring "
                     "component attributes: %C\n",
                     ex._info ().c_str ()));
        throw;
      }
    catch (...)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Extension_Container_i::set_attributes - "
                     "Caught unknown C++ exception while "
                     "configuring component attributes.\n"));

        throw;
      }
  }

  void
  Extension_Container_i::activate_component (
    Components::CCMObject_ptr compref)
  {
    CIAO_TRACE("Extension_Container_i::activate_component");

    try
      {

        CIAO::Connector_Servant_Impl_Base * svt = 0;
        PortableServer::ServantBase_var servant_from_reference;

        try
          {
            servant_from_reference =
              this->component_poa_->reference_to_servant (compref);
            svt =
              dynamic_cast<CIAO::Connector_Servant_Impl_Base *> (
                servant_from_reference.in ());
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
            CIAO_DEBUG (9,
                        (LM_TRACE,
                         CLINFO
                         "Extension_Container_i::activate_component - "
                         "Invoking CCM activate on provided "
                         "component object reference.\n"));

            svt->activate_component ();
          }
      }
    catch (const CIAO::InvalidComponent &)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Extension_Container_i::activate_component - "
                     "Failed to retrieve servant and/or cast "
                     "to servant pointer.\n"));
        throw;
      }
    catch (const CORBA::Exception &ex)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Extension_Container_i::activate_component - "
                     "Caught CORBA exception while activating "
                     "a component: %C\n",
                     ex._info ().c_str ()));
        throw;
      }
    catch (...)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Extension_Container_i::activate_component - "
                     "Caught unknown C++ exception while "
                     "activating a component.\n"));

        throw;
      }
  }

  void
  Extension_Container_i::passivate_component (Components::CCMObject_ptr compref)
  {
    CIAO_TRACE ("Extension_Container_i::passivate_component");

    try
      {
        CIAO::Connector_Servant_Impl_Base * svt = 0;
        PortableServer::ServantBase_var servant_from_reference;

        try
          {
            servant_from_reference =
              this->component_poa_->reference_to_servant (compref);
            svt =
              dynamic_cast<CIAO::Connector_Servant_Impl_Base *> (
                servant_from_reference.in ());
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
            CIAO_DEBUG (9,
                        (LM_TRACE,
                         CLINFO
                         "Extension_Container_i::passivate_component - "
                         "Invoking CCM passivate on provided "
                         "component object reference.\n"));

            svt->passivate_component ();
          }
      }
    catch (const CORBA::Exception &ex)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Extension_Container_i::passivate_component - "
                     "Caught CORBA exception while passivating "
                     "a component: %C\n",
                     ex._info ().c_str ()));

        throw;
      }
    catch (...)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Extension_Container_i::passivate_component - "
                     "Caught unknown C++ exception while "
                     "passivating a component.\n"));

        throw;
      }
  }

  void
  Extension_Container_i::uninstall (CORBA::Object_ptr objref,
                                Container_Types::OA_Type y)
  {
    CIAO_TRACE ("Extension_Container_i::uninstall");

    PortableServer::ServantBase_var svnt;

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
    this->uninstall_servant (svnt.in (), y, oid.out ());
  }

  void
  Extension_Container_i::uninstall_home (Components::CCMHome_ptr homeref)
  {
    CIAO_TRACE ("Extension_Container_i::uninstall_home");

    this->uninstall (homeref, Container_Types::HOME_t);
  }

  void
  Extension_Container_i::uninstall_component (
    Components::CCMObject_ptr homeref)
  {
    CIAO_TRACE ("Extension_Container_i::uninstall_component");

    PortableServer::ServantBase_var srv_tmp =
      this->component_poa_->reference_to_servant (homeref);
    CIAO::Connector_Servant_Impl_Base * svnt =
      dynamic_cast <CIAO::Connector_Servant_Impl_Base *> (
        srv_tmp.in ());

    if (!svnt)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Extension_Container_i::uninstall_component - "
                     "Unable to convert provided servant "
                     "reference to servant implementation."));

        throw ::Components::RemoveFailure ();
      }
    else
      {
        svnt->remove ();
      }
  }

  void
  Extension_Container_i::uninstall_servant (PortableServer::Servant svnt,
                                        Container_Types::OA_Type t,
                                        PortableServer::ObjectId_out oid)
  {
    CIAO_TRACE ("Extension_Container_i::uninstall_servant");

    PortableServer::POA_ptr tmp = PortableServer::POA::_nil();

    if ((t == Container_Types::COMPONENT_t) ||
        (t == Container_Types::HOME_t))
      {
        CIAO_DEBUG (9,
                    (LM_TRACE,
                     CLINFO
                     "Extension_Container_i::uninstall_servant - "
                     "Removing component or home servant\n"));

        tmp = this->component_poa_.in ();
      }
    else
      {
        CIAO_DEBUG (9,
                   (LM_TRACE,
                    CLINFO
                    "Extension_Container_i::uninstall_servant - "
                    "Removing facet or consumer servant\n"));

        tmp = this->facet_cons_poa_.in ();
      }

    try
      {
        PortableServer::ObjectId_var tmp_id = tmp->servant_to_id (svnt);
        tmp->deactivate_object (tmp_id);

        CIAO_DEBUG (9,
                    (LM_TRACE,
                     CLINFO
                     "Extension_Container_i::uninstall_servant - "
                     "Servant successfully removed, "
                     "reference count is %u\n",
                     svnt->_refcount_value () - 1));

        oid = tmp_id._retn ();
      }
    catch (const CORBA::Exception &ex)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Extension_Container_i::uninstall_servant - "
                     "Caught CORBA exception while "
                     "uninstalling servant: %C\n",
                     ex._info ().c_str ()));

        throw Components::RemoveFailure ();
      }
  }

  CORBA::Object_ptr
  Extension_Container_i::generate_reference (const char *obj_id,
                                         const char *repo_id,
                                         Container_Types::OA_Type t)
  {
    CIAO_TRACE ("Extension_Container_i::generate_reference");

    PortableServer::POA_ptr tmp = PortableServer::POA::_nil();

    if (t == Container_Types::COMPONENT_t
        || t == Container_Types::HOME_t)
      {
        tmp = this->component_poa_.in ();
      }
    else
      {
        tmp = this->facet_cons_poa_.in ();
      }

    PortableServer::ObjectId_var oid =
      PortableServer::string_to_ObjectId (obj_id);

    CORBA::String_var str =
      PortableServer::ObjectId_to_string (oid.in ());

    CORBA::Object_var objref =
      tmp->create_reference_with_id (oid.in (), repo_id);

    return objref._retn ();
  }

  Components::Cookie *
  Extension_Container_i::install_service_reference (const char * service_id,
                                                    CORBA::Object_ptr objref)
  {
    CIAO_TRACE ("Extension_Container_i::install_service_reference");
    if (this->installed_services_.find(service_id) !=
        this->installed_services_.end())
      {
        throw Components::CCMException (::Components::SERVICE_INSTALLATION_ERROR);
      }

    //first create a cookie out of the given object reference.
    Components::Cookie_var key_cookie;
    ACE_NEW_THROW_EX (key_cookie,
                      CIAO::Cookie_Impl (reinterpret_cast<ptrdiff_t> (objref)),
                      CORBA::NO_MEMORY ());
    //create a pair of cookie and object reference
    std::pair<Components::Cookie *, CORBA::Object_ptr> ck_obj;
    ck_obj = std::make_pair(key_cookie.in (), CORBA::Object::_duplicate(objref));
    this->installed_services_[service_id] = ck_obj;

    return key_cookie;
  }

  CORBA::Object_ptr
  Extension_Container_i::uninstall_service_reference (Components::Cookie * ck)
  {
    CIAO_TRACE ("Extension_Container_i::uninstall_service_reference");
    for (InstalledServices::iterator it = this->installed_services_.begin ();
         it != this->installed_services_.end ();
         ++it)
      {
        if (it->second.first == ck)
          {
            CORBA::Object_ptr obj = it->second.second;
            this->installed_services_.erase (it);
            return CORBA::Object::_duplicate (obj);
          }
      }
    throw Components::CCMException (::Components::SERVICE_INSTALLATION_ERROR);
  }

  CORBA::Object_ptr
  Extension_Container_i::resolve_service_reference (const char *service_id)
  {
    CIAO_TRACE ("Extension_Container_i::resolve_service_reference");
    //search
    InstalledServices::iterator it = this->installed_services_.find (service_id);
    if (it != this->installed_services_.end ())
      {
        return CORBA::Object::_duplicate(it->second.second);
      }
    // not found. Calling base.
    return Container_i< CIAO::Extension_Container >::resolve_service_reference (service_id);
  }

  Components::ContainerPortableInterceptor::ClientContainerInterceptorRegistration_ptr
  Extension_Container_i::get_client_interceptor_registration (void)
  {
    CIAO_TRACE ("Extension_Container_i::get_client_interceptor_registration");

    if (!this->client_copi_registration_)
      {
        throw Components::CCMException (::Components::REGISTRATION_ERROR);
      }
    return ::Components::ContainerPortableInterceptor::ClientContainerInterceptorRegistration::_duplicate (
      this->client_copi_registration_);
  }

  Components::ContainerPortableInterceptor::ServantContainerInterceptorRegistration_ptr
  Extension_Container_i::get_servant_interceptor_registration (void)
  {
    CIAO_TRACE ("Extension_Container_i::get_servant_interceptor_registration");

    if (!this->servant_copi_registration_)
      {
        throw Components::CCMException (::Components::REGISTRATION_ERROR);
      }
    return ::Components::ContainerPortableInterceptor::ServantContainerInterceptorRegistration::_duplicate (
      this->servant_copi_registration_);
  }

  Components::ContainerPortableInterceptor::ServerContainerInterceptorRegistration_ptr
  Extension_Container_i::get_server_interceptor_registration (void)
  {
    CIAO_TRACE ("Extension_Container_i::get_server_interceptor_registration");

    if (!this->server_copi_registration_)
      {
        throw Components::CCMException (::Components::REGISTRATION_ERROR);
      }
    return ::Components::ContainerPortableInterceptor::ServerContainerInterceptorRegistration::_duplicate (
      this->server_copi_registration_);
  }

  Components::ContainerPortableInterceptor::StubContainerInterceptorRegistration_ptr
  Extension_Container_i::get_stub_interceptor_registration (void)
  {
    CIAO_TRACE ("Extension_Container_i::get_stub_interceptor_registration");

    if (!this->stub_copi_registration_)
      {
        throw Components::CCMException (::Components::REGISTRATION_ERROR);
      }
    return ::Components::ContainerPortableInterceptor::StubContainerInterceptorRegistration::_duplicate (
      this->stub_copi_registration_);
  }
}
