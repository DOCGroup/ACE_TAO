// $Id$

#include "Session_Container.h"

#include "ciao/Servants/Servant_Impl_Base.h"
#include "ciao/Logger/Log_Macros.h"
#include "ciao/Base/CIAO_ExceptionsC.h"

namespace CIAO
{
  ///////////////////////////////////////////////////////////////

  Session_Container_i::Session_Container_i (CORBA::ORB_ptr o,
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

    ACE_DLL executor_dll;
    ACE_DLL servant_dll;

    Container_i < ::CIAO::Session_Container>::prepare_installation ("Session Home",
                                                                    primary_artifact,
                                                                    entry_point,
                                                                    servant_artifact,
                                                                    servant_entrypoint,
                                                                    name,
                                                                    executor_dll,
                                                                    servant_dll);

    void *void_ptr_executor = executor_dll.symbol (ACE_TEXT_CHAR_TO_TCHAR (entry_point));
    void *void_ptr_servant = servant_dll.symbol (ACE_TEXT_CHAR_TO_TCHAR (servant_entrypoint));

    ptrdiff_t tmp_ptr = reinterpret_cast<ptrdiff_t> (void_ptr_executor);
    HomeFactory hcreator = reinterpret_cast<HomeFactory> (tmp_ptr);

    tmp_ptr = reinterpret_cast<ptrdiff_t> (void_ptr_servant);
    HomeServantFactory screator = reinterpret_cast<HomeServantFactory> (tmp_ptr);

    if (hcreator == 0)
      {
        std::ostringstream err;
        err << "Home executor factory function [" << entry_point << "] invalid in DLL ["
            << primary_artifact;

        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Session_Container_i::install_home "
                     "- Error: %C\n",
                     err.str ().c_str ()));

        throw CIAO::Installation_Failure (name,
                                          err.str ().c_str ());
      }

    if (screator == 0)
      {
        std::ostringstream err;
        err << "Home servant factory function [" << servant_entrypoint << "] invalid in DLL ["
            << servant_artifact;

        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Session_Container_i::install_home "
                     "- Error: %C\n",
                     err.str ().c_str ()));

        throw CIAO::Installation_Failure (name,
                                          err.str ().c_str ());
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

        throw CIAO::Installation_Failure (name,
                                          "Home executor factory function failed\n");
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

        throw CIAO::Installation_Failure (name,
                                          "Home servant factory function failed\n");
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

    ACE_DLL executor_dll;
    ACE_DLL servant_dll;

    Container_i < ::CIAO::Session_Container>::prepare_installation ("Session Component",
                                                                    primary_artifact,
                                                                    entry_point,
                                                                    servant_artifact,
                                                                    servant_entrypoint,
                                                                    name,
                                                                    executor_dll,
                                                                    servant_dll);

    void *void_ptr_executor = executor_dll.symbol (ACE_TEXT_CHAR_TO_TCHAR (entry_point));
    void *void_ptr_servant = servant_dll.symbol (ACE_TEXT_CHAR_TO_TCHAR (servant_entrypoint));

    ptrdiff_t tmp_ptr = reinterpret_cast<ptrdiff_t> (void_ptr_executor);
    ComponentFactory ccreator = reinterpret_cast<ComponentFactory> (tmp_ptr);

    tmp_ptr = reinterpret_cast<ptrdiff_t> (void_ptr_servant);
    ComponentServantFactory screator = reinterpret_cast<ComponentServantFactory> (tmp_ptr);

    if (ccreator == 0)
      {
        std::ostringstream err;
        err << "Entry point [" << entry_point << "] invalid in DLL ["
            << primary_artifact;

        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Session_Container_i::install_component "
                     "- Error: %C\n",
                     err.str ().c_str ()));

        throw CIAO::Installation_Failure (name,
                                          err.str ().c_str ());
      }

    if (screator == 0)
      {
        std::ostringstream err;
        err << "Entry point [" << servant_entrypoint << "] invalid in DLL ["
            << servant_artifact;

        CIAO_ERROR (1,
                    (LM_ERROR,
                     CLINFO
                     "Session_Container_i::install_component "
                     "- Error: %C\n",
                     err.str ().c_str ()));

        throw CIAO::Installation_Failure (name,
                                          err.str ().c_str ());
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

        throw CIAO::Installation_Failure (name,
                                          "Component executor factory failed");
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

        throw CIAO::Installation_Failure (name,
                                          "Componet servant factory failed");
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
