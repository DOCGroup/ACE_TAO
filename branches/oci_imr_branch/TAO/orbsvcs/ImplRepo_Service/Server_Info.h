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

#include "ace/Refcounted_Auto_Ptr.h"
//#include "XML_ContentHandler.h"
#include "ace/SString.h"

#include "tao/PortableServer/ImplRepoC.h"

/**
 * @brief Information about IMR registered servers.
 */
struct Server_Info
{
  Server_Info (const ACE_CString& server_name,
               const ACE_CString& startup_command,
               const ImplementationRepository::EnvironmentList& environment_vars,
               const ACE_CString& working_dir,
               ImplementationRepository::ActivationMode activation,
               const ACE_CString& server_partial_ior = ACE_CString(""),
               const ACE_CString& server_ior = ACE_CString(""))
               : starting_up(false)
               , name(server_name)
               , cmdline(startup_command)
               , env_vars(environment_vars)
               , dir(working_dir)
               , activation_mode(activation)
               , partial_ior(server_partial_ior)
               , ior(server_ior)
  {
  }
  /// This is a flag to determine if the process has already been spawned
  /// and we are just waiting for it to start up.
  bool starting_up;
  /// The name of the POA.
  ACE_CString name;
  /// The command line startup command (program and arguments).
  ACE_CString cmdline;
  /// Environment Variables.
  ImplementationRepository::EnvironmentList env_vars;
  /// The working directory.
  ACE_CString dir;
  /// The type of activation this supports.
  ImplementationRepository::ActivationMode activation_mode;
  /// Current endpoint used by the server.
  ACE_CString partial_ior;
  /// IOR of the server object in the server.
  ACE_CString ior;
};

typedef ACE_Refcounted_Auto_Ptr<Server_Info, ACE_Null_Mutex> Server_Info_Ptr;

#endif /* SERVER_INFO_H */
