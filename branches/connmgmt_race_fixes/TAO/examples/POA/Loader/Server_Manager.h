// This may look like C, but it's really -*- C++ -*-
// $Id$

//=================================================================================
//
// = LIBRARY
//     TAO/examples/POA/Loader
//
// = FILENAME
//     Server_Manager.h
//
// = DESCRIPTION
//     Helper class for the server application.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
//==================================================================================

#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

#include "ace/streams.h"
#include "Servant_Activator.h"
#include "Servant_Locator.h"

class Server_i
{
  // = TITLE
  //   This class provides the server application with helper methods
  //
  // = DESCRIPTION
  //   The various methods required by the server application for
  //   utilizing the ServantActivator and ServantLocator interfaces of
  //   the Servant Manager are implemented by this class.  These
  //   include initialisation procedures of the ServantActivator and
  //   ServantLocator, and creation of POAs with emphasis on the
  //   servant retention policy.
public:
  // = Initialization and termination methods.
  Server_i (void);
  // Initialisation.

  ~Server_i (void);
  // Destruction.

  int init (int argc, char **argv);
  // Initialisation of the ORB and poa.

  PortableServer::POA_ptr create_poa (const char* name,
                                      int servant_retention_policy);
  // This method creates a POA from the root_poa with emphasis being
  // on the servant_retention_policy which decides the use of the
  // ServantActivator or ServantLocator interfaces. The
  // servent_retention_policy value is 1 for the RETAIN policy and 0
  // for the NONRETAIN policy.

  int create_activator (PortableServer::POA_var first_poa);
  // A ServantActivator object is created and initialised.

  int create_locator (PortableServer::POA_var second_poa);
  // A ServantActivator object is created and initialised.

  int run (void);
  // The server is executed.

private:
  int parse_args (int argc, char **argv);
  // Parses the input arguments.

  int write_iors_to_file (const char *first_ior,
                          const char *second_ior);
  // The IORs are written to a file for future use.

  char *ior_output_file_;
  // Default ior file.

  CORBA::ORB_var orb_;
  // The orb pointer.

  CORBA::PolicyList policies_;
  // The poa policicies.

  PortableServer::POA_var root_poa_;
  // The root_poa which is used for cretaing different child poas.

  PortableServer::POAManager_var poa_manager_;
  // The poa_manager object.

  CORBA::Object_var first_foo_;
  // The object pointer used by the ServantActivator.

  CORBA::Object_var second_foo_;
  // The object pointer used by the Servant Locator.

  PortableServer::ServantManager_var servant_activator_;
  // The servant activator object.

  PortableServer::ServantManager_var servant_locator_;
  // The servant locator object.
};

#endif /* SERVER_MANAGER_H */
