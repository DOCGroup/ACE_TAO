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

struct Server_Info;
typedef ACE_Strong_Bound_Ptr<Server_Info, ACE_Null_Mutex> Server_Info_Ptr;

/**
* @brief Information about IMR registered servers.
*/
struct Server_Info
{
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
               ImplementationRepository::ServerObject_ptr svrobj = ImplementationRepository::ServerObject::_nil());

  Server_Info (const ACE_CString& serverId,
               const ACE_CString& pname,
               bool jacorb,
               Server_Info_Ptr alt);

  Server_Info & operator= (const Server_Info& other);

  void clear (void);

  /// Convert to the corba type
  void setImRInfo (ImplementationRepository::ServerInformation* info) const;
  ImplementationRepository::ServerInformation* createImRServerInfo(void) const;

  void reset_runtime (void);

  bool is_server (const char *name) const;
  bool has_peer (const char *name) const;
  bool is_mode (ImplementationRepository::ActivationMode m) const;
  bool is_running (void) const;

  // transform the supplied limit to always be at least 1
  void start_limit (int lim);

  void update_options (const ImplementationRepository::StartupOptions &options);
  void set_contact (const char *pior,
                    const char *sior,
                    ImplementationRepository::ServerObject_ptr svrobj);

  Server_Info *active_info (void);
  const Server_Info *active_info (void) const;

  const char * ping_id (void) const;

  static bool parse_id (const char * id,
                        ACE_CString& server_id,
                        ACE_CString& poa_name);

  static void gen_id (const Server_Info *si, ACE_CString& id);

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
  ACE_CString key_name_;

  /// The name of the activator in which this server runs
  ACE_CString activator;
  /// The command line startup command (program and arguments).
  ACE_CString cmdline;
  /// Environment Variables.
  ImplementationRepository::EnvironmentList env_vars;
  /// The working directory.
  ACE_CString dir;
  /// The type of activation this supports.
  ImplementationRepository::ActivationMode activation_mode_;
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
  /// Alternate server info for sharing startup info between linked POAs
  Server_Info_Ptr alt_info_;

  int start_count;
  int waiting_clients;
  bool starting;

  /// Last known process ID reported by the activator
  int pid;
  /// the locator should expect a notification on server death from activator
  bool death_notify;
};

#endif /* SERVER_INFO_H */
