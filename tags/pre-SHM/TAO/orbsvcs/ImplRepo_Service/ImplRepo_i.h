/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/ImplRepo_Service
//
// = FILENAME
//    ImplRepo_i.h
//
// = DESCRIPTION
//    This class implements the Implementation Repository.
//
// = AUTHOR
//    Darrell Brunsch <brunsch@cs.wustl.edu>
//
// ============================================================================

#ifndef IMPLREPO_I_H
#define IMPLREPO_I_H

#include "tao/ImplRepoS.h"
#include "orbsvcs/IOR_Multicast.h"
#include "tao/IOR_LookupTable.h"
#include "Repository.h"
//#include "tao/TAO.h"

// Forward declarations.
class ImplRepo_i;
class IMR_Forwarder;

// Typedefs.
typedef ImplRepo_i *ImplRepo_i_ptr;
typedef ImplRepo_i_ptr ImplRepo_i_ref;

class IMR_Adapter_Activator : public POA_PortableServer::AdapterActivator
{
  // = TITLE
  //    Implementation Repository Adapter Activator
  //
  // = DESCRIPTION
  //    Part of the Default Servant/DSI combination that forwards
  //    arbitrary requests.  This allows for the setting up of child
  //    POAs with default servants.
public:
  IMR_Adapter_Activator (IMR_Forwarder *servant,
                         PortableServer::POAManager_ptr poa_manager);
  // Constructor

  virtual CORBA::Boolean unknown_adapter (PortableServer::POA_ptr parent,
                                          const char *name,
                                          CORBA_Environment &ACE_TRY_ENV
                                            = TAO_default_environment ());
  // Called by the POA when the incoming requested object/POA isn't found.  This will
  // create POAs when needed and will also put a DSI object (IMR_Forwarder) in that POA
  // as a default servant to handle that request
private:
  IMR_Forwarder *servant_;
  // The object to use as the default servant.

  PortableServer::POAManager_var poa_manager_;
  // POA Manager
};

class ImplRepo_i 
: public POA_ImplementationRepository::Administration,
  public TAO_IOR_LookupTable_Callback
{
  // = TITLE
  //    Implementation Repository
  //
  // = DESCRIPTION
  //    This provides the interface to Administer the Implementation Repository.
public:
  // = Constructor and destructor
  ImplRepo_i (void);
  ~ImplRepo_i (void);

  virtual int find_ior (const ACE_CString &object_name, ACE_CString &ior);
  // IOR_LookupTable_Callback method.  Will return an IOR

  // = Interface methods

  virtual void activate_server (const char *server,
                                CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::Administration::NotFound,
                     ImplementationRepository::Administration::CannotActivate));
  // Starts up the server <server> if not already running.

  virtual void register_server (const char *server,
                                const ImplementationRepository::StartupOptions &options,
                                CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::Administration::AlreadyRegistered));
  // Adds the server to the repository and registers the startup information about
  // the server <server>.

  virtual void reregister_server (const char *server,
                                  const ImplementationRepository::StartupOptions &options,
                                  CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Updates the startup information about the server <server>.

  virtual void remove_server (const char *server,
                              CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::Administration::NotFound));
  // Removes the server <server> from the repository.

  virtual void shutdown_server (const char *server,
                                CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::Administration::NotFound));
  // Attempts to gracefully shut down the server, if that fails, will try
  // to do it ungracefully.

  virtual char *server_is_running (const char *server,
                                   const char *location,
                                   ImplementationRepository::ServerObject_ptr server_object,
                                   CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::Administration::NotFound));
  // Called by the server to update transient information such as current location of
  // the <server> and its ServerObject.

  virtual void server_is_shutting_down (const char *server,
                                        CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::Administration::NotFound));
  // What the server should call before it shuts down.

  virtual void find (const char *server,
                     ImplementationRepository::ServerInformation_out info,
                     CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::Administration::NotFound));
  // Returns the startup information for a server

  virtual void list (CORBA::ULong how_many,
                     ImplementationRepository::ServerInformationList_out server_list,
                     ImplementationRepository::ServerInformationIterator_out server_iterator,
                     CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Used to access the list of servers registered.  May also return an
  // iterator which can be used to access more than <how_many> of them.

  // = Other methods

  int init (int argc, char **argv,
            CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Initialize the Server state - parsing arguments and waiting.

  int run (CORBA_Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Runs the orb.

private:
  ACE_TString activate_server_i (const char *server,
                                 const int check_startup,
                                 CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     ImplementationRepository::Administration::NotFound,
                     ImplementationRepository::Administration::CannotActivate));
  // Implementation of activate_server.  <check_startup> is a flag to check 
  // the activation mode before attempting to start it.  

  IMR_Forwarder *forwarder_impl_;
  // The class that handles the forwarding.

  IMR_Adapter_Activator *activator_;
  // Used for the forwarding of any type of POA.

  Server_Repository repository_;
  // Repository containing information about each server.

  CORBA::ORB_var orb_;
  // The ORB.

  PortableServer::POA_var root_poa_;
  // The Root POA for this ORB.

  PortableServer::POA_var imr_poa_;
  // Implementation Repository's POA.

  PortableServer::POAManager_var poa_manager_;
  // The POA manager.

  char *server_key_;
  // Key of the obj ref of the server.

  char *server_input_file_;
  // Copy of the filename for the server output file.

  TAO_IOR_Multicast *ior_multicast_;
  // The ior_multicast event handler.

  CORBA::String_var imr_ior_;
  // Implementation Repository's IOR.  Why do we store it here?  Multicast 
  // doesn't work otherwise.

  int argc_;
  // Number of command line arguments.

  char **argv_;
  // The command line arguments.
  
  friend class IMR_Forwarder;
};

class IMR_Forwarder: public PortableServer::DynamicImplementation
  // = TITLE
  //    Implementation Repository Forwarder
  //
  // = DESCRIPTION
  //    This class is provides a DSI implementation that is used to handle
  //    arbitrary calls and forward them to the correct place.
{
public:
  IMR_Forwarder (CORBA::ORB_ptr orb_ptr,
                 PortableServer::POA_ptr poa_ptr,
                 ImplRepo_i *ir_impl);
  // Constructor

  virtual void invoke (CORBA::ServerRequest_ptr request,
                       CORBA::Environment &env);
  // The invoke() method receives requests issued to any CORBA
  // object incarnated by the DSI servant and performs the
  // processing necessary to execute the request.

  CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &oid,
                                          PortableServer::POA_ptr poa,
                                          CORBA::Environment &env = TAO_default_environment ());
  // DynamicImplementation stuff

private:
  class ImplRepo_i *imr_impl_;
  // Where we find out where to forward to.

  CORBA::ORB_var orb_var_;
  // ORB reference.

  PortableServer::POA_var poa_var_;
  // POA reference.
};

class IMR_Iterator : public POA_ImplementationRepository::ServerInformationIterator
{
public:
  IMR_Iterator (Server_Repository::HASH_IMR_ITER *iterator,
               PortableServer::POA_ptr poa);
  // Constructor
  // Ownership of iterator is transfered to this class (we'll delete it)

  ~IMR_Iterator ();
  // Destructor

  virtual CORBA::Boolean next_n (CORBA::ULong how_many,
                                 ImplementationRepository::ServerInformationList_out server_list,
                                 CORBA::Environment &ACE_TRY_ENV = CORBA::Environment::default_environment ())
     ACE_THROW_SPEC ((CORBA::SystemException));
  // Returns the next list of up to <how_many> servers.  If empty, will return
  // false.

  virtual void destroy (CORBA::Environment &ACE_TRY_ENV = CORBA::Environment::default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Destroys the iterator.

private:
  Server_Repository::HASH_IMR_ITER *iterator_;
  // Our very own iterator for transversing the server repository.

  PortableServer::POA_var poa_;
  // Our lovely POA.
};

#endif /* IMPLREPO_I_H */
