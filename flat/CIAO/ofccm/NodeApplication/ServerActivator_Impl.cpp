// $Id$

#include "ServerActivator_Impl.h"
#include "Cdmw/CDMW_IDL_ExtC.h"

const char* COMPONENT_SERVER_NAME = "ComponentServer";

bool read_config_value (const ACE_CString & name,
                        const Components::ConfigValues & config,
                        CORBA::Any_out value)
throw()
{
  bool found = false;
  CORBA::ULong len = config.length();
  for (CORBA::ULong count = 0; count < len; ++count)
    {
      if (name.compare (config[count]->name()) == 0)
        {
          value = new CORBA::Any (config[count]->value());
          found = true;
          break;
        }
    }

  return found;
}

using namespace DAnCE;

ServerActivator_Impl::ServerActivator_Impl (CORBA::ORB_ptr orb,
                                            PortableServer::POA_ptr poa,
                                            Components::Deployment::ComponentInstallation_ptr compInst)
    : orb_ (CORBA::ORB::_duplicate (orb))
    , poa_ (PortableServer::POA::_duplicate (poa))
    , compInst_ (Components::Deployment::ComponentInstallation::_duplicate (compInst))
{
  ACE_DEBUG ( (LM_DEBUG, "[%M] ServerActivator_Impl::ServerActivator_Impl - started\n"));

  CORBA::Object_var obj;
  obj = this->orb_->resolve_initial_references ("ProcessDestinationNC");
  if (CORBA::is_nil(obj))
    {
      ACE_ERROR((LM_ERROR, "[%M] ServerActivator_Impl::ServerActivator_Impl - Failed to retrieve the \"ProcessDestinationNC\" object.\n"));
      throw CORBA::ORB::InvalidName();
    }
  ACE_DEBUG ( (LM_DEBUG, "[%M] ServerActivator_Impl::ServerActivator_Impl name service reference received\n"));
  try
  {
    this->naming_ = CosNaming::NamingContext::_narrow (obj);
  }
  catch (...)
  {
    ACE_ERROR((LM_ERROR, "[%M] ServerActivator_Impl::ServerActivator_Impl - failed to narrow the \"ProcessDestinationNC\" NC.\n"));
    throw;
  }
  ACE_DEBUG ( (LM_DEBUG, "[%M] ServerActivator_Impl::ServerActivator_Impl - finished\n"));
}

ServerActivator_Impl::~ServerActivator_Impl()
{
  ACE_DEBUG ( (LM_DEBUG, "[%M] ServerActivator_Impl::~ServerActivator_Impl - started\n"));
  ACE_DEBUG ( (LM_DEBUG, "[%M] ServerActivator_Impl::~ServerActivator_Impl - finished\n"));
}

::Components::Deployment::ComponentServer_ptr
ServerActivator_Impl::create_component_server (const ::Components::ConfigValues & config)
{
  ACE_DEBUG ( (LM_DEBUG, "[%M] ServerActivator_Impl::create_component_server - started\n"));

  // NOTA: These are the configuration values understood by the server activator
  // for component server creation
  //
  // +====================================+=============================+
  // | Config Name                        | Type                        |
  // +====================================+=============================+
  // | PROCESS_DESTINATION (*)            | string                      |
  // +------------------------------------+-----------------------------+
  //
  // (*) compulsory
  //
  //Extracting process destination
  ACE_CString processDest;
  {
    CORBA::Any_var processDestAny;

    if (read_config_value (Components::PROCESS_DESTINATION, config, processDestAny))
      {
        const char* szProcessDest;
        processDestAny.in() >>= szProcessDest;
        processDest = szProcessDest;
      }
    else
      {
        ACE_ERROR ( (LM_ERROR, "[%M] ServerActivator_Impl::create_component_server - read_config_value() function fails\n"));
        throw ::Components::Deployment::InvalidConfiguration();
      }
  }
  ::Components::Deployment::ComponentServer_var srv;
  if (0 == this->servers_.find (processDest, srv))
    {
      return srv._retn();//::Components::Deployment::ComponentServer::_duplicate (srv.in());
    }
  //Looking for ComponentServer
  ACE_CString processName = processDest + "." + COMPONENT_SERVER_NAME;
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (processDest.c_str());
  name[0].kind = CORBA::string_dup (COMPONENT_SERVER_NAME);
  CORBA::Object_var obj;
  CosNaming::NamingContext_var naming;
  try
    {
      obj = this->naming_->resolve (name);
    }
  catch (CosNaming::NamingContext::NotFound& )
    {
      ACE_ERROR ( (LM_ERROR, "[%M] ServerActivator_Impl::create_component_server - NotFound exception rised."
                   "(Name : CDMW/SERVICES/ASSEMBLYANDDEPLOYMENT/%s)\n"
                   , processDest.c_str()));
      throw ::Components::CreateFailure();
    }
  catch (CosNaming::NamingContext::CannotProceed& )
    {
      ACE_ERROR ( (LM_ERROR, "[%M] ServerActivator_Impl::create_component_server - NotFound exception rised."
                   "(Name : CDMW/SERVICES/ASSEMBLYANDDEPLOYMENT/%s)\n"
                   , processDest.c_str()));
      throw ::Components::CreateFailure();
    }
  catch (CosNaming::NamingContext::InvalidName& )
    {
      ACE_ERROR ( (LM_ERROR, "[%M] ServerActivator_Impl::create_component_server - NotFound exception rised."
                   "(Name : CDMW/SERVICES/ASSEMBLYANDDEPLOYMENT/%s)\n"
                   , processDest.c_str()));
      throw ::Components::CreateFailure();
    }
  ACE_DEBUG ( (LM_DEBUG, "[%M] ServerActivator_Impl::create_component_server - ComponentServer object resolved\n"));
  //Casting founded object
  ::Components::Deployment::ComponentServer_var server;
  server = ::Components::Deployment::ComponentServer::_narrow (obj);
  ACE_DEBUG ( (LM_DEBUG, "[%M] ServerActivator_Impl::create_component_server - ComponentServer object narrowed\n"));
  if (CORBA::is_nil (server))
    {
      ACE_ERROR ( (LM_ERROR, "[%M] ServerActivator_Impl::create_component_server - ComponentServer in process destination %s  could not be contacted\n", processDest.c_str()));
      throw ::Components::CreateFailure (::Components::COMPONENT_SERVER_NOT_FOUND);
    }

  //ComponentServer initialization by ServerActivator and ComponentInstallation references
  this->initializeComponentServer (server.in());

  //Saving server reference
  //TODO add checking on already present component server with exception CdmwDeployment::COMPONENT_SERVER_ALREADY_CREATED
  if (0 != this->servers_.bind (processDest, ::Components::Deployment::ComponentServer::_duplicate (server.in())))
    {
      ACE_ERROR ( (LM_ERROR, "[%M] ServerActivator_Impl::create_component_server - ComponentServer in process destination %s already exists.\n", processDest.c_str()));
      throw ::Components::CreateFailure (::Components::COMPONENT_SERVER_ALREADY_CREATED);
    }

  ACE_DEBUG ( (LM_DEBUG, "[%M] ServerActivator_Impl::create_component_server - finished\n"));
  return server._retn ();
}

void
ServerActivator_Impl::remove_component_server (::Components::Deployment::ComponentServer_ptr server)
{
  ACE_DEBUG ( (LM_DEBUG, "[%M] ServerActivator_Impl::remove_component_server - started\n"));

  // Checking input parameters
  if (CORBA::is_nil (server))
    {
      ACE_ERROR ( (LM_ERROR, "[%M] ServerActivator_Impl::remove_component_server - Wrong input parameter\n"));
      throw CORBA::BAD_PARAM();
    }
  // find equivalent server in servers list
  bool bFound = false;
  for (TCompServers::iterator iter = this->servers_.begin();
       iter != this->servers_.end();
       ++iter)
    {
      if (server->_is_equivalent ( (*iter).int_id_.in()))
        {
          // remove ComponentServer from list
          this->servers_.unbind ( (*iter).ext_id_);
          bFound = true;
          break;
        }
    }

  // if no equivalent found, throw exception
  if (!bFound)
    {
      ACE_ERROR ( (LM_ERROR, "[%M] ServerActivator_Impl::remove_component_server - equivalent server cannot be found in servers list\n"));
      throw ::Components::RemoveFailure (::Components::UNKNOWN_COMPONENT_SERVER);
    }

  ACE_DEBUG ( (LM_DEBUG, "[%M] ServerActivator_Impl::remove_component_server - finished\n"));
}

::Components::Deployment::ComponentServers *
ServerActivator_Impl::get_component_servers ()
{
  ACE_DEBUG ( (LM_DEBUG, "[%M] ServerActivator_Impl::get_component_servers - started\n"));

  ::Components::Deployment::ComponentServers* pServers;
  ACE_NEW_THROW_EX (pServers,
                    ::Components::Deployment::ComponentServers (this->servers_.current_size()),
                    CORBA::NO_MEMORY());
  int i = 0;
  for (TCompServers::iterator iter = this->servers_.begin();
       iter != this->servers_.end();
       ++iter)
    {
      (*pServers) [i] = ::Components::Deployment::ComponentServer::_duplicate ( (*iter).int_id_.in());
      i++;
    }

  ACE_DEBUG ( (LM_DEBUG, "[%M] ServerActivator_Impl::get_component_servers - finished\n"));
  return pServers;
}

void
ServerActivator_Impl::initializeComponentServer (::Components::Deployment::ComponentServer_ptr server)
{
  ACE_DEBUG ( (LM_DEBUG, "[%M] ServerActivator_Impl::initializeComponentServer - started\n"));

  // the ComponentServer should be a Cdmw ComponentServer
  Components::Deployment::ComponentServer_var cdmwServer =
    Components::Deployment::ComponentServer::_narrow (server);

  if (CORBA::is_nil (cdmwServer))
    {
      // Component server is not Cdmw ComponentServer
      ACE_ERROR ( (LM_ERROR, "[%M] ServerActivator_Impl::initializeComponentServer - ComponentServer is not CdmwComponentServer\n"));
      throw ::Components::CreateFailure (::Components::COMPONENT_SERVER_NOT_FOUND);
    }
  if (CORBA::is_nil (this->compInst_.in()))
    {
      ACE_DEBUG ( (LM_DEBUG, "[%M] ServerActivator_Impl::initializeComponentServer - Reference on ComponentInstallation is nil!!!!\n"));
    }
  CdmwCcmComponentServer::ComponentServer_var proprietary_svr =
    CdmwCcmComponentServer::ComponentServer::_narrow (cdmwServer);

  // give it the ComponentInstallation
  proprietary_svr->set_component_installation (this->compInst_.in());
  // give it the ServerActivator
  proprietary_svr->set_server_activator (Components::Deployment::ServerActivator::_narrow (this->poa_->servant_to_reference (this)));

  ACE_DEBUG ( (LM_DEBUG, "[%M] ServerActivator_Impl::initializeComponentServer - finished\n"));
}
