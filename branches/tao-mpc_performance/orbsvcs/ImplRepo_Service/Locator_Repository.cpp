// $Id$

#include "Locator_Repository.h"
#include "Locator_XMLHandler.h"
#include "utils.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_ctype.h"
#include "ace/OS_NS_unistd.h"

#include "ACEXML/parser/parser/Parser.h"
#include "ACEXML/common/FileCharStream.h"
#include "ACEXML/common/XML_Util.h"

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

#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_REGISTRY)
static const char* WIN32_REG_KEY = "Software\\TAO\\ImplementationRepository";
#endif

static ACE_CString lcase (const ACE_CString& s)
{
  ACE_CString ret(s);
  for (size_t i = 0; i < ret.length (); ++i)
    {
      ret[i] = static_cast<char>(ACE_OS::ace_tolower (s[i]));
    }
  return ret;
}

static void loadActivatorsAsBinary (ACE_Configuration& config, Locator_Repository::AIMap& map)
{
  ACE_Configuration_Section_Key root;
  int err = config.open_section (config.root_section (), ACTIVATORS_ROOT_KEY, 0, root);
  if (err == 0)
    {
      int index = 0;
      ACE_TString name;
      while (config.enumerate_sections (root, index, name) == 0)
        {
          ACE_CString ior;
          u_int token;

          ACE_Configuration_Section_Key key;

          // Can't fail, because we're enumerating
          config.open_section (root, name.c_str(), 0, key);

          config.get_string_value (key, IOR, ior);
          config.get_integer_value (key, TOKEN, token);

          Activator_Info_Ptr info (new Activator_Info (name, token, ior));
          map.bind (lcase (name), info);
          index++;
        }
    }
}

static void loadServersAsBinary(ACE_Configuration& config, Locator_Repository::SIMap& map)
{
  ACE_Configuration_Section_Key root;
  int err = config.open_section (config.root_section (), SERVERS_ROOT_KEY, 0, root);
  if (err == 0)
    {
      int index = 0;
      ACE_TString name;
      while (config.enumerate_sections (root, index, name) == 0)
        {
          ACE_CString server_id, cmdline, dir, envstr, partial_ior, ior, aname;
          u_int amodeint = ImplementationRepository::MANUAL;
          u_int start_limit;

          ACE_Configuration_Section_Key key;

          // Can't fail, because we're enumerating
          config.open_section (root, name.c_str (), 0, key);

          // Ignore any missing values. Server name is enough on its own.
          config.get_string_value (key, SERVER_ID, server_id);
          config.get_string_value (key, ACTIVATOR, aname);
          config.get_string_value (key, STARTUP_COMMAND, cmdline);
          config.get_string_value (key, WORKING_DIR, dir);
          config.get_string_value (key, ENVIRONMENT, envstr);
          config.get_integer_value(key, ACTIVATION, amodeint);
          config.get_string_value (key, PARTIAL_IOR, partial_ior);
          config.get_string_value (key, IOR, ior);
          config.get_integer_value(key, START_LIMIT, start_limit);

          ImplementationRepository::ActivationMode amode =
            static_cast <ImplementationRepository::ActivationMode> (amodeint);

          ImplementationRepository::EnvironmentList env_vars =
            ImR_Utils::parseEnvList (envstr);

          Server_Info_Ptr info (new Server_Info(server_id, name, aname, cmdline,
            env_vars, dir, amode, start_limit, partial_ior, ior));
          map.bind (name, info);
          index++;
        }
    }
}

static void loadAsBinary (ACE_Configuration& config, Locator_Repository& repo)
{
  loadServersAsBinary (config, repo.servers ());
  loadActivatorsAsBinary (config, repo.activators ());
}

// Note : There is no saveAsBinary(), because the ACE_Configuration class
// supports saving of individual entries.

static void convertEnvList (const Locator_XMLHandler::EnvList& in, ImplementationRepository::EnvironmentList& out)
{
  CORBA::ULong sz = in.size ();
  out.length (sz);
  for (CORBA::ULong i = 0; i < sz; ++i)
    {
      out[i].name = in[i].name.c_str ();
      out[i].value = in[i].value.c_str ();
    }
}

class Server_Repo_XML_Callback : public Locator_XMLHandler::Callback
{
public:
  Server_Repo_XML_Callback(Locator_Repository& repo)
    : repo_ (repo)
  {
  }
  virtual void next_server (const ACE_CString& server_id,
    const ACE_CString& name, const ACE_CString& aname,
    const ACE_CString& cmdline, const Locator_XMLHandler::EnvList& envlst,
    const ACE_CString& dir, const ACE_CString& amodestr, int start_limit,
    const ACE_CString& partial_ior, const ACE_CString& ior)
  {
    ImplementationRepository::ActivationMode amode =
      ImR_Utils::parseActivationMode (amodestr);

    ImplementationRepository::EnvironmentList env_vars;
    convertEnvList (envlst, env_vars);

    int limit = start_limit < 1 ? 1 : start_limit;

    Server_Info_Ptr si (new Server_Info (server_id, name, aname, cmdline,
      env_vars, dir, amode, limit, partial_ior, ior));

    this->repo_.servers ().bind (name, si);
  }
  virtual void next_activator (const ACE_CString& aname,
    long token,
    const ACE_CString& ior)
  {
    Activator_Info_Ptr si (new Activator_Info (aname, token, ior));
    this->repo_.activators ().bind (lcase (aname), si);
  }
private:
  Locator_Repository& repo_;
};

static int loadAsXML (const ACE_CString& fname, Locator_Repository& repo)
{
  ACEXML_FileCharStream* fstm = new ACEXML_FileCharStream; // xml input source will take ownership

  if (fstm->open (fname.c_str()) != 0)
    {
      // This is not a real error. The xml file may not exist yet.
      delete fstm;
      return 0;
    }

  Server_Repo_XML_Callback cb (repo);

  Locator_XMLHandler handler (cb);

  ACEXML_Parser parser;

  // InputSource takes ownership
  ACEXML_InputSource input (fstm);

  parser.setContentHandler (&handler);
  parser.setDTDHandler (&handler);
  parser.setErrorHandler (&handler);
  parser.setEntityResolver (&handler);

  try
    {
      parser.parse (&input);
    }
  catch (const ACEXML_Exception& ex)
    {
      ACE_ERROR ((LM_ERROR, "Error during load of ImR persistence xml file."));
      ex.print ();
      return -1;
    }
  return 0;
}

// Note : Would pass servers by const&, but ACE hash map const_iterator is broken.
static void saveAsXML (const ACE_CString& fname, Locator_Repository& repo)
{
  FILE* fp = ACE_OS::fopen (fname.c_str (), "w");
  if (fp == 0)
    {
      ACE_ERROR ((LM_ERROR, "Couldn't write to file %s\n", fname.c_str()));
      return;
    }
  ACE_OS::fprintf (fp,"<?xml version=\"1.0\"?>\n");
  ACE_OS::fprintf (fp,"<%s>\n", Locator_XMLHandler::ROOT_TAG);

  // Save servers
  Locator_Repository::SIMap::ENTRY* sientry = 0;
  Locator_Repository::SIMap::ITERATOR siit (repo.servers ());
  for (; siit.next (sientry); siit.advance() )
    {
      Server_Info_Ptr& info = sientry->int_id_;

      ACE_CString server_id = ACEXML_escape_string (info->server_id);
      ACE_CString name = ACEXML_escape_string (info->name);
      ACE_CString activator = ACEXML_escape_string (info->activator);
      ACE_CString cmdline = ACEXML_escape_string (info->cmdline);
      ACE_CString wdir = ACEXML_escape_string (info->dir);
      ACE_CString partial_ior = ACEXML_escape_string (info->partial_ior);
      ACE_CString ior = ACEXML_escape_string (info->ior);

      ACE_OS::fprintf (fp,"\t<%s", Locator_XMLHandler::SERVER_INFO_TAG);
      ACE_OS::fprintf (fp," server_id=\"%s\"", server_id.c_str ());
      ACE_OS::fprintf (fp," name=\"%s\"", name.c_str ());
      ACE_OS::fprintf (fp," activator=\"%s\"", activator.c_str ());
      ACE_OS::fprintf (fp," command_line=\"%s\"", cmdline.c_str ());
      ACE_OS::fprintf (fp," working_dir=\"%s\"", wdir.c_str ());
      ACE_CString amodestr = ImR_Utils::activationModeToString (info->activation_mode);
      ACE_OS::fprintf (fp," activation_mode=\"%s\"", amodestr.c_str ());
      ACE_OS::fprintf (fp," start_limit=\"%d\"", info->start_limit);
      ACE_OS::fprintf (fp," partial_ior=\"%s\"", partial_ior.c_str ());
      ACE_OS::fprintf (fp," ior=\"%s\"", ior.c_str ());
      ACE_OS::fprintf (fp,">\n");

      for (CORBA::ULong i = 0; i < info->env_vars.length (); ++i)
        {
          ACE_OS::fprintf (fp,"\t\t<%s", Locator_XMLHandler::ENVIRONMENT_TAG);
          ACE_OS::fprintf (fp," name=\"%s\"", info->env_vars[i].name.in ());
          ACE_CString val = ACEXML_escape_string (info->env_vars[i].value.in ());
          ACE_OS::fprintf (fp," value=\"%s\"", val.c_str ());
          ACE_OS::fprintf (fp,"/>\n");
        }

      ACE_OS::fprintf (fp,"\t</%s>\n", Locator_XMLHandler::SERVER_INFO_TAG);
    }

  // Save Activators
  Locator_Repository::AIMap::ENTRY* aientry = 0;
  Locator_Repository::AIMap::ITERATOR aiit (repo.activators ());
  for (; aiit.next (aientry); aiit.advance ())
    {
      ACE_CString aname = aientry->ext_id_;
      Activator_Info_Ptr& info = aientry->int_id_;
      ACE_OS::fprintf (fp,"\t<%s", Locator_XMLHandler::ACTIVATOR_INFO_TAG);
      ACE_OS::fprintf( fp," name=\"%s\"", aname.c_str ());
      ACE_OS::fprintf (fp," token=\"%d\"", info->token);
      ACE_OS::fprintf (fp," ior=\"%s\"", info->ior.c_str ());
      ACE_OS::fprintf (fp,"/>\n");
    }

  ACE_OS::fprintf (fp,"</%s>\n", Locator_XMLHandler::ROOT_TAG);
  ACE_OS::fclose (fp);
}

Locator_Repository::Locator_Repository ()
: rmode_ (Options::REPO_NONE)
, config_ (0)
, debug_ (0)
{
}

int
Locator_Repository::init(const Options& opts)
{
  this->rmode_ = opts.repository_mode ();
  this->fname_ = opts.persist_file_name ();
  this->debug_ = opts.debug ();

  int err = 0;
  switch (this->rmode_)
    {
    case Options::REPO_NONE:
      {
        break;
      }
    case Options::REPO_HEAP_FILE:
      {
        if (opts.repository_erase ())
        {
          ACE_OS::unlink ( this->fname_.c_str () );
        }
        ACE_Configuration_Heap* heap = new ACE_Configuration_Heap ();
        this->config_.reset (heap);
        err = heap->open (this->fname_.c_str ());
        if (err == 0)
        {
          loadAsBinary (*this->config_, *this);
        }
        break;
      }
    case Options::REPO_REGISTRY:
      {
#if defined (ACE_WIN32) && !defined (ACE_LACKS_WIN32_REGISTRY)
        if (opts.repository_erase ())
          {
            ACE_Configuration_Win32Registry config ( HKEY_LOCAL_MACHINE );
            ACE_Configuration_Section_Key root;
            config.open_section (config.root_section(), "Software\\TAO", 0, root);
            config.remove_section (root, "ImplementationRepository", 1);
          }
        HKEY root = ACE_Configuration_Win32Registry::
          resolve_key (HKEY_LOCAL_MACHINE, WIN32_REG_KEY);
        this->config_.reset (new ACE_Configuration_Win32Registry( root));
        loadAsBinary (*this->config_, *this);
#else
        ACE_ERROR ((LM_ERROR, "Registry persistence is only "
                              "supported on Windows\n"));
        err = -1;
#endif
        break;
      }
    case Options::REPO_XML_FILE:
      {
        if (opts.repository_erase ())
          {
            ACE_OS::unlink ( this->fname_.c_str() );
          }
        err = loadAsXML (this->fname_, *this);
        break;
      }
    default:
      {
        bool invalid_rmode_specified = false;
        ACE_ASSERT (invalid_rmode_specified);
        ACE_UNUSED_ARG (invalid_rmode_specified);
        err = -1;
      }
    }
  return err;
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
      ACE_TEXT ("for server \"%s %s\" ior \"%s\"\n"),
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
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t)ImR: iterating - registered server")
        ACE_TEXT ("\"%s %s\" ior \"%s\"\n"), info->server_id.c_str(),
        info->name.c_str (), info->partial_ior.c_str ()));
    }

    if (info->partial_ior == partial_ior
      && name != info->name
      && info->server_id != server_id)
    {
      if (this->debug_)
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t)ImR: reuse address %s so remove")
          ACE_TEXT ("server %s \n"), info->partial_ior.c_str (), info->name.c_str ()));
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
Locator_Repository::add_server (const ACE_CString& server_id,
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
  int limit = start_limit < 1 ? 1 : start_limit;
  Server_Info_Ptr info(new Server_Info (server_id, name, aname, startup_command,
    env_vars, working_dir, activation, limit, partial_ior, ior, svrobj));

  int err = servers ().bind (name, info);
  if (err != 0)
    {
      return err;
    }
  this->update_server (*info);
  return 0;
}

int
Locator_Repository::add_activator (const ACE_CString& name,
                        const CORBA::Long token,
                        const ACE_CString& ior,
                        ImplementationRepository::Activator_ptr act)
{
  Activator_Info_Ptr info (new Activator_Info (name, token, ior, act));

  int err = activators ().bind (lcase (name), info);
  if (err != 0)
    {
      return err;
    }
  this->update_activator (*info);
  return 0;
}

int
Locator_Repository::update_server (const Server_Info& info)
{
  if (rmode_ == Options::REPO_HEAP_FILE || rmode_ == Options::REPO_REGISTRY)
    {
      ACE_ASSERT (this->config_.get () != 0);

      ACE_Configuration& cfg = *this->config_;

      ACE_Configuration_Section_Key root;
      ACE_Configuration_Section_Key key;
      int err = cfg.open_section (cfg.root_section(), SERVERS_ROOT_KEY, 1, root);
      if (err != 0)
        {
          ACE_ERROR ((LM_ERROR, "Unable to open config section:%s\n", SERVERS_ROOT_KEY));
          return err;
        }
      err = cfg.open_section (root, info.name.c_str (), 1, key);
      if (err != 0)
        {
          ACE_ERROR((LM_ERROR, "Unable to open config section:%s\n", info.name.c_str()));
          return err;
        }

      ACE_CString envstr = ImR_Utils::envListToString(info.env_vars);

      cfg.set_string_value (key, SERVER_ID, info.server_id.c_str ());
      cfg.set_string_value (key, ACTIVATOR, info.activator.c_str ());
      cfg.set_string_value (key, STARTUP_COMMAND, info.cmdline.c_str ());
      cfg.set_string_value (key, WORKING_DIR, info.dir.c_str ());
      cfg.set_string_value (key, ENVIRONMENT, envstr);
      cfg.set_integer_value (key, ACTIVATION, info.activation_mode);
      cfg.set_integer_value (key, START_LIMIT, info.start_limit);
      cfg.set_string_value (key, PARTIAL_IOR, info.partial_ior.c_str ());
      cfg.set_string_value (key, IOR, info.ior.c_str());
    }
  else if (rmode_ == Options::REPO_XML_FILE)
    {
      saveAsXML (this->fname_, *this);
    }
  return 0;
}

int
Locator_Repository::update_activator (const Activator_Info& info)
{
  if (rmode_ == Options::REPO_HEAP_FILE || rmode_ == Options::REPO_REGISTRY)
    {
      ACE_ASSERT(this->config_.get () != 0);

      ACE_Configuration& cfg = *this->config_;

      ACE_Configuration_Section_Key root;
      ACE_Configuration_Section_Key key;
      int err = cfg.open_section (cfg.root_section(), ACTIVATORS_ROOT_KEY, 1, root);
      if (err != 0)
        {
          ACE_ERROR((LM_ERROR, "Unable to open config section:%s\n", ACTIVATORS_ROOT_KEY));
          return err;
        }
      err = cfg.open_section (root, info.name.c_str (), 1, key);
      if (err != 0)
        {
          ACE_ERROR((LM_ERROR, "Unable to open config section:%s\n", info.name.c_str()));
          return err;
        }

      cfg.set_integer_value (key, TOKEN, info.token);
      cfg.set_string_value (key, IOR, info.ior.c_str ());
    }
  else if (rmode_ == Options::REPO_XML_FILE)
    {
      saveAsXML( this->fname_, *this);
    }
  return 0;
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
  int ret = this->servers().unbind (name);
  if (ret != 0)
    {
      return ret;
    }

  if (rmode_ == Options::REPO_HEAP_FILE || rmode_ == Options::REPO_REGISTRY)
    {
      ACE_ASSERT (this->config_.get() != 0);
      ACE_Configuration& cfg = *this->config_;
      ACE_Configuration_Section_Key root;
      int err = cfg.open_section (cfg.root_section (), SERVERS_ROOT_KEY, 0, root);
      if (err != 0)
        {
          return 0; // Already gone.
        }
      ret = cfg.remove_section (root, name.c_str (), 1);
    }
  else if (rmode_ == Options::REPO_XML_FILE)
    {
      saveAsXML (this->fname_, *this);
    }
  return ret;
}

int
Locator_Repository::remove_activator (const ACE_CString& name)
{
  int ret = activators().unbind (lcase(name));
  if (ret != 0)
    {
      return ret;
    }

  if (rmode_ == Options::REPO_HEAP_FILE || rmode_ == Options::REPO_REGISTRY)
    {
      ACE_ASSERT (this->config_.get () != 0);
      ACE_Configuration& cfg = *this->config_;
      ACE_Configuration_Section_Key root;
      int err = cfg.open_section (cfg.root_section (), ACTIVATORS_ROOT_KEY, 0, root);
      if (err != 0)
      {
        return 0; // Already gone.
      }
      ret = cfg.remove_section (root, name.c_str (), 1);
    }
  else if (rmode_ == Options::REPO_XML_FILE)
  {
    saveAsXML (this->fname_, *this);
  }
  return ret;
}

Locator_Repository::SIMap&
Locator_Repository::servers (void)
{
  return server_infos_;
}

Locator_Repository::AIMap&
Locator_Repository::activators (void)
{
  return activator_infos_;
}

const char*
Locator_Repository::repo_mode ()
{
  switch (rmode_)
  {
  case Options::REPO_XML_FILE:
  case Options::REPO_HEAP_FILE:
    return fname_.c_str ();
  case Options::REPO_REGISTRY:
    return "Registry";
  case Options::REPO_NONE:
    return "Disabled";
  }
  return "Disabled";
}
