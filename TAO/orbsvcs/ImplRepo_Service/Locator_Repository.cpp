// $Id$

#include "orbsvcs/Log_Macros.h"
#include "Locator_Repository.h"
#include "utils.h"
#include "tao/ORB_Core.h"
#include "tao/default_ports.h"
#include "ace/Read_Buffer.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_ctype.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Vector_T.h"

Locator_Repository::Locator_Repository (const Options& opts,
                                        CORBA::ORB_ptr orb)
: opts_ (opts),
  orb_(CORBA::ORB::_duplicate(orb)),
  registered_(false)
{
}

Locator_Repository::~Locator_Repository ()
{
  teardown_multicast();
}

int
Locator_Repository::init (PortableServer::POA_ptr root_poa,
                          PortableServer::POA_ptr imr_poa,
                          const char* this_ior)
{
  this->imr_ior_ = this_ior;
  int err = init_repo(imr_poa);
  if (err != 0)
    {
      return err;
    }

  // Activate the two poa managers
  PortableServer::POAManager_var poaman =
    root_poa->the_POAManager ();
  poaman->activate ();
  poaman = imr_poa->the_POAManager ();
  poaman->activate ();

  err = report_ior(imr_poa);
  return err;
}

int
Locator_Repository::report_ior (PortableServer::POA_ptr )
{
  if (this->registered_)
    {
      ORBSVCS_ERROR_RETURN ((LM_ERROR,
        ACE_TEXT ("ERROR: Repository already reported IOR\n")), -1);
    }

  if (this->opts_.debug () > 0)
    {
      ORBSVCS_DEBUG ((LM_INFO, ACE_TEXT ("report_ior <%C>\n"),
        this->imr_ior_.in ()));
    }

  // Register the ImR for use with INS
  CORBA::Object_var obj = this->orb_->resolve_initial_references ("IORTable");
  IORTable::Table_var ior_table = IORTable::Table::_narrow (obj.in ());
  ACE_ASSERT (! CORBA::is_nil (ior_table.in ()));

  ior_table->rebind ("ImplRepoService", this->imr_ior_.in());
  ior_table->rebind ("ImR", this->imr_ior_.in());

  // Set up multicast support (if enabled)
  if (this->opts_.multicast ())
    {
      ACE_Reactor* reactor = this->orb_->orb_core ()->reactor ();
      if (this->setup_multicast (reactor, this->imr_ior_.in ()) != 0)
        return -1;
    }

  // We write the ior file last so that the tests can know we are ready.
  if (this->opts_.ior_filename ().length () > 0)
    {
      FILE* orig_fp = ACE_OS::fopen(this->opts_.ior_filename ().c_str(),
                                    ACE_TEXT("r"));

      bool write_data = true;
      if (orig_fp != 0)
        {
          ACE_Read_Buffer reader (orig_fp, false);

          char* string = reader.read ();

          if (string != 0)
            {
              write_data =
                (ACE_OS::strcasecmp (string, this->imr_ior_.in ()) != 0);
              reader.alloc ()->free (string);
            }
          ACE_OS::fclose (orig_fp);
        }

      if (write_data)
        {
          FILE* fp = ACE_OS::fopen (this->opts_.ior_filename ().c_str (),
                                    ACE_TEXT("w"));
          if (fp == 0)
            {
              ORBSVCS_ERROR_RETURN ((LM_ERROR,
                ACE_TEXT("ImR: Could not open file: %s\n"),
                this->opts_.ior_filename ().c_str ()), -1);
            }
          ACE_OS::fprintf (fp, "%s", this->imr_ior_.in ());
          ACE_OS::fclose (fp);
        }
    }

  registered_ = true;

  return 0;
}

int
Locator_Repository::recover_ior (void)
{
  if (this->registered_)
    {
      ORBSVCS_ERROR_RETURN ((LM_ERROR,
         ACE_TEXT ("ERROR: Repository already registered IOR. ")
         ACE_TEXT ("recover_ior should not be called.\n")), -1);
    }

  if (this->opts_.debug () > 0)
    {
      ORBSVCS_DEBUG ((LM_INFO, ACE_TEXT ("recover_ior <%C>\n"),
                  this->opts_.ior_filename ().c_str()));
    }

  // Load the IOR from the specified file if it is available.
  const ACE_TString& combined_ior_file = this->opts_.ior_filename ();

  // Check if the file exists. If not, then return 1 indicating
  // we cannot recover our state.
  if (ACE_OS::access (combined_ior_file.c_str (), F_OK) != 0)
    return -1;

  try {
    ACE_TString combined_ior = "file://" + combined_ior_file;

    CORBA::Object_var combined_obj =
      this->orb_->string_to_object (combined_ior.c_str());

    if (!CORBA::is_nil (combined_obj.in ()))
      {
        // Convert the object back into an IOR string to store in the
        // imr_ior_ attribute.
        this->imr_ior_ = this->orb_->object_to_string (combined_obj.in ());
      }
  }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Attempting to read combined_ior for ImR_Locator\n");
      return -1;
    }

  // Register the ImR for use with INS
  CORBA::Object_var obj = this->orb_->resolve_initial_references ("IORTable");
  IORTable::Table_var ior_table = IORTable::Table::_narrow (obj.in ());
  ACE_ASSERT (! CORBA::is_nil (ior_table.in ()));

  ior_table->bind ("ImplRepoService", this->imr_ior_.in());
  ior_table->bind ("ImR", this->imr_ior_.in());

  // Set up multicast support (if enabled)
  if (this->opts_.multicast ())
    {
      ACE_Reactor* reactor = this->orb_->orb_core ()->reactor ();
      if (this->setup_multicast (reactor, this->imr_ior_.in ()) != 0)
        return -1;
    }

  registered_ = true;

  return 0;
}

int
Locator_Repository::setup_multicast (ACE_Reactor* reactor, const char* ior)
{
  ACE_ASSERT (reactor != 0);
#if defined (ACE_HAS_IP_MULTICAST)

  TAO_ORB_Core* core = TAO_ORB_Core_instance ();
  // See if the -ORBMulticastDiscoveryEndpoint option was specified.
  ACE_CString mde (core->orb_params ()->mcast_discovery_endpoint ());

  if (mde.length () != 0)
    {
      if (this->ior_multicast_.init (ior,
                                     mde.c_str (), TAO_SERVICEID_IMPLREPOSERVICE) == -1)
        {
          return -1;
        }
    }
  else
    {
      // Port can be specified as param, env var, or default
      CORBA::UShort port =
        core->orb_params ()->service_port (TAO::MCAST_IMPLREPOSERVICE);
      if (port == 0)
        {
          // Check environment var. for multicast port.
          const char* port_number = ACE_OS::getenv ("ImplRepoServicePort");

          if (port_number != 0)
            port = static_cast<CORBA::UShort> (ACE_OS::atoi (port_number));
        }
      if (port == 0)
        port = TAO_DEFAULT_IMPLREPO_SERVER_REQUEST_PORT;

      if (this->ior_multicast_.init (ior, port,
                                     ACE_DEFAULT_MULTICAST_ADDR, TAO_SERVICEID_IMPLREPOSERVICE) == -1)
        {
          return -1;
        }
    }

  // Register event handler for the ior multicast.
  if (reactor->register_handler (&this->ior_multicast_,
                                 ACE_Event_Handler::READ_MASK) == -1)
    {
      if (this->opts_.debug() > 0)
        ORBSVCS_DEBUG ((LM_DEBUG, "ImR: cannot register Event handler\n"));
      return -1;
    }
#else /* ACE_HAS_IP_MULTICAST*/
  ACE_UNUSED_ARG (reactor);
  ACE_UNUSED_ARG (ior);
#endif /* ACE_HAS_IP_MULTICAST*/
  return 0;
}

void
Locator_Repository::teardown_multicast ()
{
  ACE_Reactor* r = ior_multicast_.reactor ();
  if (r != 0) {
    r->remove_handler (&ior_multicast_, ACE_Event_Handler::READ_MASK);
    ior_multicast_.reactor (0);
  }
}

bool
Locator_Repository::multicast() const
{
  return this->ior_multicast_.reactor () != 0;
}

ACE_CString
Locator_Repository::lcase (const ACE_CString& s)
{
  ACE_CString ret(s);
  for (size_t i = 0; i < ret.length (); ++i)
    {
      ret[i] = static_cast<char> (ACE_OS::ace_tolower (s[i]));
    }
  return ret;
}

int
Locator_Repository::unregister_if_address_reused (
  const ACE_CString& server_id,
  const ACE_CString& name,
  const char* partial_ior)
{
  if (this->opts_.debug() > 0)
  {
    ORBSVCS_DEBUG ((LM_DEBUG,
      ACE_TEXT ("(%P|%t)ImR: checking reuse address ")
      ACE_TEXT ("for server \"%C %C\" ior \"%C\"\n"),
      server_id.c_str(),
      name.c_str (),
      partial_ior));
  }

  ACE_Vector<ACE_CString> srvs;

  Locator_Repository::SIMap::ENTRY* sientry = 0;
  Locator_Repository::SIMap::ITERATOR siit (servers ());
  for (; siit.next (sientry); siit.advance() )
  {
    Server_Info_Ptr& info = sientry->int_id_;

    if (this->opts_.debug() > 0)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
        ACE_TEXT ("(%P|%t)ImR: iterating - registered server")
        ACE_TEXT ("\"%C %C\" ior \"%C\"\n"), info->server_id.c_str(),
        info->name.c_str (), info->partial_ior.c_str ()));
    }

    if (info->partial_ior == partial_ior
      && name != info->name
      && info->server_id != server_id)
    {
      if (this->opts_.debug() > 0)
      {
        ORBSVCS_DEBUG ((LM_DEBUG,
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
  bool jacorbs,
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
  int err = sync_load ();
  if (err != 0)
    {
      return err;
    }

  int limit = start_limit < 1 ? 1 : start_limit;
  Server_Info_Ptr info(new Server_Info (server_id, name, jacorbs, aname,
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
  int err = sync_load ();
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
  sync_load ();

  Server_Info_Ptr server (0);
  servers ().find (name, server);
  return server;
}

Activator_Info_Ptr
Locator_Repository::get_activator (const ACE_CString& name)
{
  sync_load ();

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
  int err = sync_load ();
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
  int err = sync_load ();
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

int
Locator_Repository::sync_load ()
{
  // nothing more to do for default server/activator load
  return 0;
}

bool
Locator_Repository::registered () const
{
  return this->registered_;
}

UpdateableServerInfo::UpdateableServerInfo (
  Locator_Repository* repo, const ACE_CString& name)
: repo_(repo),
  si_(repo->get_server (name)),
  needs_update_(false)
{
}

UpdateableServerInfo::UpdateableServerInfo (Locator_Repository* repo,
                                            const Server_Info_Ptr& si,
                                            bool reset_start_count)
: repo_(repo),
  si_(si),
  needs_update_(false)
{
  if (reset_start_count)
    {
      needs_update_ = repo_ != 0;
      si_->start_count = 0;
    }
}

UpdateableServerInfo::UpdateableServerInfo (const Server_Info& si)
: repo_(0),
  si_(new Server_Info(si)),
  needs_update_(false)
{
}

UpdateableServerInfo::~UpdateableServerInfo ()
{
  update_repo();
}

void
UpdateableServerInfo::update_repo ()
{
  if (!needs_update_)
    return;

  needs_update_ = false;
  int err = repo_->update_server (si_);
  ACE_ASSERT (err == 0);
  ACE_UNUSED_ARG (err);
}

const Server_Info*
UpdateableServerInfo::operator-> () const
{
  return si_.get();
}

const Server_Info&
UpdateableServerInfo::operator* () const
{
  return *(si_.get());
}


const Server_Info_Ptr&
UpdateableServerInfo::edit ()
{
  needs_update_ = repo_ != 0;
  return si_;
}

void
UpdateableServerInfo::needs_update ()
{
  needs_update_ = true;
}

bool
UpdateableServerInfo::null() const
{
  return si_.null();
}

No_Backing_Store::No_Backing_Store (const Options& opts,
                                    CORBA::ORB_ptr orb)
  : Locator_Repository(opts, orb)
{
}

No_Backing_Store::~No_Backing_Store ()
{
}

const ACE_TCHAR*
No_Backing_Store::repo_mode () const
{
  return ACE_TEXT ("Disabled");
}

int
No_Backing_Store::init_repo (PortableServer::POA_ptr )
{
  // nothing more to do for no backing store init
  return 0;
}

int
No_Backing_Store::persistent_update (const Server_Info_Ptr& , bool )
{
  // nothing more to do for no backing store update
  return 0;
}

int
No_Backing_Store::persistent_update (const Activator_Info_Ptr& , bool )
{
  // nothing more to do for no backing store update
  return 0;
}

int
No_Backing_Store::persistent_remove (const ACE_CString& , bool )
{
  // nothing more to do for no backing store remove
  return 0;
}

