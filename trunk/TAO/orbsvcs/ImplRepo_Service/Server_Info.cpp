// $Id$
#include "Server_Info.h"

#if 0
Server_Info::Server_Info (const ACE_CString& serverId,
                          const ACE_CString& pname,
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
  , poa_name (pname)
  , is_jacorb (jacorbs)
  , key_name ()
  , activator (aname)
  , cmdline( cmdline)
  , env_vars (env)
  , dir (working_dir)
  , activation_mode (amode)
  , partial_ior (partial_ior)
  , ior (server_ior)
  , server(ImplementationRepository::ServerObject::_duplicate (svrobj))
  , peers ()
  , peer_count (0)
  , alt_key ()
  , start_count (0)
  , waiting_clients (0)
  , starting (false)
  , pid (0)
{
  Server_Info::gen_key (this->server_id, this->poa_name, this->key_name);
  this->start_limit (limit);
}
#endif

Server_Info::Server_Info (void)
{
  this->clear ();
}

Server_Info::Server_Info (const Server_Info &other)
{
  server_id = other.server_id;
  poa_name = other.poa_name;
  is_jacorb = other.is_jacorb;
  key_name = other.key_name;
  activator = other.activator;
  cmdline = other.cmdline;
  dir = other.dir;
  activation_mode = other.activation_mode;
  start_limit_ = other.start_limit_;
  partial_ior = other.partial_ior;
  ior = other.ior;
  last_ping = other.last_ping;
  server = other.server;
  alt_key = other.alt_key;
  start_count = other.start_count;
  waiting_clients = other.waiting_clients;
  starting = other.starting;
  pid = other.pid;
  peers.length (other.peers.length());
  for (size_t i = 0; i < other.peers.length(); i++)
    {
      peers[i] = other.peers[i];
    }
  env_vars.length (other.env_vars.length());
  for (size_t i = 0; i < other.env_vars.length(); i++)
    {
      env_vars[i].name = other.env_vars[i].name;
      env_vars[i].value = other.env_vars[i].value;
    }

}


Server_Info::Server_Info (const ACE_CString& fqname,
                          const ACE_CString& aname,
                          const ACE_CString& cmdline,
                          const ImplementationRepository::EnvironmentList& env,
                          const ACE_CString& working_dir,
                          ImplementationRepository::ActivationMode amode,
                          int limit,
                          const ACE_CString& partial_ior,
                          const ACE_CString& server_ior,
                          ImplementationRepository::ServerObject_ptr svrobj)
  : server_id ()
  , poa_name ()
  , is_jacorb (false)
  , key_name ()
  , activator (aname)
  , cmdline( cmdline)
  , env_vars (env)
  , dir (working_dir)
  , activation_mode (amode)
  , partial_ior (partial_ior)
  , ior (server_ior)
  , server(ImplementationRepository::ServerObject::_duplicate (svrobj))
  , peers ()
  , alt_key ()
  , start_count (0)
  , waiting_clients (0)
  , starting (false)
  , pid (0)
{
  this->is_jacorb = parse_id (fqname.c_str(), this->server_id, this->poa_name);
  Server_Info::gen_key (this->server_id, this->poa_name, this->key_name);

  this->start_limit (limit);
}

Server_Info::Server_Info (const ACE_CString& serverId,
                          const ACE_CString& pname,
                          bool jacorb,
                          const ACE_CString& alt)
  : server_id (serverId)
  , poa_name (pname)
  , is_jacorb (jacorb)
  , key_name ()
  , alt_key (alt)
{
  Server_Info::gen_key (this->server_id, this->poa_name, this->key_name);
}

void
Server_Info::start_limit (int lim)
{
  this->start_limit_ =
    lim < 0 ? -lim : (lim == 0 ? 1 : lim);
}

bool
Server_Info::parse_id (const char* id,
                       ACE_CString& server_id,
                       ACE_CString& pname)
{
  pname = id;
  server_id.clear ();
  size_t idx = pname.find (':');
  if (idx == ACE_CString::npos)
    {
      return false;
    }
  server_id = pname.substr (0, idx);
  pname = pname.substr (idx + 1);
  if (server_id != "JACORB")
    {
      return false;
    }
  idx = pname.find ('/');
  if (idx == ACE_CString::npos)
    {
      server_id.clear ();
    }
  else
    {
      server_id = pname.substr (0, idx);
      pname = pname.substr (idx + 1);
    }
  return true;
}

void
Server_Info::gen_key (const ACE_CString& serverId,
                      const ACE_CString& poa_name,
                      ACE_CString &key)
{
  if (serverId.length () > 0)
    {
      key = serverId + ":" + poa_name;
    }
  else
    {
      key = poa_name;
    }
}

void
Server_Info::fqname_to_key (const char * fqname, ACE_CString& key)
{
  ACE_CString serverId;
  ACE_CString poa_name;

  Server_Info::parse_id (fqname, serverId, poa_name);
  Server_Info::gen_key (serverId, poa_name, key);
}

bool
Server_Info::has_peer (const char *name)
{
  return (this->key_name == name || this->poa_name == name);
}

bool
Server_Info::is_server (const char *name)
{
  for (size_t i = 0; i < peers.length(); i++)
    {
      if (ACE_OS::strcmp (peers[i], name) == 0)
        return true;
    }
  return false;
}

void
Server_Info::setImRInfo (ImplementationRepository::ServerInformation* info) const
{
  info->startup.command_line = cmdline.c_str ();
  if (this->is_jacorb)
    {
      ACE_CString jacorb_name (ACE_TEXT ("JACORB:") + poa_name);
      info->server = jacorb_name.c_str();
    }
  else
    info->server = this->poa_name.c_str();

  info->startup.command_line = cmdline.c_str ();
  info->startup.environment = env_vars;
  info->startup.working_directory = dir.c_str ();
  info->startup.activation = activation_mode;
  info->startup.activator = activator.c_str ();
  info->startup.start_limit =
    start_count >= start_limit_ ? -start_limit_ : start_limit_;
  info->partial_ior = partial_ior.c_str();
  info->activeStatus = ImplementationRepository::ACTIVE_MAYBE;
}

ImplementationRepository::ServerInformation*
Server_Info::createImRServerInfo (void) const
{
  ImplementationRepository::ServerInformation* info;
  ACE_NEW_THROW_EX (info,
                    ImplementationRepository::ServerInformation,
                    CORBA::NO_MEMORY ());
  this->setImRInfo (info);

  return info;
}

void
Server_Info::clear (void)
{
  server_id = "";
  poa_name = "";
  is_jacorb = false;
  key_name = "";
  activator = "";
  cmdline = "";
  env_vars.length (0);
  dir = "";
  activation_mode = ImplementationRepository::NORMAL;
  start_limit_ = 0;
  partial_ior = "";
  ior = "";
  last_ping = ACE_Time_Value::zero;
  server = ImplementationRepository::ServerObject::_nil ();
  peers.length (0);
  alt_key = "";
  start_count  = 0;
  waiting_clients = 0;
  starting = false;
  pid = 0;
}

void
Server_Info::reset_runtime (void)
{
  this->ior = "";
  this->partial_ior = "";
  this->pid = 0;
  this->last_ping = ACE_Time_Value::zero;
  this->server = ImplementationRepository::ServerObject::_nil ();
  // start_count = 0; Note : We can't do this, because it would
  // be reset during startup.
}
