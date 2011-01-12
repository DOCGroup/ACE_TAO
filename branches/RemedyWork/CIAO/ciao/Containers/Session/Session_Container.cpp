// $Id$

#include "Session_Container.h"

#include "ciao/Servants/Servant_Impl_Base.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  ///////////////////////////////////////////////////////////////

  Session_Container_i::Session_Container_i (
        CORBA::ORB_ptr o,
        PortableServer::POA_ptr poa)
    : Container_i < ::CIAO::Session_Container> (o, poa)
  {
  }

  Session_Container_i::~Session_Container_i (void)
  {
  }

  void
  Session_Container_i::init (const char *name)
  {
    Container_i < ::CIAO::Session_Container>::init (name);
  }

  void
  Session_Container_i::fini (void)
  {
    CIAO_TRACE ("Session_Container_i::fini");

    Container_i < ::CIAO::Session_Container>::fini ();
  }

  //@@ Apparently we need to be cautious when handling the exception
  //   thrown here. We should make sure that new DnC interfaces
  //   NodeApplication/NodeApplicationManager etc will cache the new
  //   exceptions--> rethrow of new exceptions is needed.
  //                                            --Tao
  Components::CCMHome_ptr
  Session_Container_i::install_home (const char *primary_artifact,
                                     const char *entry_point,
                                     const char *servant_artifact,
                                     const char *servant_entrypoint,
                                     const char *name)
  {
    CIAO_TRACE ("Session_Container_i::install_home");

    CIAO_DEBUG (6,
                (LM_DEBUG,
                  CLINFO
                  "Session_Container_i::install_home - "
                  "Loading home [%C] from shared libraries\n",
                  name));

    CIAO_DEBUG (6,
                (LM_DEBUG,
                  CLINFO
                  "Session_Container_i::install_home - "
                  "Executor library [%C] with entrypoint [%C]\n",
                  primary_artifact,
                  entry_point));

    CIAO_DEBUG (6,
                (LM_DEBUG,
                  CLINFO
                  "Session_Container_i::install_home - "
                  "Servant library [%C] with entrypoint [%C]\n",
                  servant_artifact,
                  servant_entrypoint));

    if (!primary_artifact)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                      CLINFO
                      "Session_Container_i::install_home - "
                      "ERROR: Null component executor DLL name\n"));

        throw Components::Deployment::UnknownImplId ();
      }

    if (!servant_artifact)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                      CLINFO
                      "Session_Container_i::install_home - "
                      "ERROR: Null component servant DLL name\n"));

        throw Components::Deployment::UnknownImplId ();
      }

    if (!entry_point)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                      CLINFO
                      "Session_Container_i::install_home - "
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
                      "Session_Container_i::install_home - "
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
                      "Session_Container_i::install_home - "
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
                    "Session_Container_i::install_home - "
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
                      "Session_Container_i::install_home - "
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
                      "Session_Container_i::install_home - "
                      "Servant DLL [%C] successfully openend\n",
                     servant_artifact));
      }

    // We have to do this casting in two steps because the C++
    // standard forbids casting a pointer-to-object (including
    // void*) directly to a pointer-to-function.
    void *void_ptr =
      executor_dll.symbol (ACE_TEXT_CHAR_TO_TCHAR (entry_point));
    ptrdiff_t tmp_ptr = reinterpret_cast<ptrdiff_t> (void_ptr);

    HomeFactory hcreator = reinterpret_cast<HomeFactory> (tmp_ptr);

    void_ptr =
      servant_dll.symbol (ACE_TEXT_CHAR_TO_TCHAR (servant_entrypoint));
    tmp_ptr = reinterpret_cast<ptrdiff_t> (void_ptr);
    HomeServantFactory screator = reinterpret_cast<HomeServantFactory> (tmp_ptr);

    if (!hcreator)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Session_Container_i::install_home "
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
                     "Session_Container_i::install_home"
                     " - Error: Entry point [%C] "
                     "invalid in dll [%C]\n",
                     servant_entrypoint,
                     servant_artifact));

        throw Components::Deployment::ImplEntryPointNotFound ();
      }

    CIAO_DEBUG (9,
                (LM_TRACE,
                 CLINFO
                 "Session_Container_i::install_home"
                 " - Loading home executor\n"));

    Components::HomeExecutorBase_var home_executor = hcreator ();

    if (CORBA::is_nil (home_executor.in ()))
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Session_Container_i::install_home - "
                     "Home executor factory failed.\n"));

        throw Components::Deployment::InstallationFailure ();
      }

    CIAO_DEBUG (9,
                (LM_TRACE,
                 CLINFO
                 "Session_Container_i::install_home"
                 " - Loading home servant\n"));

    PortableServer::Servant home_servant =
      screator (home_executor.in (), this, name);

    if (home_servant == 0)
      {
            CIAO_ERROR (1,
                        (LM_ERROR,
                         CLINFO
                         "Session_Container_i::install_home - "
                         "Home servant factory failed.\n"));

        throw Components::Deployment::InstallationFailure ();
      }

    PortableServer::ServantBase_var safe (home_servant);

    CIAO_DEBUG (9,
                (LM_TRACE,
                 CLINFO
                 "Session_Container_i::install_home "
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
                 "Session_Container_i::install_home - "
                 "Home successfully created with name\n"));

    return homeref._retn ();
  }

  Components::CCMObject_ptr
  Session_Container_i::install_component (const char *primary_artifact,
                                          const char *entry_point,
                                          const char *servant_artifact,
                                          const char *servant_entrypoint,
                                          const char *name)
  {
    CIAO_TRACE ("Session_Container_i::install_component");

    CIAO_DEBUG (6,
                (LM_DEBUG,
                  CLINFO
                  "Session_Container_i::install_component - "
                  "Loading component [%C] from shared libraries\n",
                  name));

    CIAO_DEBUG (6,
                (LM_DEBUG,
                  CLINFO
                  "Session_Container_i::install_component - "
                  "Executor library [%C] with entrypoint [%C]\n",
                  primary_artifact,
                  entry_point));

    CIAO_DEBUG (6,
                (LM_DEBUG,
                  CLINFO
                  "Session_Container_i::install_component - "
                  "Servant library [%C] with entrypoint [%C]\n",
                  servant_artifact,
                  servant_entrypoint));

    if (primary_artifact == 0)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO
                "Session_Container_i::install_component - "
                "ERROR: Null component executor DLL name\n"));
        throw Components::Deployment::UnknownImplId ();
      }

    if (servant_artifact == 0)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                      CLINFO
                      "Session_Container_i::install_component - "
                      "ERROR: Null component servant DLL name\n"));

        throw Components::Deployment::UnknownImplId ();
      }

    if (entry_point == 0)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                      CLINFO
                      "Session_Container_i::install_component - "
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
                      "Session_Container_i::install_component - "
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
                      "Session_Container_i::install_component - "
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
                     "Session_Container_i::install_component"
                     " - Executor DLL [%C] successfully opened\n",
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
                      "Session_Container_i::install_component - "
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
                     "Session_Container_i::install_component "
                     "- Servant DLL [%C] successfully openend\n",
                     servant_artifact));
      }

    // We have to do this casting in two steps because the C++
    // standard forbids casting a pointer-to-object (including
    // void*) directly to a pointer-to-function.
    void *void_ptr =
      executor_dll.symbol (ACE_TEXT_CHAR_TO_TCHAR (entry_point));
    ptrdiff_t tmp_ptr = reinterpret_cast<ptrdiff_t> (void_ptr);
    ComponentFactory ccreator = reinterpret_cast<ComponentFactory> (tmp_ptr);

    void_ptr =
      servant_dll.symbol (ACE_TEXT_CHAR_TO_TCHAR (servant_entrypoint));
    tmp_ptr = reinterpret_cast<ptrdiff_t> (void_ptr);
    ComponentServantFactory screator = reinterpret_cast<ComponentServantFactory> (tmp_ptr);

    if (ccreator == 0)
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Session_Container_i::install_component "
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
                     "Session_Container_i::install_component "
                     "- Error: Entry point [%C] "
                     "invalid in dll [%C]\n",
                     servant_entrypoint,
                     servant_artifact));

        throw Components::Deployment::ImplEntryPointNotFound ();
      }

    CIAO_DEBUG (9,
                (LM_TRACE,
                 CLINFO
                 "Session_Container_i::install_component - "
                 "Loading component executor\n"));

    Components::EnterpriseComponent_var component_executor =
      ccreator ();

    if (CORBA::is_nil (component_executor.in ()))
      {
        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Session_Container_i::install_component - "
                     "Component executor factory failed.\n"));

        throw Components::Deployment::InstallationFailure ();
      }

    CIAO_DEBUG (9,
                (LM_TRACE,
                 CLINFO
                 "Session_Container_i::install_component - "
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
                   "Session_Container_i::install_component - "
                   "Component servant factory failed.\n"));

        throw Components::Deployment::InstallationFailure ();
      }

    PortableServer::ServantBase_var safe (component_servant);

    CIAO_DEBUG (9,
                (LM_TRACE,
                 CLINFO
                 "Session_Container_i::install_component - "
                 "Installing component servant\n"));

    PortableServer::ObjectId_var oid;

    CORBA::Object_var objref =
      this->install_servant (component_servant,
                             Container_Types::COMPONENT_t,
                             oid.out ());

    Components::CCMObject_var componentref =
      Components::CCMObject::_narrow (objref.in ());

    CIAO_DEBUG (9, (LM_TRACE, CLINFO
                "Session_Container_i::install_component - "
                "Component successfully created\n"));

    return componentref._retn ();
  }
}
