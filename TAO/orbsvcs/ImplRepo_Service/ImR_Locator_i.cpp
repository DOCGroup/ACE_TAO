// $Id$

#include "ImR_Locator_i.h"

#include "Locator_Options.h"
#include "Forwarder.h"
#include "INS_Locator.h"
#include "Adapter_Activator.h"

#include "orbsvcs/orbsvcs/IOR_Multicast.h"

#include "tao/IORTable/IORTable.h"
#include "tao/PortableServer/POA.h"

#include "tao/ORB_Core.h"
#include "tao/default_ports.h"

#include "ace/Get_Opt.h"
#include "ace/Process_Manager.h"
#include "ace/OS_NS_sys_time.h"

ImR_Locator_i::ImR_Locator_i ()
  : 
    forwarder_impl_ (0),
    activator_ (0),
    ins_locator_ (0)
{
  this->ins_locator_ = new INS_Locator(this);
}

int
ImR_Locator_i::init (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::ORB_var orb = OPTIONS::instance()->orb();

  CORBA::Object_var obj =
    orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  PortableServer::POA_var root_poa =
    PortableServer::POA::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (CORBA::is_nil (root_poa.in ()))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Unable to obtain RootPOA reference.\n"),
                      -1);

  PortableServer::POAManager_var poa_manager =
    root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // We are going to set the policies to be used with the child
  // POA.
  CORBA::PolicyList policies (2);
  policies.length (2);

  policies[0] =
    root_poa->create_id_assignment_policy (PortableServer::USER_ID
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  policies[1] =
    root_poa->create_lifespan_policy (PortableServer::PERSISTENT
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  PortableServer::POA_var locator_poa =
    root_poa->create_POA ("ImplRepoService",
                          poa_manager.in (),
                          policies
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Creation of the new POAs is over, so destroy the Policy_ptr's as
  // we dont need them anymore.
  for (CORBA::ULong i = 0; i < policies.length (); ++i)
    {
      CORBA::Policy_ptr policy = policies[i];
      policy->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }

  // Register 'this' with the ImR_Forwarder
  ACE_NEW_RETURN (this->forwarder_impl_,
                  ImR_Forwarder (this, orb.in ()),
                  -1);

  // Get the ObjectID for the string child POA name.
  PortableServer::ObjectId_var locator_id =
    PortableServer::string_to_ObjectId ("ImplRepoService");

  // Activate the object with the given id.
  locator_poa->activate_object_with_id (locator_id.in (),
                                        this
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Get the locator object reference
  CORBA::Object_var locator_obj = 
    locator_poa->id_to_reference (locator_id.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Get the reference to IORTable.
  obj = orb->resolve_initial_references ("IORTable" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Narrow it down to the correct type
  IORTable::Table_var adapter =
    IORTable::Table::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CORBA::String_var ior;

  if (CORBA::is_nil (adapter.in ()))
    {
      ACE_ERROR ((LM_ERROR, "Nil IORTable\n"));
    }
  else
    {
      // Set the locator
      adapter->set_locator (this->ins_locator_.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      // Get the stringified format of the locator's object
      // reference.
      ior = orb->object_to_string (locator_obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      // Finally, bind this ior to the child POA's name in the
      // IORTable.
      adapter->bind ("ImplRepoService", ior.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }

  ACE_CString output_filename = OPTIONS::instance()->output_filename();
  
  if (output_filename.length() > 0)
    {
      FILE* fp = ACE_OS::fopen(output_filename.c_str(), "w");
      ACE_ASSERT(fp != 0);
      ACE_OS::fprintf(fp, "%s", ior.in());
      ACE_OS::fclose(fp);
    }

  ACE_NEW_RETURN (this->activator_,
                  ImR_Adapter_Activator (this->forwarder_impl_),
                  -1);

  // Register the Adapter_Activator reference to be the RootPOA's
  // Adapter Activator.
  root_poa->the_activator (this->activator_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (OPTIONS::instance()->multicast())
    {
      ACE_Reactor *reactor = orb->orb_core ()->reactor ();

      ACE_Process_Manager process_manager;
      process_manager.open (ACE_Process_Manager::DEFAULT_SIZE, reactor);

      if (this->setup_multicast (reactor, ior.in ()) != 0)
        return -1;
    }
  else
    {
      if (OPTIONS::instance()->debug() >= 1)
        {
          ACE_DEBUG ((LM_DEBUG, "ImR Locator: Multicast Handler disabled.\n"));
        }
    }

  poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  orb->run (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

/**
 * First, figure out if we should set up a multicast handler (based on command
 * line settings) and which port to set it up (based on the environment,
 * command line, or the default IMPLREPOSERVICE port).  Then register the
 * handler with the @param reactor.
 *
 * @retval  0 ok
 * @retval -1 failed
 */
int
ImR_Locator_i::setup_multicast (ACE_Reactor *reactor,
                                const char *ior)
{
  ACE_ASSERT (reactor != 0);
#if defined (ACE_HAS_IP_MULTICAST)

  // See if the -ORBMulticastDiscoveryEndpoint option was specified.
  ACE_CString mde (TAO_ORB_Core_instance ()->orb_params ()->mcast_discovery_endpoint ());

  // First, see if the user has given us a multicast port number
  // on the command-line;
  u_short port =
    TAO_ORB_Core_instance ()->orb_params ()->service_port (IMPLREPOSERVICE);

  if (port == 0)
    {
      // Check environment var. for multicast port.
      const char *port_number = ACE_OS::getenv ("ImplRepoServicePort");

      if (port_number != 0)
        port = ACE_OS::atoi (port_number);
    }

  // Port wasn't specified on the command-line or in environment -
  // use the default.
  if (port == 0)
    port = TAO_DEFAULT_IMPLREPO_SERVER_REQUEST_PORT;

  // Instantiate a handler which will handle client requests for
  // the ImplRepoService ior, received on the multicast port.
  TAO_IOR_Multicast *ior_multicast;

  ACE_NEW_RETURN (ior_multicast,
                  TAO_IOR_Multicast (),
                  -1);

  if (mde.length () != 0)
    {
      // initializes
      if (ior_multicast->init (ior,
                               mde.c_str (),
                               TAO_SERVICEID_IMPLREPOSERVICE) == -1)
        return -1;
    }
  else
    {
      if (ior_multicast->init (ior,
                               port,
                               ACE_DEFAULT_MULTICAST_ADDR,
                               TAO_SERVICEID_IMPLREPOSERVICE) == -1)
        return -1;
    }

  // Register event handler for the ior multicast.
  if (reactor->register_handler (ior_multicast,
                                 ACE_Event_Handler::READ_MASK) == -1)
    {
      if (OPTIONS::instance()->debug() >= 1)
        ACE_DEBUG ((LM_DEBUG,
                    "ImR Locator: "
                    "cannot register Event handler\n"));
      return -1;
    }

  if (OPTIONS::instance()->debug() >= 1)
    ACE_DEBUG ((LM_DEBUG,
                "ImR Locator: Multicast Handler enabled.\n"));

  return 0;
#else /* ACE_HAS_IP_MULTICAST */

  ACE_UNUSED_ARG (reactor);
  ACE_UNUSED_ARG (ior);
  ACE_DEBUG ((LM_WARNING,
              "ImR Locator: "
              "ACE_HAS_IP_MULTICAST is not defined. "
              "Multicast support disabled.\n"));

  return 0;
#endif /* ACE_HAS_IP_MULTICAST */
}

CORBA::Long
ImR_Locator_i::register_activator (const char* activator,
                                   ImplementationRepository::Administration_ptr admin,
                                   const ImplementationRepository::Locator::ServerNameList & servers
                                   ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
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
    if (OPTIONS::instance()->debug() >= 1)
    {
      ACE_DEBUG ((LM_DEBUG, "ImR Locator: New activator registered for %s.\n", activator));
    }
    return info.token; 
  }
  else if (bind_return != 0) 
  {
    ACE_ASSERT(bind_return != 0);
  }

  if (OPTIONS::instance()->debug() >= 1)
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
  ActivatorInfo info;
  if (this->activator_map_.find (activator, info) == 0) 
  {
    if (info.token != token && OPTIONS::instance()->debug() >= 1) 
    {
      ACE_DEBUG((LM_DEBUG, "ImR Locator: Ignoring unregister activator:%s. Wrong token.\n", activator));
      return;
    }
  
    int bind_return = this->activator_map_.unbind (activator, info); 
    ACE_UNUSED_ARG (bind_return);
    ACE_ASSERT(bind_return == 0);

    if (OPTIONS::instance()->debug() >= 1)
    {
      ACE_DEBUG ((LM_DEBUG, "ImR Locator: Activator %s unregistered.\n", activator));
    }
  } 
  else 
  {
    if (OPTIONS::instance()->debug() >= 1)
    {
      ACE_DEBUG((LM_DEBUG, "ImR Locator: Ignoring unregister activator:%s. Unknown activator.\n", activator));
    }
  }
}

// Starts up the server <server> at all the activators where it is
// registered, if not already running.
void
ImR_Locator_i::activate_server (const char *server ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
       CORBA::SystemException,
       ImplementationRepository::NotFound,
       ImplementationRepository::CannotActivate))
{
  if (OPTIONS::instance()->debug() >= 1)
    {
      ACE_DEBUG ((LM_DEBUG, "ImR Locator: Activating server %s.\n", server));
    }

  ACE_CString activator;
  if (this->server_map_.find(server, activator) == 0) 
  {
    this->activate_server_in_activator(server, activator.c_str());
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
ImR_Locator_i::activate_server_with_startup (const char *server,
                                             int check_startup
                                             ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ImplementationRepository::NotFound,
                   ImplementationRepository::CannotActivate))
{
  if (OPTIONS::instance()->debug() >= 1)
  {
    ACE_DEBUG ((LM_DEBUG, "ImR Locator: Activating server %s."
      " Check startup=%d.\n", server, check_startup));
  }

  ACE_CString activator;
  if (this->server_map_.find(server, activator) == 0) 
  {
    ImplementationRepository::Administration_var admin =
      this->choose_activator (activator.c_str() ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);
    char* partial_ior = admin->activate_server_with_startup (server,
      check_startup ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (0);
    if (partial_ior != 0) {
      return partial_ior; 
    }
  } 
  ACE_ERROR ((LM_ERROR, "ImR Locator: Couldn't find an activator for server:%s\n", server));
  ACE_THROW_RETURN (ImplementationRepository::NotFound(), 0);
  return 0;
}


// Starts up the server <server> if not already running in the
// specified activator.
void
ImR_Locator_i::activate_server_in_activator (const char *server,
                                            const char * activator
                                            ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ImplementationRepository::NotFound,
                   ImplementationRepository::CannotActivate))
{
  if (OPTIONS::instance()->debug() >= 1)
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
  ACE_CString activator;
  if (this->server_map_.find(server, activator) == 0) 
  {
    ACE_THROW(ImplementationRepository::AlreadyRegistered());
    return;
  }

  ImplementationRepository::Administration_var admin =
    this->choose_activator (options.activator.in() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  admin->register_server (server, options ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (OPTIONS::instance()->debug() >= 1)
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

  if (OPTIONS::instance()->debug() >= 1)
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
  if (OPTIONS::instance()->debug() >= 1)
    {
      ACE_DEBUG ((LM_DEBUG, "ImR Locator: Remove server %s.\n", server));
    }
  ACE_CString activator;
  if (this->server_map_.find(server, activator) == 0) 
  {
    this->remove_server_in_activator(server, activator.c_str());
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
  if (OPTIONS::instance()->debug() >= 1)
    {
      ACE_DEBUG ((LM_DEBUG, "ImR Locator: Remove server %s "
        "from activator %s.\n", server, activator));
    }
  ACE_CString registered_activator;
  if (this->server_map_.find(server, registered_activator) != 0) 
  {
    ACE_THROW(ImplementationRepository::NotFound()); // todo : Add to signature in pidl and others
  }
  if (registered_activator.compare(activator) != 0) 
  {
    ACE_ASSERT(false);
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
  if (OPTIONS::instance()->debug() >= 1)
    {
      ACE_DEBUG ((LM_DEBUG, "ImR Locator: Shutdown server %s.\n", server));
    }
  ACE_CString activator;
  if (this->server_map_.find(server, activator) == 0) 
  {
    this->shutdown_server_in_activator(server, activator.c_str());
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
  ImplementationRepository::Administration_var admin =
    this->choose_activator (activator ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (OPTIONS::instance()->debug() >= 1)
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
                                  ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   ImplementationRepository::NotFound))
{
  ACE_ERROR((LM_ERROR, "ImR_Locator_i::server_is_running() "
    "This operation is only for the ImR_Activator.\n"));
  ACE_ASSERT(false);
}

void
ImR_Locator_i::server_is_running_in_activator (const char* server,
                                               const char* activator,
                                               const char* partial_ior,
                                               ImplementationRepository::ServerObject_ptr server_object
                                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound))
{
  // todo : At this point we could automatically register the server, so that 
  // manually started servers don't require tao_imr add first.
  ImplementationRepository::Administration_var admin =
    this->choose_activator (activator ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (OPTIONS::instance()->debug() >= 1)
    {
      ACE_DEBUG ((LM_DEBUG, "ImR Locator: Server %s "
        "running in activator %s.\n", server, activator));
    }
  admin->server_is_running (server, partial_ior, server_object ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
ImR_Locator_i::server_is_shutting_down (const char* ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound))
{
  ACE_ERROR((LM_ERROR, "ImR_Locator_i::server_is_shutting_down()"
    " This operation is only for the ImR_Activator.\n"));
  ACE_ASSERT(false);
}

// What the server should call before it shuts down.
void
ImR_Locator_i::server_is_shutting_down_in_activator (
      const char *server,
      const char *activator
      ACE_ENV_ARG_DECL
    ) ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound))
{
  ImplementationRepository::Administration_var admin =
    this->choose_activator (activator ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (OPTIONS::instance()->debug() >= 1)
    {
      ACE_DEBUG ((LM_DEBUG, "ImR Locator: Server %s "
        "shutting down in activator %s.\n", server, activator));
    }
  admin->server_is_shutting_down (server ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

// Find 
void
ImR_Locator_i::find (
      const char * server,
      ImplementationRepository::ServerInformation_out info
      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound))
{
  ACE_CString activator;
  if (this->server_map_.find(server, activator) == 0) 
  {
    this->find_in_activator(server, activator.c_str(), info);
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
  ImplementationRepository::Administration_var admin =
    this->choose_activator (activator ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  if (OPTIONS::instance()->debug() >= 1)
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
  if (OPTIONS::instance()->debug() >= 1)
  {
    ACE_DEBUG ((LM_DEBUG, "ImR Locator: List servers.\n"));
  }
  ActivatorMap::ENTRY *next_entry = 0;
  int cnt = 0;

  for (ActivatorMap::ITERATOR iterator (this->activator_map_);
    iterator.next (next_entry) != 0;
    iterator.advance ())
  {
    ImplementationRepository::Administration_ptr admin = next_entry->int_id_.admin.in();
    ++cnt;
    // todo : Check to make sure activator implementation supports repeat calls correctly.
    admin->list (how_many,
      server_list,
      server_iterator
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
  }

  if (cnt <= 0) 
  {
    // Temporary fix for the seg fault that occurs when the skel tries to
    // handle the unitialised sequence when this method is called and
    // no activator has been registered yet.
    // @todo A full rework of this method is needed - see bug #1543. 
    server_iterator =
      ImplementationRepository::ServerInformationIterator::_nil (); 
    ACE_NEW_THROW_EX (server_list,
      ImplementationRepository::ServerInformationList (0),
      CORBA::NO_MEMORY ());
  }
}

ImplementationRepository::Administration_ptr
ImR_Locator_i::choose_activator (const char *activator ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((ImplementationRepository::NotFound, CORBA::SystemException))
{
  ActivatorInfo info;
  if (this->activator_map_.find (activator, info) != 0)
    {
      ACE_THROW_RETURN (ImplementationRepository::NotFound (), 0);
    }
  return info.admin._retn();
}

char *
ImR_Locator_i::find_ior (const char *object_name ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound))
{
  if (OPTIONS::instance()->debug() >= 1)
    {
      ACE_DEBUG ((LM_DEBUG, "ImR Locator: Find ior %s.\n", object_name));
    }

  ActivatorMap::ENTRY *next_entry = 0;
  
  for (ActivatorMap::ITERATOR iterator (this->activator_map_);
       iterator.next (next_entry) != 0;
       iterator.advance ())
    {
      ImplementationRepository::Administration_ptr admin = next_entry->int_id_.admin.in();
      ACE_TRY
        {
          char* ior = admin->find_ior (object_name ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          return ior;    
        }
      ACE_CATCH (ImplementationRepository::NotFound, ex)
        {
          ACE_UNUSED_ARG(ex);
        }
      ACE_ENDTRY;
    }
  
  ACE_ERROR ((LM_ERROR, "ImR Locator: Couldn't find an activator for object:%s\n", object_name));
  ACE_THROW_RETURN (ImplementationRepository::NotFound (), 0);
  return 0;
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

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
