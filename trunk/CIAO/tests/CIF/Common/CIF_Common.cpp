// $Id$

#include "CIF_Common.h"
#include "ciao/Logger/Logger_Service.h"
#include "ciao/ComponentServer/Client_init.h"

CIF_Common::CIF_Common (void)
{
}

CIF_Common::~CIF_Common (void)
{
}

void
CIF_Common::init (int argc, ACE_TCHAR *argv[])
{
  ::CIAO::Logger_Service logger;
  logger.init (argc, argv);
  this->orb_ = CORBA::ORB_init (argc, argv);
  ::CIAO::Client_init (this->orb_);

  CORBA::Object_var object =
    this->orb_->resolve_initial_references ("RootPOA");

  this->root_poa_ =
    PortableServer::POA::_narrow (object.in ());

  PortableServer::POAManager_var poa_manager =
    this->root_poa_->the_POAManager ();

  poa_manager->activate ();
}

void
CIF_Common::shutdown (::CIAO::Deployment::ComponentServer_ptr server,
                      ::CIAO::Deployment::Container_ptr cont,
                      ::Components::CCMObject_ptr comp)
{
  cont->remove_component (comp);
  server->remove_container (cont);

  this->sa_->remove_component_server (server);
  this->orb_->shutdown ();
}

::CIAO::Deployment::ComponentServer_ptr
CIF_Common::create_componentserver (const int& spawn_delay,
                                    const char * cs_path,
                                    const char * artifact_name)
{
  ::CIAO::Deployment::ComponentInstallation_Impl *tmp_ci = 0;

  ACE_NEW_THROW_EX (tmp_ci,
                    CIAO::Deployment::ComponentInstallation_Impl (),
                    CORBA::NO_MEMORY ());

  this->root_poa_->activate_object (tmp_ci);

  ::CIAO::Deployment::CIAO_ServerActivator_i *sa_tmp = 0;
  ACE_NEW_THROW_EX (
    sa_tmp,
    ::CIAO::Deployment::CIAO_ServerActivator_i (spawn_delay,
                                                cs_path,
                                                0,
                                                false,
                                                tmp_ci->_this (),
                                                this->orb_.in (),
                                                this->root_poa_.in ()),
    CORBA::NO_MEMORY ());

  if (::CORBA::is_nil (this->sa_.in ()))
    {
      this->sa_ = sa_tmp->_this ();
    }
  // Make a componentserver with no configvalues
  ::CIAO::Deployment::ComponentServer_var
    server (::CIAO::Deployment::ComponentServer::_narrow
      (this->sa_->create_component_server (0)));

  if (CORBA::is_nil (server.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "Nil componentserver reference"),
                         0);
    }

  ACE_CString svnt = artifact_name, exec = artifact_name;
  svnt += "_svnt";
  exec += "_exec";
  tmp_ci->install (svnt.c_str (), svnt.c_str ());
  tmp_ci->install (exec.c_str (), exec.c_str ());

  return server._retn ();
}

::CIAO::Deployment::Container_ptr
CIF_Common::create_container (::CIAO::Deployment::ComponentServer_ptr server)
{
  ::Components::Deployment::Container_var tmp = server->create_container (0);
  ::CIAO::Deployment::Container_var cont =
    ::CIAO::Deployment::Container::_narrow (tmp.in ());
  return cont._retn ();
}

::Components::CCMObject_ptr
CIF_Common::install_component (::CIAO::Deployment::Container_ptr cont,
                               const char * artifact_name)
{
  CORBA::Any val;
  ::Components::ConfigValues configs(3);
  configs.length (3);

  ACE_CString tmp = "create_";
  ACE_CString impl_name = tmp;

  tmp       += artifact_name;
  impl_name += artifact_name;
  tmp += "_Servant";
  impl_name += "_Impl";

  val <<= tmp.c_str();
  ACE_NEW_THROW_EX (
    configs[0],
    CIAO::ConfigValue_impl (CIAO::Deployment::SVNT_ENTRYPT,
                            val),
    CORBA::NO_MEMORY ());
  tmp = artifact_name;
  tmp += "_svnt";
  val <<= tmp.c_str ();
  ACE_NEW_THROW_EX (
    configs[1],
    CIAO::ConfigValue_impl (CIAO::Deployment::SVNT_ARTIFACT,
                            val),
    CORBA::NO_MEMORY ());

  tmp = artifact_name;
  tmp += "_exec";
  val <<= tmp.c_str ();
  ACE_NEW_THROW_EX (
    configs[2],
    CIAO::ConfigValue_impl (CIAO::Deployment::EXEC_ARTIFACT,
                            val),
    CORBA::NO_MEMORY ());

  // Install Component
  Components::CCMObject_var cmp = Components::CCMObject::_nil ();
  try
    {
      cmp = cont->install_component (tmp.c_str (),
                                     impl_name.c_str (),
                                     configs);
    }
  catch (const ::Components::Deployment::UnknownImplId &e)
    {
      ACE_ERROR ((LM_ERROR, "CIF_Common::install_component - "
                  "::Components::Deployment::UnknownImplId caught.\n"));
      return ::Components::CCMObject::_nil ();
    }
  catch (const ::Components::Deployment::ImplEntryPointNotFound &e)
    {
      ACE_ERROR ((LM_ERROR, "CIF_Common::install_component - "
                  "::Components::Deployment::ImplEntryPointNotFound caught\n"));
      return ::Components::CCMObject::_nil ();
    }
  catch (const ::Components::Deployment::InstallationFailure &e)
    {
      ACE_ERROR ((LM_ERROR, "CIF_Common::install_component - "
                  "::Components::Deployment::InstallationFailure caught\n"));
      return ::Components::CCMObject::_nil ();
    }
  catch (const ::Components::Deployment::InvalidConfiguration &e)
    {
      ACE_ERROR ((LM_ERROR, "CIF_Common::install_component - "
                  "::Components::Deployment::InvalidConfiguration caught\n"));
      return ::Components::CCMObject::_nil ();
    }

  if (CORBA::is_nil (cmp))
    {
      ACE_ERROR ((LM_ERROR, "CIF_Common::install_component - "
                  "Got back a nil component ref from install_component\n"));
      return ::Components::CCMObject::_nil ();
    }
  return cmp._retn ();
}
