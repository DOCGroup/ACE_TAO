// $Id$

#ifndef IMR_LOCATOR_I_H
#define IMR_LOCATOR_I_H
#include /**/ "ace/pre.h"

#include "Adapter_Activator.h"
#include "Forwarder.h"
#include "INS_Locator.h"
#include "Locator_Options.h"
#include "locator_export.h"

#include "orbsvcs/orbsvcs/IOR_Multicast.h"

#include "tao/PortableServer/ImR_LocatorS.h"

#include "ace/Hash_Map_Manager.h"
#include "ace/Null_Mutex.h"
#include "ace/SString.h"

class ACE_Reactor;

struct ActivatorInfo {
  CORBA::Long token; // A unique token is assigned at registration
  ImplementationRepository::Administration_var admin;
};

// Look up activator information by activator name.
typedef ACE_Hash_Map_Manager_Ex <ACE_CString,
                                 ActivatorInfo,
                                 ACE_Hash<ACE_CString>,
                                 ACE_Equal_To<ACE_CString>,
                                 ACE_Null_Mutex> ActivatorMap;

// Look up activator name by server name. (where the server name is the name of a poa 
//      registered in the call to server_is_running().)
typedef ACE_Hash_Map_Manager_Ex <ACE_CString,
                                 ACE_CString,
                                 ACE_Hash<ACE_CString>,
                                 ACE_Equal_To<ACE_CString>,
                                 ACE_Null_Mutex> ServerMap;


// Gets a request from a client and depending on the POA name,
// requests an activator to take care of activating the
// corresponding server and raises a forward exception to the
// client pointing to the correct server.
class Locator_Export ImR_Locator_i : public virtual POA_ImplementationRepository::Locator
{
 public:
  ImR_Locator_i();

  /// Initialize the service, creating its own orb, poa, etc.
  int init (Options& opts ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  /// Same as above, but use the given orb
  int init_with_orb (CORBA::ORB_ptr orb, Options& opts ACE_ENV_ARG_DECL_WITH_DEFAULTS);
  /// Cleans up any state created by init*.
  int fini (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  /// Run using the orb reference created during init()
  int run(ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  // Register an activator with the locator. 
  // Returns a token that must be used when unregistering.
  virtual CORBA::Long register_activator (const char *activator,
    ImplementationRepository::Administration_ptr admin,
    const ImplementationRepository::Locator::ServerNameList & servers
    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // UnRegister an activator with the locator.
  virtual void unregister_activator (const char *activator,
    CORBA::Long token ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Starts up the server <server> if not already running.
  virtual void activate_server (const char *server
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::NotFound,
                     ImplementationRepository::CannotActivate));

  // Starts up the server <server> if not already running.
  virtual void activate_server_in_activator (const char *server,
                                            const char *activator
                                            ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::NotFound,
                     ImplementationRepository::CannotActivate));

  // Adds the server to the repository and registers the startup information
  // about the server <server>.
  virtual void register_server (const char *server,
                                const ImplementationRepository::StartupOptions &options
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
                                ACE_THROW_SPEC ((CORBA::SystemException, 
                                ImplementationRepository::AlreadyRegistered,
                                ImplementationRepository::NotFound));

  // Updates the startup information about the server <server>.
  virtual void reregister_server (const char *server,
                                  const ImplementationRepository::StartupOptions &options
                                  ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,ImplementationRepository::NotFound ));

  // Removes the server <server> from the repository.
  virtual void remove_server (const char *server
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound));

  // Removes the server <server> from the repository.
  virtual void remove_server_in_activator (const char *server,
                                          const char *activator
                                          ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound));

  // Attempts to gracefully shut down the server,
  virtual void shutdown_server (const char *server
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound));

  // Attempts to gracefully shut down the server <server> in activator
  // <activator>
  virtual void shutdown_server_in_activator (const char *server,
                                            const char *activator
                                            ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound));

  // Invoked by the server to update transient information such as current
  // activator of the <server> and its ServerObject.
  virtual void server_is_running_in_activator (const char *server,
                                   const char* activator,
                                   const char* partial_ior,
                                   ImplementationRepository::ServerObject_ptr server_object
                                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound));

  // This version should only be used by the activator.
  virtual void server_is_running (const char* server,
                                   const char* partial_ior,
                                   ImplementationRepository::ServerObject_ptr server_object
                                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound));

  // Only used by activator.
  virtual void server_is_shutting_down (const char *server ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound));

  // What the server <server> should call before it shuts down from
  // activator <activator>
  virtual void server_is_shutting_down_in_activator (const char *server,
                                                    const char *activator
                                                    ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound));

  /// IOR_LookupTable_Callback method helper. Will return an IOR
  char *find_ior (const char* object_name ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound));

  virtual void find (const char *server,
                     ImplementationRepository::ServerInformation_out info
                     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound));

  // Returns the startup information for a server
  virtual void find_in_activator (const char *server,
                     const char* activator,
                     ImplementationRepository::ServerInformation_out info
                     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound));

  // Used to access the list of servers registered.  May also return an
  // iterator which can be used to access more than <how_many> of them.
  virtual void list (
      CORBA::ULong how_many,
      ImplementationRepository::ServerInformationList_out server_list,
      ImplementationRepository::ServerInformationIterator_out server_iterator
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Starts up the server <server> on one or more activators.
  // Returns a partial ior for the server that is missing only the ObjectKey.
  char* activate_server_with_startup (const char *server,
                                      int check_startup
                                      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::NotFound,
                     ImplementationRepository::CannotActivate));

  // The following methods are used internally by the locator to contact
  // the activator for information. They are not IDL methods.
  char* start_server(const char* server ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::NotFound,
                     ImplementationRepository::CannotActivate));

  char* find_ior_i (const char* object_name ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::NotFound));

 private:

  // Set up the multicast related if 'm' is passed on the command
  // line.
  int setup_multicast (ACE_Reactor *reactor, const char *ior);

  // As the name suggests, this methods helps choose the activator
  // based on the activator passed.
  ImplementationRepository::Administration_ptr
    choose_activator(const char *activator ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((ImplementationRepository::NotFound, CORBA::SystemException));

  void unregister_activator_i(const char* activator);

private:
  // Table that maintains the activator to Object Reference of the
  // Activator running in that activator.
  ActivatorMap activator_map_;
  ServerMap server_map_;

  // The class that handles the forwarding.
  ImR_Forwarder forwarder_;

  // Used for the forwarding of any type of POA.
  ImR_Adapter_Activator activator_;

  /// The locator interface for the IORTable
  INS_Locator ins_locator_;

  CORBA::ORB_var orb_;
  PortableServer::POA_var root_poa_;
  PortableServer::POA_var imr_poa_;

  int debug_;

  TAO_IOR_Multicast ior_multicast_;
};

#include /**/ "ace/post.h"
#endif /* IMR_LOCATOR_I_H */
