// $Id$

#include "Locator_Repository.h"
#include "utils.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_ctype.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Vector_T.h"

Locator_Repository::Locator_Repository ()
: debug_ (0)
{
}

ACE_CString
Locator_Repository::lcase (const ACE_CString& s)
{
  ACE_CString ret(s);
  for (size_t i = 0; i < ret.length (); ++i)
    {
      ret[i] = static_cast<char>(ACE_OS::ace_tolower (s[i]));
    }
  return ret;
}

int
Locator_Repository::unregister_if_address_reused (
  const ACE_CString& server_id,
  const ACE_CString& name,
  const char* partial_ior)
{
  if (this->debug_ > 0)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t)ImR: checking reuse address ")
      ACE_TEXT ("for server \"%C %C\" ior \"%C\"\n"),
      server_id.c_str(), name.c_str (), partial_ior));
  }

  ACE_Vector<ACE_CString> srvs;

  Locator_Repository::SIMap::ENTRY* sientry = 0;
  Locator_Repository::SIMap::ITERATOR siit (servers ());
  for (; siit.next (sientry); siit.advance() )
  {
    Server_Info_Ptr& info = sientry->int_id_;

    if (this->debug_)
    {
      ACE_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t)ImR: iterating - registered server")
        ACE_TEXT ("\"%C %C\" ior \"%C\"\n"), info->server_id.c_str(),
        info->name.c_str (), info->partial_ior.c_str ()));
    }

    if (info->partial_ior == partial_ior
      && name != info->name
      && info->server_id != server_id)
    {
      if (this->debug_)
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("(%P|%t)ImR: reuse address %C so remove server %C \n"),
          info->partial_ior.c_str (), info->name.c_str ()));
      }
      if (! info->name.empty ())
      {
        srvs.push_back (info->name);
      }
    }
  }

  int err = 0;
  for (size_t i = 0; i < srvs.size (); ++i)
  {

    if (this->remove_server (srvs[i]) != 0)
    {
      err = -1;
    }
  }

  return err;
}

int
Locator_Repository::add_server (
  const ACE_CString& server_id,
  const ACE_CString& name,
  const ACE_CString& aname,
  const ACE_CString& startup_command,
  const ImplementationRepository::EnvironmentList& env_vars,
  const ACE_CString& working_dir,
  ImplementationRepository::ActivationMode activation,
  int start_limit,
  const ACE_CString& partial_ior,
  const ACE_CString& ior,
  ImplementationRepository::ServerObject_ptr svrobj)
{
  int err = sync_load (name, SYNC_ADD, false);
  if (err != 0)
    {
      return err;
    }

  int limit = start_limit < 1 ? 1 : start_limit;
  Server_Info_Ptr info(new Server_Info (server_id, name, aname,
    startup_command, env_vars, working_dir, activation, limit, partial_ior,
    ior, svrobj));

  err = servers ().bind (name, info);
  if (err != 0)
    {
      return err;
    }
  this->persistent_update(info, true);
  return 0;
}

int
Locator_Repository::add_activator (const ACE_CString& name,
                        const CORBA::Long token,
                        const ACE_CString& ior,
                        ImplementationRepository::Activator_ptr act)
{
  int err = sync_load (name, SYNC_ADD, true);
  if (err != 0)
    {
      return err;
    }

  Activator_Info_Ptr info (new Activator_Info (name, token, ior, act));

  err = activators ().bind (lcase (name), info);
  if (err != 0)
    {
      return err;
    }
  this->persistent_update(info, true);
  return 0;
}

int
Locator_Repository::update_server (const Server_Info_Ptr& info)
{
  return this->persistent_update(info, false);
}

int
Locator_Repository::update_activator (const Activator_Info_Ptr& info)
{
  return this->persistent_update(info, false);
}

Server_Info_Ptr
Locator_Repository::get_server (const ACE_CString& name)
{
  Server_Info_Ptr server (0);
  servers ().find (name, server);
  return server;
}

Activator_Info_Ptr
Locator_Repository::get_activator (const ACE_CString& name)
{
  Activator_Info_Ptr activator (0);
  activators ().find (lcase (name), activator);
  return activator;
}

bool
Locator_Repository::has_activator (const ACE_CString& name)
{
  Activator_Info_Ptr activator (0);
  return activators().find (lcase (name), activator) == 0;
}

int
Locator_Repository::remove_server (const ACE_CString& name)
{
  int err = sync_load (name, SYNC_REMOVE, false);
  if (err != 0)
    {
      return err;
    }

  int ret = this->servers().unbind (name);
  if (ret != 0)
    {
      return ret;
    }

  return persistent_remove(name, false);
}

int
Locator_Repository::remove_activator (const ACE_CString& name)
{
  int err = sync_load (name, SYNC_REMOVE, true);
  if (err != 0)
    {
      return err;
    }

  int ret = activators().unbind (lcase(name));
  if (ret != 0)
    {
      return ret;
    }

  return persistent_remove(name, true);
}

Locator_Repository::SIMap&
Locator_Repository::servers (void)
{
  return server_infos_;
}

const Locator_Repository::SIMap&
Locator_Repository::servers (void) const
{
  return server_infos_;
}

Locator_Repository::AIMap&
Locator_Repository::activators (void)
{
  return activator_infos_;
}

const Locator_Repository::AIMap&
Locator_Repository::activators (void) const
{
  return activator_infos_;
}

void
Locator_Repository::debug(int debug)
{
  this->debug_ = debug;
}

int
Locator_Repository::persistent_load ()
{
  // nothing more to do for default load
  return 0;
}

int
Locator_Repository::sync_load (const ACE_CString& , SyncOp , bool )
{
  // nothing more to do for default server/activator load
  return 0;
}

int
Locator_Repository::persistent_update (const Server_Info_Ptr& , bool )
{
  // nothing more to do for default update
  return 0;
}

int
Locator_Repository::persistent_update(const Activator_Info_Ptr& , bool )
{
  // nothing more to do for default update
  return 0;
}

int
Locator_Repository::persistent_remove(const ACE_CString& , bool )
{
  // nothing more to do for default update
  return 0;
}

void
Locator_Repository::notify_updated_server(
  const ImplementationRepository::ServerUpdate& )
{
  // default is Backing store doesn't support replicated updates
}

void
Locator_Repository::notify_updated_activator(
  const ImplementationRepository::ActivatorUpdate& )
{
  // default is Backing store doesn't support replicated updates
}

void
Locator_Repository::register_replica(
  ImplementationRepository::UpdatePushNotification_ptr ,
  ImplementationRepository::SequenceNum_out ,
  ImplementationRepository::SequenceNum )
{
  ACE_ERROR((LM_INFO,
             ACE_TEXT ("ERROR: ImR Locator not configured for replication ")
             ACE_TEXT ("(repo mode=%s)\n"), repo_mode()));
}

No_Backing_Store::~No_Backing_Store()
{
}

const char*
No_Backing_Store::repo_mode() const
{
  return "Disabled";
}
