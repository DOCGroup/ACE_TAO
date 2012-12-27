// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file     Server_Manager.h
 *
 *  $Id$
 *
 *   Helper class for the server application.
 *
 *
 *  @author  Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#ifndef SERVER_MANAGER_H
#define SERVER_MANAGER_H

#include "Servant_Activator.h"
#include "Servant_Locator.h"

/**
 * @class Server_i
 *
 * @brief This class provides the server application with helper methods
 *
 * The various methods required by the server application for
 * utilizing the ServantActivator and ServantLocator interfaces of
 * the Servant Manager are implemented by this class.  These
 * include initialisation procedures of the ServantActivator and
 * ServantLocator, and creation of POAs with emphasis on the
 * servant retention policy.
 */
class Server_i
{
public:
  // = Initialization and termination methods.
  /// Initialisation.
  Server_i (void);

  /// Destruction.
  ~Server_i (void);

  /// Initialisation of the ORB and poa.
  int init (int argc, ACE_TCHAR **argv);

  /**
   * This method creates a POA from the root_poa with emphasis being
   * on the servant_retention_policy which decides the use of the
   * ServantActivator or ServantLocator interfaces. The
   * servent_retention_policy value is 1 for the RETAIN policy and 0
   * for the NONRETAIN policy.
   */
  PortableServer::POA_ptr create_poa (const char* name,
                                      int servant_retention_policy);

  /// A ServantActivator object is created and initialised.
  int create_activator (PortableServer::POA_var first_poa);

  /// A ServantActivator object is created and initialised.
  int create_locator (PortableServer::POA_var second_poa);

  /// The server is executed.
  int run (void);

private:
  /// Parses the input arguments.
  int parse_args (int argc, ACE_TCHAR **argv);

  /// The IORs are written to a file for future use.
  int write_iors_to_file (const char *first_ior,
                          const char *second_ior);

  /// Default ior file.
  ACE_TCHAR *ior_output_file_;

  /// The orb pointer.
  CORBA::ORB_var orb_;

  /// The poa policicies.
  CORBA::PolicyList policies_;

  /// The root_poa which is used for cretaing different child poas.
  PortableServer::POA_var root_poa_;

  /// The poa_manager object.
  PortableServer::POAManager_var poa_manager_;

  /// The object pointer used by the ServantActivator.
  CORBA::Object_var first_test_;

  /// The object pointer used by the Servant Locator.
  CORBA::Object_var second_test_;

  /// The servant activator object.
  ServantActivator_i *servant_activator_impl_;

  /// The servant locator object.
  ServantLocator_i *servant_locator_impl_;
};

#endif /* SERVER_MANAGER_H */
