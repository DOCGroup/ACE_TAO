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
 ImplementationRepository::ServerObject_ptr svrobj)
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
 , start_count (0)
 , waiting_clients (0)
 , starting (false)
{
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
