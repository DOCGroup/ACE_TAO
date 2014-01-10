// $Id$
#include "Server_Info.h"

Server_Info::Server_Info
(
 const ACE_CString& serverId,
 const ACE_CString& server_name,
 bool jacorbs,
 const ACE_CString& aname,
 const ACE_CString& cmdline,
 const ImplementationRepository::EnvironmentList& env,
 const ACE_CString& working_dir,
 ImplementationRepository::ActivationMode amode,
 int limit,
 const ACE_CString& partial_ior,
 const ACE_CString& server_ior,
 ImplementationRepository::ServerObject_ptr svrobj,
 const ACE_CString& peer_list)
 : server_id (serverId)
 , name (server_name)
 , jacorb_server( jacorbs)
 , activator (aname)
 , cmdline( cmdline)
 , env_vars (env)
 , dir (working_dir)
 , activation_mode (amode)
 , start_limit (limit)
 , partial_ior (partial_ior)
 , ior (server_ior)
 , server(ImplementationRepository::ServerObject::_duplicate (svrobj))
 , peers (peer_list)
 , peer_count (0)
 , start_count (0)
 , waiting_clients (0)
 , starting (false)
 , pid (0)
{
  if (peers.length() > 0)
    {
      peer_count = 1;
      for (size_t i = peers.find (','); i != ACE_CString::npos;
           i = peers.substr (i+1).find (','))
        {
          ++peer_count;
        }
    }
}

ImplementationRepository::ServerInformation*
Server_Info::createImRServerInfo (void) const
{
  ImplementationRepository::ServerInformation* info;
  ACE_NEW_THROW_EX (info,
                    ImplementationRepository::ServerInformation,
                    CORBA::NO_MEMORY ());

  info->startup.command_line = cmdline.c_str ();
  if (jacorb_server)
    {
      ACE_CString jacorb_name (ACE_TEXT ("JACORB:") + name);
      info->server = jacorb_name.c_str();
    }
  else
    info->server = name.c_str();

  info->startup.command_line = cmdline.c_str ();
  info->startup.environment = env_vars;
  info->startup.working_directory = dir.c_str ();
  info->startup.activation = activation_mode;
  info->startup.activator = activator.c_str ();
  if (start_count >= start_limit)
    {
      info->startup.start_limit = -start_limit;
    }
  else
    {
      info->startup.start_limit = start_limit;
    }
  info->partial_ior = partial_ior.c_str();

  info->activeStatus = ImplementationRepository::ACTIVE_MAYBE;

  return info;
}

void
Server_Info::reset (void)
{
  ior = "";
  partial_ior = "";
  last_ping = ACE_Time_Value::zero;
  server = ImplementationRepository::ServerObject::_nil ();
  // start_count = 0; Note : We can't do this, because it would
  // be reset during startup.
}
