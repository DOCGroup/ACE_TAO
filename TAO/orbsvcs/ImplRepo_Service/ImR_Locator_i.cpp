// $Id$

#include "ImR_Locator_i.h"

#include "Locator_Options.h"
#include "Multi_Iterator.h"

#include "tao/IORTable/IORTable.h"
#include "tao/PortableServer/POA.h"
#include "tao/ORB_Core.h"
#include "tao/default_ports.h"

#include "ace/Vector_T.h"
#include "ace/ARGV.h"
#include "ace/OS_NS_sys_time.h"

ImR_Locator_i::ImR_Locator_i ()
: forwarder_(*this)
, ins_locator_(*this)
, debug_(0)
{
}

int
ImR_Locator_i::init_with_orb (CORBA::ORB_ptr orb, Options& opts ACE_ENV_ARG_DECL)
{
  orb_ = CORBA::ORB::_duplicate(orb);
  debug_ = opts.debug();

  CORBA::Object_var obj =
    this->orb_->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->root_poa_ = PortableServer::POA::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  ACE_ASSERT(! CORBA::is_nil(this->root_poa_.in()));

  this->forwarder_.init(orb ACE_ENV_ARG_PARAMETER);
  this->activator_.init(& this->forwarder_);
  // Register the Adapter_Activator reference to be the RootPOA's
  // Adapter Activator.
  root_poa_->the_activator (&this->activator_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::PolicyList policies;
  imr_poa_ = root_poa_->create_POA ("ImplRepoService",
    PortableServer::POAManager::_nil(), policies ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  PortableServer::ObjectId_var id = this->imr_poa_->activate_object(this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  obj = this->imr_poa_->id_to_reference (id.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  CORBA::String_var ior = this->orb_->object_to_string(obj.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Get the reference to IORTable.
  obj = orb->resolve_initial_references ("IORTable" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Narrow it down to the correct type
  IORTable::Table_var ior_table = IORTable::Table::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  ACE_ASSERT(! CORBA::is_nil(ior_table.in()));

  ior_table->set_locator (&this->ins_locator_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ior_table->bind ("ImplRepoService", ior.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (opts.multicast())
  {
    ACE_Reactor* reactor = orb->orb_core()->reactor();
    if (this->setup_multicast (reactor, ior.in()) != 0)
      return -1;
  }

  PortableServer::POAManager_var poaman =
    this->root_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  poaman->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  poaman = this->imr_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  poaman->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // We write the ior file last so that the tests can know we are ready.
  if (opts.ior_filename().length() > 0)
  {
    FILE* fp = ACE_OS::fopen(opts.ior_filename().c_str(), "w");
    if (fp == 0) 
    {
      ACE_ERROR_RETURN ((LM_ERROR, 
        "ImR Locator: Could not open file: %s\n", opts.ior_filename().c_str()), -1);
    }
    ACE_OS::fprintf(fp, "%s", ior.in());
    ACE_OS::fclose(fp);
  }

  return 0;
}

int
ImR_Locator_i::init(Options& opts ACE_ENV_ARG_DECL)
{
  ACE_ARGV av(opts.cmdline().c_str());
  int argc = av.argc();
  char** argv = av.argv();
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, "TAO_ImR_Locator" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);
  int err = this->init_with_orb(orb.in(), opts ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);
  return err;
}

int
ImR_Locator_i::run(ACE_ENV_SINGLE_ARG_DECL) 
{
  this->orb_->run(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  return 0;
}

int
ImR_Locator_i::fini (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_TRY
  {
    this->root_poa_->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    this->orb_->destroy(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "ImR_Locator_i::fini");
    ACE_RE_THROW;
  }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

int
ImR_Locator_i::setup_multicast (ACE_Reactor* reactor, const char* ior)
{
  ACE_ASSERT (reactor != 0);
  ACE_ASSERT (ior != 0);
#if defined (ACE_HAS_IP_MULTICAST)

  TAO_ORB_Core* core = TAO_ORB_Core_instance();
  // See if the -ORBMulticastDiscoveryEndpoint option was specified.
  ACE_CString mde (core->orb_params()->mcast_discovery_endpoint());

  if (mde.length () != 0)
  {
    if (this->ior_multicast_.init(ior,
      mde.c_str(), TAO_SERVICEID_IMPLREPOSERVICE) == -1)
    {
      return -1;
    }
  }
  else
  {
    // Port can be specified as param, env var, or default
    CORBA::UShort port = core->orb_params()->service_port(IMPLREPOSERVICE);
    if (port == 0)
    {
      // Check environment var. for multicast port.
      const char *port_number = ACE_OS::getenv ("ImplRepoServicePort");

      if (port_number != 0)
        port = ACE_OS::atoi (port_number);
    }
    if (port == 0)
      port = TAO_DEFAULT_IMPLREPO_SERVER_REQUEST_PORT;

    if (this->ior_multicast_.init(ior, port, 
      ACE_DEFAULT_MULTICAST_ADDR,TAO_SERVICEID_IMPLREPOSERVICE) == -1)
    {
      return -1;
    }
  }

  // Register event handler for the ior multicast.
  if (reactor->register_handler (&this->ior_multicast_,
    ACE_Event_Handler::READ_MASK) == -1)
  {
    if (debug_ >= 1)
      ACE_DEBUG ((LM_DEBUG, "ImR Locator: cannot register Event handler\n"));
    return -1;
  }

  if (this->debug_ >= 1)
    ACE_DEBUG ((LM_DEBUG, "ImR Locator: Multicast Handler enabled.\n"));

  return 0;
#else /* ACE_HAS_IP_MULTICAST */
  ACE_UNUSED_ARG (reactor);
  ACE_UNUSED_ARG (ior);
  return 0;
#endif /* ACE_HAS_IP_MULTICAST */
}

CORBA::Long
ImR_Locator_i::register_activator (const char* activator,
                                   ImplementationRepository::Administration_ptr admin,
                                   const ImplementationRepository::Locator::ServerNameList & servers
                                   ACE_ENV_ARG_DECL)
                                   ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_ASSERT(activator != 0);
  ACE_ASSERT(! CORBA::is_nil(admin));
  // Before we can register the activator, we need to ensure that any existing
  // registration is purged.
  this->unregister_activator_i(activator);
  ACE_CHECK_RETURN(0);

  for (CORBA::ULong idx = 0; idx < servers.length(); ++idx) {
    const char* server = servers[idx];
    int err = this->server_map_.bind(server, activator);
    ACE_ASSERT(err == 0 || err == 1);
    ACE_UNUSED_ARG (err);
  }

  ActivatorInfo info;
  // We save the unique hash value of the passed in admin object, so that 
  // we can prevent accidental unregister by a different activator.
  info.token = ACE_OS::gettimeofday().msec(); 
  info.admin = ImplementationRepository::Administration::_duplicate(admin);

  int bind_return = this->activator_map_.bind (activator, info);

  if (bind_return == 1) // Success, but was already bound.
  {
    if (this->debug_ >= 1)
    {
      ACE_DEBUG ((LM_DEBUG, "ImR Locator: New activator registered for %s.\n", activator));
    }
    return info.token; 
  }
  else if (bind_return != 0) 
  {
    ACE_ASSERT(bind_return != 0);
    return 0;
  }

  if (this->debug_ >= 1)
  {
    ACE_DEBUG ((LM_DEBUG, "ImR Locator: Activator registered for %s.\n", activator));
  }
  return info.token;
}

// Remove a activator when it no longer wants to receive requests.
// object_ref is set to the removed activator.
void
ImR_Locator_i::unregister_activator (const char * activator,
                                     CORBA::Long token
                                     ACE_ENV_ARG_DECL)
                                     ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_ASSERT(activator != 0);
  ActivatorInfo info;
  if (this->activator_map_.find (activator, info) == 0) 
  {
    if (info.token != token && this->debug_ >= 1) 
    {
      ACE_DEBUG((LM_DEBUG, "ImR Locator: Ignoring unregister activator:%s. Wrong token.\n", activator));
      return;
    }

    this->unregister_activator_i(activator);
    ACE_CHECK;

    if (this->debug_ >= 1)
    {
      ACE_DEBUG ((LM_DEBUG, "ImR Locator: Activator %s unregistered.\n", activator));
    }
  } 
  else 
  {
    if (this->debug_ >= 1)
    {
      ACE_DEBUG((LM_DEBUG, "ImR Locator: Ignoring unregister activator:%s. Unknown activator.\n", activator));
    }
  }
}

void 
ImR_Locator_i::unregister_activator_i(const char* activator)
{
  ACE_ASSERT(activator != 0);
  int bind_return = this->activator_map_.unbind (activator); 
  if (bind_return != 0)
    return;

  ACE_Vector<ACE_CString> servers_to_delete;
  // Note : There may be a much more efficient way to unbind the entries
  ServerMap::ENTRY* entry = 0;
  ServerMap::ITERATOR it(this->server_map_);
  for (; it.next(entry) != 0; it.advance())
  {
    if (entry->int_id_ == activator)
    {
      servers_to_delete.push_back(entry->ext_id_);
    }
  }
  for (size_t i = 0; i < servers_to_delete.size(); ++i)
  {
    this->server_map_.unbind(servers_to_delete[i]);
  }
}

void
ImR_Locator_i::activate_server (const char *server ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((
                CORBA::SystemException,
                ImplementationRepository::NotFound,
                ImplementationRepository::CannotActivate))
{
  ACE_ASSERT(server != 0);
  if (this->debug_ >= 1)
  {
    ACE_DEBUG ((LM_DEBUG, "ImR Locator: Activating server %s.\n", server));
  }

  ACE_CString activator;
  if (this->server_map_.find(server, activator) == 0) 
  {
    this->activate_server_in_activator(server, activator.c_str()  ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  } 
  else 
  {
    ACE_ERROR ((LM_ERROR, "ImR Locator: Couldn't find an activator for server:%s\n", server));
    ACE_THROW(ImplementationRepository::NotFound());
  }
}

// Starts up the server <server> at one or more activators
// and returns the IOR of the activated server.
char *
ImR_Locator_i::activate_server_with_startup (const char*, int ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException,
                ImplementationRepository::NotFound,
                ImplementationRepository::CannotActivate))
{
  ACE_ERROR((LM_ERROR, "ImR_Locator_i::activate_server_with_startup()"
    " This operation is only for the ImR_Activator.\n"));
  ACE_THROW_RETURN(CORBA::NO_IMPLEMENT(), 0);
}

char* 
ImR_Locator_i::start_server(const char* server ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException,
                ImplementationRepository::NotFound,
                ImplementationRepository::CannotActivate))
{
  ACE_ASSERT(server != 0);
  if (this->debug_ >= 1)
    ACE_DEBUG ((LM_DEBUG, "ImR Locator: Activating server %s.\n", server));

  ACE_CString activator;
  if (this->server_map_.find(server, activator) == 0) 
  {
    ImplementationRepository::Administration_var admin =
      this->choose_activator (activator.c_str() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);
    if (! CORBA::is_nil(admin.in()))
    {
      char* partial_ior = admin->activate_server_with_startup (server, 1 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
      if (partial_ior != 0) {
        return partial_ior; 
      }
    }
  } 
  ACE_ERROR ((LM_ERROR, "ImR Locator: Couldn't find an activator for server:%s\n", server));
  ACE_THROW_RETURN (ImplementationRepository::NotFound(), 0);
}


// Starts up the server <server> if not already running in the
// specified activator.
void
ImR_Locator_i::activate_server_in_activator (const char* server,
                                             const char* activator
                                             ACE_ENV_ARG_DECL)
                                             ACE_THROW_SPEC ((CORBA::SystemException,
                                             ImplementationRepository::NotFound,
                                             ImplementationRepository::CannotActivate))
{
  ACE_ASSERT(server != 0);
  ACE_ASSERT(activator != 0);
  if (this->debug_ >= 1)
  {
    ACE_DEBUG ((LM_DEBUG, "ImR Locator: Activating server"
      " %s in activator %s.\n", server, activator));
  }
  ImplementationRepository::Administration_var admin =
    this->choose_activator (activator ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  admin->activate_server (server ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

// Adds the server to the repository and registers the startup information
// about the server <server>.
void
ImR_Locator_i::register_server (const char *server,
                                const ImplementationRepository::StartupOptions &options
                                ACE_ENV_ARG_DECL)
                                ACE_THROW_SPEC ((CORBA::SystemException,
                                ImplementationRepository::AlreadyRegistered,
                                ImplementationRepository::NotFound))
{
  ACE_ASSERT(server != 0);
  ACE_CString activator;
  if (this->server_map_.find(server, activator) == 0) 
  {
    ACE_THROW(ImplementationRepository::AlreadyRegistered());
  }

  ImplementationRepository::Administration_var admin =
    this->choose_activator (options.activator.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  admin->register_server (server, options ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (this->debug_ >= 1)
  {
    ACE_DEBUG ((LM_DEBUG, "ImR Locator: Registered server %s "
      "in activator %s.\n", server, options.activator.in()));
  }
  int bind_return = this->server_map_.bind (server, options.activator.in());
  ACE_UNUSED_ARG (bind_return);
  ACE_ASSERT(bind_return == 0);
}

// Updates the startup information about the server <server>.
// Can not currently be used to move a server from one activator to another,
// this simply updates the registered info.
void
ImR_Locator_i::reregister_server (
                                  const char *server,
                                  const ImplementationRepository::StartupOptions &options
                                  ACE_ENV_ARG_DECL )
                                  ACE_THROW_SPEC ((CORBA::SystemException,ImplementationRepository::NotFound))
{
  ACE_ASSERT(server != 0);
  ACE_CString activator;
  if (this->server_map_.find(server, activator) != 0) 
  {
    ACE_THROW(ImplementationRepository::NotFound());
  }
  if (activator.compare(options.activator.in()) != 0) 
  {
    ACE_THROW(ImplementationRepository::NotFound());
  }

  ImplementationRepository::Administration_var admin =
    this->choose_activator (activator.c_str() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->debug_ >= 1)
  {
    ACE_DEBUG ((LM_DEBUG, "ImR Locator: Reregister server %s "
      "in activator %s.\n", server, activator.c_str()));
  }
  admin->reregister_server (server, options ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

// Removes the server <server> from the repository.
void
ImR_Locator_i::remove_server (const char *server ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound))
{
  ACE_ASSERT(server != 0);
  if (this->debug_ >= 1)
  {
    ACE_DEBUG ((LM_DEBUG, "ImR Locator: Remove server %s.\n", server));
  }
  ACE_CString activator;
  if (this->server_map_.find(server, activator) == 0) 
  {
    this->remove_server_in_activator(server, activator.c_str()  ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  } 
  else 
  {
    ACE_ERROR ((LM_ERROR, "ImR Locator: Couldn't find an activator for server:%s\n", server));
    ACE_THROW(ImplementationRepository::NotFound());
  }
}

// Removes the server <server> from the repository.
void
ImR_Locator_i::remove_server_in_activator (const char *server,
                                           const char *activator
                                           ACE_ENV_ARG_DECL)
                                           ACE_THROW_SPEC ((CORBA::SystemException,
                                           ImplementationRepository::NotFound
                                           ))
{
  ACE_ASSERT(server != 0);
  ACE_ASSERT(activator != 0);
  if (this->debug_ >= 1)
  {
    ACE_DEBUG ((LM_DEBUG, "ImR Locator: Remove server %s "
      "from activator %s.\n", server, activator));
  }
  ACE_CString registered_activator;
  if (this->server_map_.find(server, registered_activator) != 0) 
  {
    ACE_THROW(ImplementationRepository::NotFound());
  }
  if (registered_activator.compare(activator) != 0) 
  {
    ACE_ERROR ((LM_ERROR, 
      "ImR Locator: Wrong activator: %s for server: %s expected: %s\n", 
      activator, server, registered_activator.c_str()));
    ACE_THROW(ImplementationRepository::NotFound());
  }

  ImplementationRepository::Administration_var admin =
    this->choose_activator (activator ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  admin->remove_server (server ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  int err = this->server_map_.unbind(server);
  ACE_UNUSED_ARG (err);
  ACE_ASSERT(err == 0);
}

// Attempts to gracefully shut down the server, at all activators that
// it is running.
void
ImR_Locator_i::shutdown_server (const char *server ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound))
{
  ACE_ASSERT(server != 0);
  if (this->debug_ >= 1)
  {
    ACE_DEBUG ((LM_DEBUG, "ImR Locator: Shutdown server %s.\n", server));
  }
  ACE_CString activator;
  if (this->server_map_.find(server, activator) == 0) 
  {
    this->shutdown_server_in_activator(server, activator.c_str()  ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  } 
  else 
  {
    ACE_ERROR ((LM_ERROR, "ImR Locator: Couldn't find an activator for server:%s\n", server));
    ACE_THROW(ImplementationRepository::NotFound());
  }
}

// Attempts to gracefully shut down the server,
void
ImR_Locator_i::shutdown_server_in_activator (
  const char *server,
  const char *activator
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound))
{
  ACE_ASSERT(server != 0);
  ACE_ASSERT(activator != 0);
  ImplementationRepository::Administration_var admin =
    this->choose_activator (activator ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (this->debug_ >= 1)
  {
    ACE_DEBUG ((LM_DEBUG, "ImR Locator: Shutdown server %s "
      "in activator %s.\n", server, activator));
  }
  admin->shutdown_server (server ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
ImR_Locator_i::server_is_running (const char *,
                                  const char *,
                                  ImplementationRepository::ServerObject_ptr
                                  ACE_ENV_ARG_DECL)
                                  ACE_THROW_SPEC ((CORBA::SystemException,
                                  ImplementationRepository::NotFound))
{
  ACE_ERROR((LM_ERROR, "ImR_Locator_i::server_is_running() "
    "This operation is only for the ImR_Activator.\n"));
  ACE_THROW(CORBA::NO_IMPLEMENT());
}

void
ImR_Locator_i::server_is_running_in_activator (const char* server,
                                               const char* activator,
                                               const char* partial_ior,
                                               ImplementationRepository::ServerObject_ptr server_object
                                               ACE_ENV_ARG_DECL)
                                               ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound))
{
  ACE_ASSERT(server != 0);
  ACE_ASSERT(activator != 0);
  ACE_ASSERT(partial_ior != 0);
  ACE_ASSERT(! CORBA::is_nil(server_object));
  ImplementationRepository::Administration_var admin =
    this->choose_activator (activator ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (this->debug_ >= 1)
  {
    ACE_DEBUG ((LM_DEBUG, "ImR Locator: Server %s "
      "running in activator %s.\n", server, activator));
  }
  admin->server_is_running (server, partial_ior, server_object ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  // We must make sure that the server is associated with the specified activator
  this->server_map_.bind(server, activator);
}

void
ImR_Locator_i::server_is_shutting_down (const char* ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound))
{
  ACE_ERROR((LM_ERROR, "ImR_Locator_i::server_is_shutting_down()"
    " This operation is only for the ImR_Activator.\n"));
  ACE_THROW(CORBA::NO_IMPLEMENT());
}

// What the server should call before it shuts down.
void
ImR_Locator_i::server_is_shutting_down_in_activator (
  const char *server,
  const char *activator
  ACE_ENV_ARG_DECL
  ) ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound))
{
  ACE_ASSERT(server != 0);
  ACE_ASSERT(activator != 0);
  ImplementationRepository::Administration_var admin =
    this->choose_activator (activator ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (this->debug_ >= 1)
  {
    ACE_DEBUG ((LM_DEBUG, "ImR Locator: Server %s "
      "shutting down in activator %s.\n", server, activator));
  }
  admin->server_is_shutting_down (server ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
ImR_Locator_i::find (
                     const char * server,
                     ImplementationRepository::ServerInformation_out info
                     ACE_ENV_ARG_DECL)
                     ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound))
{
  ACE_ASSERT(server != 0);
  ACE_CString activator;
  if (this->server_map_.find(server, activator) == 0) 
  {
    this->find_in_activator(server, activator.c_str(), info  ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  } 
  else 
  {
    ACE_ERROR ((LM_ERROR, "ImR Locator: Couldn't find an activator for server:%s\n", server));
    ACE_THROW(ImplementationRepository::NotFound());
  }
}

void
ImR_Locator_i::find_in_activator (
                                  const char* server,
                                  const char* activator,
                                  ImplementationRepository::ServerInformation_out info
                                  ACE_ENV_ARG_DECL)
                                  ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound))
{
  ACE_ASSERT(server != 0);
  ACE_ASSERT(activator != 0);
  ImplementationRepository::Administration_var admin =
    this->choose_activator (activator ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (this->debug_ >= 1)
  {
    ACE_DEBUG ((LM_DEBUG, "ImR Locator: Find server %s "
      "in activator %s.\n", server, activator));
  }
  admin->find(server, info ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

// Used to access the list of servers registered.  May also return an
// iterator which can be used to access more than <how_many> of them.
void
ImR_Locator_i::list (CORBA::ULong how_many,
                     ImplementationRepository::ServerInformationList_out server_list,
                     ImplementationRepository::ServerInformationIterator_out server_iterator
                     ACE_ENV_ARG_DECL
                     ) ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->debug_ >= 1)
    ACE_DEBUG ((LM_DEBUG, "ImR Locator: List servers.\n"));

  server_iterator = ImplementationRepository::ServerInformationIterator::_nil();
  ACE_NEW_THROW_EX (server_list, 
    ImplementationRepository::ServerInformationList(0), CORBA::NO_MEMORY());

  if (how_many == 0) 
    return;

  ImR_Multi_Iterator::ItList iters(this->activator_map_.current_size());
  ACE_Vector<ImplementationRepository::ServerInformationList*> 
    silstlst(this->activator_map_.current_size());

  CORBA::ULong cnt = 0;
  CORBA::ULong total = 0;
  ActivatorMap::ENTRY* entry = 0;
  ActivatorMap::ITERATOR iterator(this->activator_map_);
  for (; iterator.next(entry) != 0; iterator.advance())
  {
    ImplementationRepository::ServerInformationList_var tmplst;
    ImplementationRepository::ServerInformationIterator_var tmpit;
    ImplementationRepository::Administration_ptr admin = entry->int_id_.admin.in();
    CORBA::ULong n = how_many - cnt;
    admin->list(n, tmplst.out(), tmpit.out() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
    cnt += tmplst->length();
    if (! CORBA::is_nil(tmpit.in()))
    {
      iters.push_back(tmpit);
    }
    if (n > 0 && tmplst->length() > 0) 
    {
      ACE_ASSERT(n >= tmplst->length());
      total += tmplst->length();
      silstlst.push_back(tmplst._retn());
    }
  }

  // Copy each of the returned sequences to our output sequence. 
  if (silstlst.size() > 0) {
    ACE_ASSERT(total > 0);
    server_list->length(total);
    CORBA::ULong i = 0;
    CORBA::ULong k = 0;
    for(i = 0; i < silstlst.size(); ++i) 
    {
      CORBA::ULong sz = silstlst[i]->length();
      for (CORBA::ULong j = 0; j < sz; ++j)
      {
	// Originally we stored a SIList_var directly in the silstlst, but
	// this would not compile on all platforms (sunCC 5.4)
	ImplementationRepository::ServerInformationList_var tmplst(silstlst[i]);
        server_list[k++] = tmplst[j];
      }
    }
  }

  // Activate a Multi_Iterator to allow the caller to retrieve the remainder
  if (iters.size() > 0) 
  {
    ImR_Multi_Iterator* mi;

    ACE_NEW_THROW_EX (mi, ImR_Multi_Iterator (iters, this->imr_poa_.in()), CORBA::NO_MEMORY());

    ACE_TRY
    {
      PortableServer::ObjectId_var id = this->imr_poa_->activate_object (mi ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::Object_var obj = this->imr_poa_->id_to_reference(id.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      server_iterator = ImplementationRepository::
        ServerInformationIterator::_narrow(obj.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
    ACE_CATCHANY
    {
      ACE_RE_THROW;
    }
    ACE_ENDTRY;
    ACE_CHECK;
  }
}

ImplementationRepository::Administration_ptr
ImR_Locator_i::choose_activator (const char *activator ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((ImplementationRepository::NotFound, CORBA::SystemException))
{
  ACE_ASSERT(activator != 0);
  ActivatorInfo info;
  if (this->activator_map_.find (activator, info) != 0)
  {
    ACE_THROW_RETURN (ImplementationRepository::NotFound (), 
      ImplementationRepository::Administration::_nil());
  }
  return info.admin._retn();
}

char *
ImR_Locator_i::find_ior (const char* ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound))
{
  ACE_ERROR((LM_ERROR, "ImR_Locator_i::find_ior()"
    " This operation is only for the ImR_Activator.\n"));
  ACE_THROW_RETURN(CORBA::NO_IMPLEMENT(), 0);
}

char *
ImR_Locator_i::find_ior_i (const char* object_name ACE_ENV_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound))
{
  ACE_ASSERT(object_name != 0);
  if (this->debug_ >= 1)
    ACE_DEBUG ((LM_DEBUG, "ImR Locator: Find ior %s.\n", object_name));

  // We assume that the first part of the object name is the server name.
  // So a name of foo/bar means that the server name is foo.
  ACE_CString server_name(object_name);
  ssize_t pos = server_name.find ('/');
  if (pos != ACE_CString::npos) 
    server_name = server_name.substr(pos + 1);

  ACE_CString activator;
  if (this->server_map_.find(server_name, activator) != 0) 
  {
    ACE_ERROR ((LM_ERROR, "ImR Locator: Couldn't find an activator for object:%s\n", object_name));
    ACE_THROW_RETURN(ImplementationRepository::NotFound(), 0);
  }

  ImplementationRepository::Administration_var admin =
    this->choose_activator (activator.c_str() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(0);

  if (CORBA::is_nil(admin.in()))
  {
    ACE_ERROR ((LM_ERROR, "ImR Locator: Couldn't find an activator for object:%s\n", object_name));
    ACE_THROW_RETURN(ImplementationRepository::NotFound(), 0);
  }

  char* ior = admin->find_ior(object_name ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(0);

  ACE_ASSERT(ior != 0);
  return ior;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Manager<ACE_CString, ActivatorInfo, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, ActivatorInfo, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_CString, ActivatorInfo, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, ActivatorInfo, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<ACE_CString, ActivatorInfo>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString, ActivatorInfo, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, ActivatorInfo, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, ActivatorInfo, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Manager<ACE_CString, ACE_CString, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_CString, ACE_CString, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<ACE_CString, ACE_CString>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString, ACE_CString, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;

template class ACE_Array_Base<ImplementationRepository::ServerInformationList*>;
template class ACE_Array<ImplementationRepository::ServerInformationList*>;
template class ACE_Vector<ImplementationRepository::ServerInformationList*>;

template class ACE_Array_Base<ACE_CString>;
template class ACE_Array<ACE_CString>;
template class ACE_Vector<ACE_CString>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Manager<ACE_CString,ActivatorInfo,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, ActivatorInfo, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString,ActivatorInfo,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, ActivatorInfo, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, ActivatorInfo>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString, ActivatorInfo, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, ActivatorInfo, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, ActivatorInfo, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Manager<ACE_CString,ACE_CString,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString,ACE_CString,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, ACE_CString>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString, ACE_CString, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>

#pragma instantiate ACE_Array_Base<ImplementationRepository::ServerInformationList*>
#pragma instantiate ACE_Array<ImplementationRepository::ServerInformationList*>
#pragma instantiate ACE_Vector<ImplementationRepository::ServerInformationList*>

#pragma instantiate ACE_Array_Base<ACE_CString>
#pragma instantiate ACE_Array<ACE_CString>
#pragma instantiate ACE_Vector<ACE_CString>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
