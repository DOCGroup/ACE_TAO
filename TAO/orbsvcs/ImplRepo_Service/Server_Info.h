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
#ifndef SERVER_INFO_H
#define SERVER_INFO_H

#include "ace/Bound_Ptr.h"

#include "ImplRepoC.h"

#include "ace/SString.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
* @brief Information about IMR registered servers.
*/
struct Server_Info
{
  Server_Info (const ACE_CString& serverId,
    const ACE_CString& name,
    bool jacorbs,
    const ACE_CString& aname,
    const ACE_CString& cmdline,
    const ImplementationRepository::EnvironmentList& env,
    const ACE_CString& working_dir,
    ImplementationRepository::ActivationMode amode,
    int start_limit,
    const ACE_CString& partial_ior = ACE_CString(""),
    const ACE_CString& server_ior = ACE_CString(""),
    ImplementationRepository::ServerObject_ptr svrobj = ImplementationRepository::ServerObject::_nil()
    );

  /// Convert to the corba type
  ImplementationRepository::ServerInformation* createImRServerInfo(void) const;

  void reset();

  /// The name of the server.
  ACE_CString server_id;
  ACE_CString name;
  bool jacorb_server;

  /// The name of the activator in which this server runs
  ACE_CString activator;
  /// The command line startup command (program and arguments).
  ACE_CString cmdline;
  /// Environment Variables.
  ImplementationRepository::EnvironmentList env_vars;
  /// The working directory.
  ACE_CString dir;
  /// The type of activation this supports.
  ImplementationRepository::ActivationMode activation_mode;
  /// Limit of retries to start the server
  int start_limit;
  /// Current endpoint used by the server.
  ACE_CString partial_ior;
  /// IOR of the server object in the server.
  ACE_CString ior;
  /// The timestamp of the last time we verified the server is alive
  ACE_Time_Value last_ping;
  /// The cached server object
  ImplementationRepository::ServerObject_var server;
  int start_count;
  int waiting_clients;
  bool starting;
};

typedef ACE_Strong_Bound_Ptr<Server_Info, ACE_Null_Mutex> Server_Info_Ptr;

#endif /* SERVER_INFO_H */
