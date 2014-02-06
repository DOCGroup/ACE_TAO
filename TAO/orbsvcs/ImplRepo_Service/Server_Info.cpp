// $Id$
#include "Server_Info.h"

Server_Info::Server_Info (void)
{
  this->clear ();
}

Server_Info::Server_Info (const Server_Info &other)
{
  *this = other;
}

Server_Info&
Server_Info::operator= (const Server_Info &other)
{
  server_id = other.server_id;
  poa_name = other.poa_name;
  is_jacorb = other.is_jacorb;
  key_name_ = other.key_name_;
  activator = other.activator;
  cmdline = other.cmdline;
  dir = other.dir;
  activation_mode_ = other.activation_mode_;
  start_limit_ = other.start_limit_;
  partial_ior = other.partial_ior;
  ior = other.ior;
  last_ping = other.last_ping;
  server = other.server;
  alt_info_ = other.alt_info_;
  start_count = other.start_count;
  waiting_clients = other.waiting_clients;
  starting = other.starting;
  pid = other.pid;
  peers = other.peers;
  env_vars = other.env_vars;
  return *this;
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
  , key_name_ ()
  , activator (aname)
  , cmdline( cmdline)
  , env_vars (env)
  , dir (working_dir)
  , activation_mode_ (amode)
  , partial_ior (partial_ior)
  , ior (server_ior)
  , server(ImplementationRepository::ServerObject::_duplicate (svrobj))
  , peers ()
  , alt_info_ ()
  , start_count (0)
  , waiting_clients (0)
  , starting (false)
  , pid (0)
{
  this->is_jacorb = parse_id (fqname.c_str(), this->server_id, this->poa_name);
  Server_Info::gen_key (this->server_id, this->poa_name, this->key_name_);

  this->start_limit (limit);
}

Server_Info::Server_Info (const ACE_CString& serverId,
                          const ACE_CString& pname,
                          bool jacorb,
                          Server_Info_Ptr alt)
  : server_id (serverId)
  , poa_name (pname)
  , is_jacorb (jacorb)
  , key_name_ ()
  , activator ("")
  , cmdline ("")
  , env_vars ()
  , dir ("")
  , activation_mode_ (ImplementationRepository::NORMAL)
  , start_limit_ (1)
  , partial_ior ("")
  , ior ("")
  , server (ImplementationRepository::ServerObject::_nil())
  , peers ()
  , alt_info_ (alt)
  , start_count (0)
  , waiting_clients (0)
  , starting (false)
  , pid (0)
{
  Server_Info::gen_key (this->server_id, this->poa_name, this->key_name_);
}

Server_Info *
Server_Info::active_info (void)
{
   return this->alt_info_.null() ? this : this->alt_info_.get();
}

const Server_Info *
Server_Info::active_info (void) const
{
   return this->alt_info_.null() ? this : this->alt_info_.get();
}

void
Server_Info::start_limit (int lim)
{
  this->active_info ()->start_limit_ = lim < 0 ? -lim : (lim == 0 ? 1 : lim);
}

bool
Server_Info::is_mode (ImplementationRepository::ActivationMode m) const
{
  return this->active_info ()->activation_mode_ == m;
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
Server_Info::gen_id (const Server_Info * si, ACE_CString &id)
{
  char sep = ':';
  id = "";
  if (si->is_jacorb)
    {
      id = "JACORB:";
      sep = '/';
    }
  if (si->server_id.length() > 0)
    {
      id += si->server_id + sep;
    }
  id += si->poa_name;
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
  return (this->key_name_ == name || this->poa_name == name);
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
  ACE_CString id;
  Server_Info::gen_id (this, id);
  info->server = id.c_str();
  const Server_Info *startup = this->active_info ();
  info->startup.command_line = startup->cmdline.c_str ();
  info->startup.environment = startup->env_vars;
  info->startup.working_directory = startup->dir.c_str ();
  info->startup.activation = startup->activation_mode_;
  info->startup.activator = startup->activator.c_str ();
  info->startup.start_limit =
    startup->start_count >= startup->start_limit_ ? -startup->start_limit_ : startup->start_limit_;
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
  key_name_ = "";
  activator = "";
  cmdline = "";
  env_vars.length (0);
  dir = "";
  activation_mode_ = ImplementationRepository::NORMAL;
  start_limit_ = 0;
  partial_ior = "";
  ior = "";
  last_ping = ACE_Time_Value::zero;
  server = ImplementationRepository::ServerObject::_nil ();
  peers.length (0);
  alt_info_.reset ();
  start_count  = 0;
  waiting_clients = 0;
  starting = false;
  pid = 0;
}

void
Server_Info::reset_runtime (void)
{
  this->partial_ior = "";
  Server_Info *startup = this->active_info ();
  startup->ior = "";
  startup->pid = 0;
  startup->last_ping = ACE_Time_Value::zero;
  startup->server = ImplementationRepository::ServerObject::_nil ();
}

void
Server_Info::update_options (const ImplementationRepository::StartupOptions &options)
{
  Server_Info *startup = this->active_info ();
  startup->activator = options.activator.in ();
  startup->cmdline = options.command_line.in ();
  startup->env_vars = options.environment;
  startup->dir = options.working_directory.in ();
  startup->activation_mode_ = options.activation;
  startup->start_limit (options.start_limit);
  startup->start_count = 0;
}

const char *
Server_Info::ping_id (void) const
{
  return this->active_info ()->key_name_.c_str();
}

void
Server_Info::set_contact (const char *partial_ior,
                          const char *srv_ior,
                          ImplementationRepository::ServerObject_ptr svrobj)
{
  this->partial_ior = partial_ior;
  Server_Info *startup = this->active_info ();
  startup->ior = srv_ior;
  startup->server = ImplementationRepository::ServerObject::_duplicate (svrobj);
}
