/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Server_Info.h
 *
 *  $Id$
 *
 *  This class implements the Server_Info for the Implementation Repository.
 *
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 *  @author Priyanka Gontla <gontla_p@ociweb.com>
 */
//=============================================================================

#ifndef SERVER_INFO_H
#define SERVER_INFO_H

#include "XML_ContentHandler.h"

#include "tao/PortableServer/ImplRepoC.h"

/**
 * @class Server_Info
 *
 * @brief Information about IMR registered servers.
 *
 * Contains all the necessary information about the server including
 * Information on how to start it up and where it is running.
 */
class Server_Info
{
public:
  // = Constructors

  /// Initialize the command_line and working_dir.
  Server_Info (const ACE_CString POA_name,
               const ACE_CString logical_server_name,
               const ACE_CString startup_command,
               const ImplementationRepository::EnvironmentList
                     environment_vars,
               const ACE_CString working_dir,
               const ImplementationRepository::ActivationMode activation);

  // = Destructors

  /// The only destructor there is.
  ~Server_Info (void);

  /// Updates information that is relevant only when an instance
  /// of the server is running.
  void update_running_info (const ACE_CString location,
                            const ACE_CString server_object_ior);

  /// Returns startup information.
  void get_startup_info (ACE_CString &logical_server_name,
                         ACE_CString &startup_command,
                         ImplementationRepository::EnvironmentList
                            &environment_vars,
                         ACE_CString &working_dir,
                         ImplementationRepository::ActivationMode
                         &activation);

  /// Returns information about a running instance.
  void get_running_info (ACE_CString &location,
                         ACE_CString &server_object_ior);

  // ActivationMode get_activation (void);
  // Get the activation mode.

  // @@ Does this belong here?
  //  int startup ();
  // Starts up the server based on the information.
  // Returns:  0  if successful
  //           -1 if there is no registration command (it has to be manually
  //              restarted)

  /// This is a flag to determine if the process has already been spawned
  /// and we are just waiting for it to start up.
  int starting_up_;

private:
  /// Which server process this poa is grouped in.
  ACE_CString logical_server_name_;

  /// The name of the POA.
  ACE_CString POA_name_;

  /// The command line startup command (program and arguments).
  ACE_CString startup_command_;

  /// Environment Variables.
  ImplementationRepository::EnvironmentList environment_vars_;

  /// The working directory.
  ACE_CString working_dir_;

  /// Current endpoint used by the server.
  ACE_CString location_;

  /// IOR of the server object in the server.
  ACE_CString server_object_ior_;

  /// The type of activation this supports.
  ImplementationRepository::ActivationMode activation_;

  // No copying allowed.
  void operator= (Server_Info &);
  Server_Info (Server_Info &);
};

#endif /* SERVER_INFO_H */
