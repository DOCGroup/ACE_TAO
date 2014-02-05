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
#if 0
  Server_Info (const ACE_CString& serverId,
               const ACE_CString& pname,
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
#endif

  Server_Info (void);
  Server_Info (const Server_Info & other);

  Server_Info (const ACE_CString& fqname,
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

  Server_Info (const ACE_CString& serverId,
               const ACE_CString& pname,
               bool jacorb,
               const ACE_CString& alt);

  void clear (void);

  /// Convert to the corba type
  void setImRInfo (ImplementationRepository::ServerInformation* info) const;
  ImplementationRepository::ServerInformation* createImRServerInfo(void) const;

  void reset_runtime (void);

  bool is_server (const char *name);
  bool has_peer (const char *name);

  // transform the supplied limit to always be at least 1
  void start_limit (int lim);

  static bool parse_id (const char * id,
                        ACE_CString& server_id,
                        ACE_CString& poa_name);

  static void gen_key (const ACE_CString& server_id,
                       const ACE_CString& poa_name,
                       ACE_CString& key);

  static void fqname_to_key (const char * fqname, ACE_CString& key);

  /// The name of the server instance.
  ACE_CString server_id;
  /// the name of the poa
  ACE_CString poa_name;
  /// true if the server is JacORB
  bool is_jacorb;

  /// The fully qualified name of this info, combines the serverID and POA name
  ACE_CString key_name;

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
  int start_limit_;
  /// Current endpoint used by the server.
  ACE_CString partial_ior;
  /// IOR of the server object in the server.
  ACE_CString ior;
  /// The timestamp of the last time we verified the server is alive
  ACE_Time_Value last_ping;
  /// The cached server object
  ImplementationRepository::ServerObject_var server;
  /// A comma separated list of additional POAs bound to this server
  CORBA::StringSeq peers;
  /// The key used to look up the full server entry. Linked POAs have
  /// the same key
  ACE_CString alt_key;

  int start_count;
  int waiting_clients;
  bool starting;
  int pid;
};

typedef ACE_Strong_Bound_Ptr<Server_Info, ACE_Null_Mutex> Server_Info_Ptr;

#endif /* SERVER_INFO_H */
