#include "Server_Repository.h"
#include "XML_ContentHandler.h"
#include "utils.h"

#include "ACEXML/parser/parser/Parser.h"
#include "ACEXML/common/FileCharStream.h"

ACE_RCSID (ImplRepo_Service,
           Server_Repository,
           "$Id$")

static const char* STARTUP_COMMAND = "StartupCommand";
static const char* WORKING_DIR = "WorkingDir";
static const char* ENVIRONMENT = "Environment";
static const char* ACTIVATION = "Activation";
static const char* PARTIAL_IOR = "Location";
static const char* IOR = "IOR";

#if defined (ACE_WIN32)
static const char* WIN32_REG_KEY = "Software\\TAO\\IR\\Servers";
#endif

static void loadAsBinary(ACE_Configuration& config, 
                         Server_Repository::SIMap& server_infos) 
{

  int index = 0;
  ACE_CString name;
  while (config.enumerate_sections (config.root_section(), index, name) == 0)
  {
    ACE_CString cmdline, dir, envstr, partial_ior, ior;
    u_int amodeint = ImplementationRepository::MANUAL;

    ACE_Configuration_Section_Key server_key;

    // Can't fail, because we're enumerating
    config.open_section(config.root_section(), name.c_str(), 0, server_key);

    // Ignore any missing values. Server name is enough on its own.
    config.get_string_value (server_key, STARTUP_COMMAND, cmdline);
    config.get_string_value (server_key, WORKING_DIR, dir);
    config.get_string_value (server_key, ENVIRONMENT, envstr);
    config.get_integer_value(server_key, ACTIVATION, amodeint);
    config.get_string_value (server_key, PARTIAL_IOR, partial_ior);
    config.get_string_value (server_key, IOR, ior);

    ImplementationRepository::ActivationMode amode = ACE_static_cast (
      ImplementationRepository::ActivationMode, amodeint);

    ImplementationRepository::EnvironmentList env_vars =
      ImR_Utils::parseEnvList(envstr);

    Server_Info_Ptr si(
      new Server_Info(name, cmdline, env_vars, dir, amode, partial_ior, ior));
    server_infos.bind (name, si);
    index++;
  }
}

// Note : There is no saveAsBinary(), because the ACE_Configuration class
// supports saving of individual entries.

class XML_ServerInfo_Callback : public XML_ContentHandler::Callback {
  Server_Repository::SIMap& server_infos_;
public:
  XML_ServerInfo_Callback(Server_Repository::SIMap& server_infos)
    : server_infos_(server_infos)
  {
  }
  virtual void next_server(const ACE_CString& name, const ACE_CString& cmdline, 
    const ACE_CString& envstr, const ACE_CString& dir,
    const ACE_CString& amodestr, const ACE_CString& partial_ior, const ACE_CString& ior)
  {
    ImplementationRepository::ActivationMode amode = ImR_Utils::parseActivationMode(amodestr);
    ImplementationRepository::EnvironmentList env_vars = ImR_Utils::parseEnvList(envstr);
    Server_Info_Ptr si(
      new Server_Info(name, cmdline, env_vars, dir, amode, partial_ior, ior));
    this->server_infos_.bind(name, si);
  }
};

static int loadAsXML(const ACE_CString& fname, Server_Repository::SIMap& server_infos) {

  ACEXML_FileCharStream* fstm = new ACEXML_FileCharStream; // xml input source will take ownership

  if (fstm->open(fname.c_str()) != 0)
  {
    // This is not a real error. The xml file may not exist yet.
    delete fstm;
    return 0;
  }

  XML_ServerInfo_Callback cb(server_infos);

  XML_ContentHandler handler(cb);

  ACEXML_Parser parser;

  // InputSource takes ownership
  ACEXML_InputSource input(fstm);

  parser.setContentHandler (&handler);
  parser.setDTDHandler (&handler);
  parser.setErrorHandler (&handler);
  parser.setEntityResolver (&handler);

  ACEXML_TRY_NEW_ENV
  {
    parser.parse (&input ACEXML_ENV_ARG_PARAMETER);
    ACEXML_TRY_CHECK;
  }
  ACEXML_CATCH (ACEXML_Exception, ex)
  {
    ACE_ERROR((LM_ERROR, "Error during load of ImR persistence xml file."));
    ex.print();
    return -1;
  }
  ACEXML_ENDTRY;
  return 0;
}

// Note : Would pass servers by const&, but ACE hash map const_iterator is broken.
static void saveAsXML(const ACE_CString& fname, Server_Repository::SIMap& servers) {
  FILE* fp = ACE_OS::fopen (fname.c_str(), "w");
  if (fp == 0)
  {
    ACE_ERROR((LM_ERROR, "Couldn't write to file %s\n", fname.c_str()));
    return;
  }
  ACE_OS::fprintf(fp,"<?xml version=\"1.0\"?>\n");
  ACE_OS::fprintf(fp,"<!DOCTYPE %s/>\n", XML_ContentHandler::ROOT_TAG);
  ACE_OS::fprintf(fp,"<%s>\n", XML_ContentHandler::ROOT_TAG);

  Server_Repository::SIMap::ENTRY* entry = 0;
  Server_Repository::SIMap::ITERATOR it(servers);
  for (; it.next(entry); it.advance()) {
    Server_Info& info = * entry->int_id_;
    ACE_CString amodestr = ImR_Utils::activationModeToString(info.activation_mode);
    ACE_CString envstr = ImR_Utils::envListToString(info.env_vars);

    ACE_OS::fprintf(fp,"<%s>\n", XML_ContentHandler::SERVER_INFO_TAG);

    ACE_OS::fprintf(fp,"\t<%s>", XML_ContentHandler::SERVER_TAG);
    ACE_OS::fprintf(fp,"%s", info.name.c_str());
    ACE_OS::fprintf(fp,"</%s>\n", XML_ContentHandler::SERVER_TAG);

    ACE_OS::fprintf(fp,"\t<%s>\n", XML_ContentHandler::STARTUP_OPTIONS_TAG);

    ACE_OS::fprintf(fp,"\t\t<%s>", XML_ContentHandler::COMMAND_LINE_TAG);
    ACE_OS::fprintf(fp,"%s", info.cmdline.c_str());
    ACE_OS::fprintf(fp,"</%s>\n", XML_ContentHandler::COMMAND_LINE_TAG);
    ACE_OS::fprintf(fp,"\t\t<%s>", XML_ContentHandler::WORKING_DIR_TAG);
    ACE_OS::fprintf(fp,"%s", info.dir.c_str());
    ACE_OS::fprintf(fp,"</%s>\n", XML_ContentHandler::WORKING_DIR_TAG);

    for (CORBA::ULong i = 0; i < info.env_vars.length(); ++i)
    {
      ACE_OS::fprintf(fp,"\t\t<%s", XML_ContentHandler::ENVIRONMENT_TAG);
      ACE_OS::fprintf(fp," name=\"%s\"", info.env_vars[i].name.in());
      ACE_OS::fprintf(fp," value=\"%s\"", info.env_vars[i].value.in());
      ACE_OS::fprintf(fp,"/>\n");
    }

    ACE_OS::fprintf(fp,"\t\t<%s>", XML_ContentHandler::ACTIVATION_TAG);
    ACE_OS::fprintf(fp,"%s", amodestr.c_str());
    ACE_OS::fprintf(fp,"</%s>\n", XML_ContentHandler::ACTIVATION_TAG);

    ACE_OS::fprintf(fp,"\t</%s>\n", XML_ContentHandler::STARTUP_OPTIONS_TAG);

    ACE_OS::fprintf(fp,"\t<%s>", XML_ContentHandler::PARTIAL_IOR_TAG);
    ACE_OS::fprintf(fp,"%s", info.partial_ior.c_str());
    ACE_OS::fprintf(fp,"</%s>\n", XML_ContentHandler::PARTIAL_IOR_TAG);

    ACE_OS::fprintf(fp,"\t<%s>", XML_ContentHandler::IOR_TAG);
    ACE_OS::fprintf(fp,"%s", info.ior.c_str());
    ACE_OS::fprintf(fp,"</%s>\n", XML_ContentHandler::IOR_TAG);

    ACE_OS::fprintf(fp,"</%s>\n", XML_ContentHandler::SERVER_INFO_TAG);
  }
  ACE_OS::fprintf(fp,"</%s>\n", XML_ContentHandler::ROOT_TAG);
  ACE_OS::fclose(fp);
}

Server_Repository::Server_Repository(const ACE_CString& activator)
: rmode_(Options::REPO_NONE)
, config_(0)
, activator_name_(activator)
{
}

int
Server_Repository::init(Options::RepoMode rmode, const ACE_CString& name) 
{
  this->rmode_ = rmode;
  this->fname_ = name;

  int err = 0;
  switch (this->rmode_) {
  case Options::REPO_NONE:
    {
      break;
    }
  case Options::REPO_HEAP_FILE:
    {
      ACE_Configuration_Heap* heap = new ACE_Configuration_Heap();
      this->config_.reset(heap);
      err = heap->open(this->fname_.c_str());
      if (err == 0)
      {
        loadAsBinary(*this->config_, this->server_infos_);
      }
      break;
    }
#if defined (ACE_WIN32)
  case Options::REPO_REGISTRY:
    {
      HKEY root = ACE_Configuration_Win32Registry::
        resolve_key(HKEY_LOCAL_MACHINE, WIN32_REG_KEY);
      this->config_.reset(new ACE_Configuration_Win32Registry(root));
      loadAsBinary(*this->config_, this->server_infos_);
      break;
    }
#endif
  case Options::REPO_XML_FILE:
    {
      err = loadAsXML(this->fname_, this->server_infos_);  
      break;
    }
  default:
    {
      bool invalid_rmode_specified = false;
      ACE_ASSERT(invalid_rmode_specified);
      ACE_UNUSED_ARG(invalid_rmode_specified);
      err = -1;
    }
  }
  return err;
}


int
Server_Repository::add (
                        const ACE_CString& server_name,
                        const ACE_CString& startup_command,
                        const ImplementationRepository::EnvironmentList& env_vars,
                        const ACE_CString& working_dir,
                        ImplementationRepository::ActivationMode activation,
                        const ACE_CString& partial_ior,
                        const ACE_CString& ior
                        )
{
  Server_Info_Ptr new_server(new Server_Info(server_name, 
    startup_command, env_vars, working_dir, activation, partial_ior, ior));

  int err = this->server_infos_.bind (server_name, new_server);
  if (err != 0) 
  {
    return err;
  }

  this->update(* new_server);
  return 0;
}

int
Server_Repository::update (const Server_Info& info)
{
  if (rmode_ == Options::REPO_HEAP_FILE || rmode_ == Options::REPO_REGISTRY)
  {
    ACE_ASSERT(this->config_.get() != 0);

    ACE_Configuration& cfg = *this->config_;

    ACE_Configuration_Section_Key key;
    int err = cfg.open_section (cfg.root_section(), info.name.c_str(), 1, key);
    if (err != 0)
    {
      ACE_ERROR((LM_ERROR, "Unable to open config section:%s\n", info.name.c_str()));
      return err;
    }

    ACE_CString envstr = ImR_Utils::envListToString(info.env_vars);

    cfg.set_string_value (key, STARTUP_COMMAND, info.cmdline.c_str());
    cfg.set_string_value (key, WORKING_DIR, info.dir.c_str());
    cfg.set_string_value (key, ENVIRONMENT, envstr);
    cfg.set_integer_value (key, ACTIVATION, info.activation_mode);
    cfg.set_string_value (key, PARTIAL_IOR, info.partial_ior.c_str());
    cfg.set_string_value (key, IOR, info.ior.c_str());
  }
  else if (rmode_ == Options::REPO_XML_FILE)
  {
    saveAsXML(this->fname_, this->server_infos_);
  }
  return 0;
}

Server_Info_Ptr
Server_Repository::get_server_info (const ACE_CString& server_name)
{
  Server_Info_Ptr server(0);
  this->server_infos_.find (server_name, server);
  return server;
}

int
Server_Repository::remove (const ACE_CString& server_name)
{
  int ret = this->server_infos_.unbind (server_name);
  if (ret != 0) 
  {
    return ret;
  }

  if (rmode_ == Options::REPO_HEAP_FILE || rmode_ == Options::REPO_REGISTRY)
  {
    ACE_ASSERT(this->config_.get() != 0);
    ACE_Configuration& cfg = *this->config_;
    ret = cfg.remove_section (cfg.root_section(), server_name.c_str(), 1);
  }
  else if (rmode_ == Options::REPO_XML_FILE)
  {
    saveAsXML(this->fname_, this->server_infos_);
  }
  return ret;
}

Server_Repository::SIMap&
Server_Repository::servers(void)
{
  return this->server_infos_;
}

const char* 
Server_Repository::activator_name(void) const
{
  return this->activator_name_.c_str();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<ACE_CString, ACE_Refcounted_Auto_Ptr<Server_Info, ACE_Null_Mutex> >;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, ACE_Refcounted_Auto_Ptr<Server_Info, ACE_Null_Mutex>, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, ACE_Refcounted_Auto_Ptr<Server_Info, ACE_Null_Mutex>,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, ACE_Refcounted_Auto_Ptr<Server_Info, ACE_Null_Mutex>,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, ACE_Refcounted_Auto_Ptr<Server_Info, ACE_Null_Mutex>,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;

#if defined(__SUNPRO_CC) && (__SUNPRO_CC == 0x500)
// This template is already defined in TAO, but Sun/CC 5.0 is broken
template class ACE_Equal_To<ACE_CString>;
#endif /* __SUNPRO_CC */
template class ACE_Auto_Ptr<ACE_Configuration>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, ACE_Refcounted_Auto_Ptr<Server_Info, ACE_Null_Mutex> >
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, ACE_Refcounted_Auto_Ptr<Server_Info, ACE_Null_Mutex> ,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, ACE_Refcounted_Auto_Ptr<Server_Info, ACE_Null_Mutex> ,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, ACE_Refcounted_Auto_Ptr<Server_Info, ACE_Null_Mutex> ,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, ACE_Refcounted_Auto_Ptr<Server_Info, ACE_Null_Mutex> ,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>

#if defined(__SUNPRO_CC) && (__SUNPRO_CC == 0x500)
// This template is already defined in TAO, but Sun/CC 5.0 is broken
#pragma instantiate ACE_Equal_To<ACE_CString>
#endif /* __SUNPRO_CC */
#pragma instantiate  ACE_Auto_Ptr<ACE_Configuration>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
