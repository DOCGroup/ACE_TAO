#include "orbsvcs/Log_Macros.h"
#include "Config_Backing_Store.h"
#include "Server_Info.h"
#include "Activator_Info.h"
#include "utils.h"
#include "ace/OS_NS_unistd.h"

static const ACE_TCHAR* STARTUP_COMMAND = ACE_TEXT("StartupCommand");
static const ACE_TCHAR* WORKING_DIR = ACE_TEXT("WorkingDir");
static const ACE_TCHAR* ENVIRONMENT = ACE_TEXT("Environment");
static const ACE_TCHAR* ACTIVATION = ACE_TEXT("Activation");
static const ACE_TCHAR* PARTIAL_IOR = ACE_TEXT("Location");
static const ACE_TCHAR* IOR = ACE_TEXT("IOR");
static const ACE_TCHAR* START_LIMIT = ACE_TEXT("StartLimit");
static const ACE_TCHAR* ACTIVATOR = ACE_TEXT("Activator");
static const ACE_TCHAR* SERVERS_ROOT_KEY = ACE_TEXT("Servers");
static const ACE_TCHAR* ACTIVATORS_ROOT_KEY = ACE_TEXT("Activators");
static const ACE_TCHAR* TOKEN = ACE_TEXT("Token");
static const ACE_TCHAR* SERVER_ID = ACE_TEXT("ServerId");
static const ACE_TCHAR* JACORB_SERVER = ACE_TEXT("JacORBServer");
static const ACE_TCHAR* ALTKEY = ACE_TEXT("AltKey");
static const ACE_TCHAR* POA = ACE_TEXT("POA");
static const ACE_TCHAR* PEERS = ACE_TEXT("Peers");
static const ACE_TCHAR* PID = ACE_TEXT("Pid");
#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_REGISTRY)
static const char* WIN32_REG_KEY = "Software\\TAO\\ImplementationRepository";
#endif

Config_Backing_Store::Config_Backing_Store (const Options& opts,
                                            CORBA::ORB_ptr orb,
                                            ACE_Configuration& config)
: Locator_Repository (opts, orb),
  config_ (config),
  status_ (-1)
{
}

Config_Backing_Store::~Config_Backing_Store ()
{
}

void Config_Backing_Store::loadActivators ()
{
  ACE_Configuration_Section_Key root;
  int err =
    config_.open_section (config_.root_section (), ACTIVATORS_ROOT_KEY, 0, root);
  if (err == 0)
  {
    int index = 0;
    ACE_TString name;
    while (config_.enumerate_sections (root, index, name) == 0)
    {
      u_int token;

      ACE_Configuration_Section_Key key;

      // Can't fail, because we're enumerating
      config_.open_section (root, name.c_str(), 0, key);

      ACE_TString ior;
      config_.get_string_value (key, IOR, ior);
      config_.get_integer_value (key, TOKEN, token);

      Activator_Info* ai;
      ACE_CString name_cstr(ACE_TEXT_ALWAYS_CHAR(name.c_str()));
      ACE_CString ior_cstr(ACE_TEXT_ALWAYS_CHAR(ior.c_str()));

      ACE_NEW(ai, Activator_Info (name_cstr, token, ior_cstr));

      Activator_Info_Ptr info (ai);
      activators ().bind (lcase (name_cstr), info);
      ++index;
    }
  }
}

void
Config_Backing_Store::loadServers ()
{
  ACE_Configuration_Section_Key root;
  int err =
    config_.open_section (config_.root_section (), SERVERS_ROOT_KEY, 0, root);
  if (err == 0)
    {
      int index = 0;
      ACE_TString name;
      Server_Info *si = 0;
      u_int tmp_int = 0;
      ACE_TString tmp;

      while (config_.enumerate_sections (root, index, name) == 0)
        {
          Server_Info_Ptr info;
          ACE_CString name_cstr(ACE_TEXT_ALWAYS_CHAR(name.c_str()));
          if (this->servers ().find (name_cstr, info) != 0)
            {
              ACE_NEW (si, Server_Info);
              info.reset (si);
              si->key_name_ = name_cstr;
            }

          name = ACE_TEXT_CHAR_TO_TCHAR(name_cstr.c_str());

          ACE_Configuration_Section_Key key;

          // Can't fail, because we're enumerating
          config_.open_section (root, name.c_str(), 0, key);
          if (!config_.get_string_value (key, POA, tmp))
          {
            si->poa_name = si->key_name_;
          }
          else
          {
            si->poa_name = ACE_TEXT_ALWAYS_CHAR(tmp.c_str());
          }

          // Ignore any missing values. Server name is enough on its own.
          config_.get_string_value (key, SERVER_ID, tmp);
          si->server_id = ACE_TEXT_ALWAYS_CHAR(tmp.c_str());

          config_.get_string_value (key, JACORB_SERVER, tmp);
          si->is_jacorb = (tmp == ACE_TEXT("1"));
          
          config_.get_string_value (key, ACTIVATOR, tmp);
          si->activator = ACE_TEXT_ALWAYS_CHAR(tmp.c_str());

          config_.get_string_value (key, STARTUP_COMMAND, tmp);
          si->cmdline = ACE_TEXT_ALWAYS_CHAR(tmp.c_str());

          config_.get_string_value (key, WORKING_DIR, tmp);
          si->dir = ACE_TEXT_ALWAYS_CHAR(tmp.c_str());

          config_.get_string_value (key, ENVIRONMENT, tmp);
          ACE_CString tmp_cstr(ACE_TEXT_ALWAYS_CHAR(tmp.c_str()));

          ImR_Utils::stringToEnvList (tmp_cstr, si->env_vars);
          config_.get_integer_value (key, ACTIVATION, tmp_int);
          si->activation_mode_ =
            static_cast <ImplementationRepository::ActivationMode> (tmp_int);
          
          
          config_.get_string_value (key, PARTIAL_IOR, tmp);
          si->partial_ior = ACE_TEXT_ALWAYS_CHAR(tmp.c_str());

          config_.get_string_value (key, IOR, tmp);
          si->ior = ACE_TEXT_ALWAYS_CHAR(tmp.c_str());

          config_.get_integer_value (key, START_LIMIT, tmp_int);
          si->start_limit_ = tmp_int;
          
          config_.get_integer_value (key, PID, tmp_int);
          si->pid = tmp_int;
          
          if (config_.get_string_value (key, ALTKEY, tmp))
            {
              tmp_cstr = ACE_TEXT_ALWAYS_CHAR(tmp.c_str());
              if (tmp.length () > 0 &&
                  this->servers ().find (tmp_cstr, si->alt_info_) != 0)
                {
                  Server_Info *base_si = 0;
                  ACE_NEW (base_si, Server_Info);
                  base_si->key_name_ = tmp_cstr;
                  si->alt_info_.reset (base_si);
                  this->servers ().bind (tmp_cstr, si->alt_info_);
                }
            }
          config_.get_string_value (key, PEERS, tmp);
          tmp_cstr = ACE_TEXT_ALWAYS_CHAR(tmp.c_str());
          ImR_Utils::stringToPeerList (tmp_cstr, si->peers);
          this->servers ().bind (name_cstr, info);
          si = 0;
          ++index;
        }
    }
}

int
Config_Backing_Store::persistent_remove (const ACE_CString& name,
                                         bool activator)
{
  const ACE_TCHAR* const key =
    (activator ? ACTIVATORS_ROOT_KEY : SERVERS_ROOT_KEY);
  return remove (name, key);
}

int
Config_Backing_Store::remove (const ACE_CString& name, const ACE_TCHAR* key)
{
  ACE_Configuration_Section_Key root;
  int err = config_.open_section (config_.root_section (), key, 0, root);
  if (err != 0)
    {
      if (this->opts_.debug () > 9)
        {
          ORBSVCS_DEBUG((LM_INFO, ACE_TEXT ("could not remove %C, already gone!\n"),
            name.c_str()));
        }
      return 0; // Already gone.
    }
  return config_.remove_section (root, ACE_TEXT_CHAR_TO_TCHAR(name.c_str()), 1);
}

static int get_key (ACE_Configuration& cfg, const ACE_CString& name,
                    const ACE_TCHAR* const sub_section,
                    ACE_Configuration_Section_Key& key)
{
  ACE_Configuration_Section_Key root;
  int err = cfg.open_section (cfg.root_section(), sub_section, 1, root);
  if (err != 0)
    {
      ORBSVCS_ERROR ((LM_ERROR, ACE_TEXT ("Unable to open config section:%s\n"),
        sub_section));
      return err;
    }
  err = cfg.open_section (root, ACE_TEXT_CHAR_TO_TCHAR(name.c_str ()), 1, key);
  if (err != 0)
    {
      ORBSVCS_ERROR((LM_ERROR, ACE_TEXT ("Unable to open config section:%C\n"),
        name.c_str ()));
    }
  return err;
}

int
Config_Backing_Store::persistent_update (const Server_Info_Ptr& info, bool )
{
  ACE_Configuration_Section_Key key;
  int err = get_key(this->config_, info->key_name_, SERVERS_ROOT_KEY, key);
  if (err != 0)
    {
      ORBSVCS_ERROR((LM_ERROR, ACE_TEXT ("ERROR: could not get key for %C\n"),
        info->key_name_.c_str ()));
      return err;
    }

  if (this->opts_.debug() > 9)
    {
      ORBSVCS_DEBUG((LM_INFO, ACE_TEXT ("updating %C\n"), info->poa_name.c_str()));
    }
  ACE_CString envstr = ImR_Utils::envListToString (info->env_vars);
  ACE_CString peerstr = ImR_Utils::peerListToString (info->peers);

  this->config_.set_string_value (key, POA, ACE_TEXT_CHAR_TO_TCHAR(info->poa_name.c_str()));
  this->config_.set_string_value (key, SERVER_ID, ACE_TEXT_CHAR_TO_TCHAR(info->server_id.c_str()));
  this->config_.set_string_value (key, JACORB_SERVER, info->is_jacorb ? ACE_TEXT("1") : ACE_TEXT("0"));
  this->config_.set_string_value (key, ACTIVATOR, ACE_TEXT_CHAR_TO_TCHAR(info->activator.c_str()));
  this->config_.set_string_value (key, STARTUP_COMMAND, ACE_TEXT_CHAR_TO_TCHAR(info->cmdline.c_str()));
  this->config_.set_string_value (key, WORKING_DIR, ACE_TEXT_CHAR_TO_TCHAR(info->dir.c_str()));
  this->config_.set_string_value (key, ENVIRONMENT, ACE_TEXT_CHAR_TO_TCHAR(envstr.c_str()));
  this->config_.set_integer_value (key, ACTIVATION, info->activation_mode_);
  this->config_.set_integer_value (key, START_LIMIT, info->start_limit_);
  this->config_.set_string_value (key, PARTIAL_IOR, ACE_TEXT_CHAR_TO_TCHAR(info->partial_ior.c_str()));
  this->config_.set_string_value (key, IOR, ACE_TEXT_CHAR_TO_TCHAR(info->ior.c_str()));
  this->config_.set_integer_value (key, PID, info->pid);
  this->config_.set_string_value (key, ALTKEY,
                                  info->alt_info_.null () ? ACE_TEXT("") : ACE_TEXT_CHAR_TO_TCHAR(info->alt_info_->key_name_.c_str()));
  this->config_.set_string_value (key, PEERS, ACE_TEXT_CHAR_TO_TCHAR(peerstr.c_str()));

  return 0;
}

int
Config_Backing_Store::persistent_update (const Activator_Info_Ptr& info, bool )
{
  ACE_Configuration_Section_Key key;
  int err = get_key (this->config_, info->name, ACTIVATORS_ROOT_KEY, key);
  if (err != 0)
    {
      ORBSVCS_DEBUG((LM_INFO,
        ACE_TEXT ("ERROR: could not get key for activator %C\n"),
        info->name.c_str ()));
      return err;
    }

  if (this->opts_.debug() > 9)
    {
      ORBSVCS_DEBUG((LM_INFO, ACE_TEXT ("updating activator %C\n"),
        info->name.c_str ()));
    }
  this->config_.set_integer_value (key, TOKEN, info->token);
  this->config_.set_string_value (key, IOR, ACE_TEXT_CHAR_TO_TCHAR(info->ior.c_str()));

  return 0;
}

int
Config_Backing_Store::init_repo (PortableServer::POA_ptr )
{
  if (status_ != 0)
    {
      if (this->opts_.debug () > 9)
        {
          ORBSVCS_DEBUG ((LM_INFO, ACE_TEXT ("not loading\n")));
        }
      return status_;
    }

  this->loadActivators();
  this->loadServers();

  return 0;
}

Heap_Backing_Store::Heap_Backing_Store (const Options& opts,
                                        CORBA::ORB_ptr orb)
: Config_Backing_Store (opts, orb, heap_),
  filename_ (opts.persist_file_name())
{
  if (opts.repository_erase ())
    {
      if (this->opts_.debug () > 9)
        {
          ORBSVCS_DEBUG ((LM_INFO, ACE_TEXT ("Heap start clean\n")));
        }
      ACE_OS::unlink ( this->filename_.c_str() );
    }

  status_ = heap_.open (this->filename_.c_str());
}

Heap_Backing_Store::~Heap_Backing_Store ()
{
}

const ACE_TCHAR*
Heap_Backing_Store::repo_mode() const
{
  return this->filename_.c_str();
}

#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_REGISTRY)
static HKEY setup_registry (const bool start_clean)
{
  if (start_clean)
    {
      ACE_Configuration_Win32Registry config ( HKEY_LOCAL_MACHINE );
      ACE_Configuration_Section_Key root;
      config.open_section (config.root_section(), ACE_TEXT("Software\\TAO"), 0, root);
      config.remove_section (root, ACE_TEXT("ImplementationRepository"), 1);
    }

  return ACE_Configuration_Win32Registry::
    resolve_key (HKEY_LOCAL_MACHINE, ACE_TEXT_CHAR_TO_TCHAR(WIN32_REG_KEY));
}
#endif

Registry_Backing_Store::Registry_Backing_Store(const Options& opts,
                                               CORBA::ORB_ptr orb)
#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_REGISTRY)
: Config_Backing_Store(opts, orb, win32registry_),
  win32registry_ (setup_registry (opts.repository_erase()))
#else
: Config_Backing_Store (opts, orb, invalid_config_)
#endif
{
#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_REGISTRY)
  status_ = 0;
#else
  ORBSVCS_ERROR ((LM_ERROR, ACE_TEXT ("Registry persistence is only ")
                        ACE_TEXT ("supported on Windows\n")));
  status_ = -1;
#endif
}

Registry_Backing_Store::~Registry_Backing_Store ()
{
}

const ACE_TCHAR*
Registry_Backing_Store::repo_mode () const
{
  return ACE_TEXT("Registry");
}
