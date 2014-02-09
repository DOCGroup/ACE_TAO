// $Id$

#include "Locator_XMLHandler.h"
#include "XML_Backing_Store.h"
#include "utils.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_sys_time.h"

const ACE_TCHAR* Locator_XMLHandler::ROOT_TAG = ACE_TEXT("ImplementationRepository");
const ACE_TCHAR* Locator_XMLHandler::SERVER_INFO_TAG = ACE_TEXT("Servers");
const ACE_TCHAR* Locator_XMLHandler::ACTIVATOR_INFO_TAG = ACE_TEXT("Activators");
const ACE_TCHAR* Locator_XMLHandler::ENVIRONMENT_TAG = ACE_TEXT("EnvironmentVariables");

const ACE_TCHAR* Locator_XMLHandler::SERVER_TAG = ACE_TEXT("server_id");
const ACE_TCHAR* Locator_XMLHandler::POANAME_TAG = ACE_TEXT("poa_name");
const ACE_TCHAR* Locator_XMLHandler::JACORB_TAG = ACE_TEXT("is_jacorb");
const ACE_TCHAR* Locator_XMLHandler::ACTNAME_TAG = ACE_TEXT("activator");
const ACE_TCHAR* Locator_XMLHandler::CMDLINE_TAG = ACE_TEXT("command_line");
const ACE_TCHAR* Locator_XMLHandler::DIR_TAG = ACE_TEXT("working_dir");
const ACE_TCHAR* Locator_XMLHandler::MODE_TAG = ACE_TEXT("activation_mode");
const ACE_TCHAR* Locator_XMLHandler::LIMIT_TAG = ACE_TEXT("start_limit");
const ACE_TCHAR* Locator_XMLHandler::PARTIOR_TAG = ACE_TEXT("partial_ior");
const ACE_TCHAR* Locator_XMLHandler::IOR_TAG = ACE_TEXT("ior");
const ACE_TCHAR* Locator_XMLHandler::STARTED_TAG = ACE_TEXT("started");
const ACE_TCHAR* Locator_XMLHandler::PEER_TAG = ACE_TEXT("peer");
const ACE_TCHAR* Locator_XMLHandler::PID_TAG = ACE_TEXT("pid");
const ACE_TCHAR* Locator_XMLHandler::KEYNAME_TAG = ACE_TEXT("key_name");
const ACE_TCHAR* Locator_XMLHandler::ALTKEY_TAG = ACE_TEXT("altkey");


Locator_XMLHandler::Locator_XMLHandler (XML_Backing_Store& repo)
  : repo_(repo),
    si_ (0),
    server_started_ (false),
    extra_params_ (),
    env_vars_ (),
    peer_list_ ()
{
}

static void convertEnvList (const Locator_XMLHandler::EnvList& in,
                            ImplementationRepository::EnvironmentList& out)
{
  CORBA::ULong sz = static_cast<CORBA::ULong> (in.size ());
  out.length (sz);
  for (CORBA::ULong i = 0; i < sz; ++i)
    {
      out[i].name = in[i].name.c_str ();
      out[i].value = in[i].value.c_str ();
    }
}

static void convertPeerList (const Locator_XMLHandler::PeerList& in,
                             CORBA::StringSeq& out)
{
  CORBA::ULong sz = static_cast<CORBA::ULong> (in.size ());
  out.length (sz);
  for (CORBA::ULong i = 0; i < sz; ++i)
    {
      out[i] = in[i].c_str ();
    }
}


void
Locator_XMLHandler::startElement (const ACEXML_Char*,
                                  const ACEXML_Char*,
                                  const ACEXML_Char* qName,
                                  ACEXML_Attributes* attrs)
{
  if (ACE_OS::strcasecmp (qName, SERVER_INFO_TAG) == 0)
    {
      ACE_NEW (this->si_, Server_Info);
      this->env_vars_.clear ();

      // if attrs exists and if the previously required 9 fields
      const size_t previous_size = 9;
      if (attrs != 0 && attrs->getLength () >= previous_size)
        {
          size_t index = 0;
          this->si_->server_id = attrs->getValue (index++);
          this->si_->poa_name = attrs->getValue (index++);
          this->si_->activator = attrs->getValue (index++);
          this->si_->cmdline = attrs->getValue (index++);
          this->si_->dir = attrs->getValue (index++);
          this->si_->activation_mode_ =
            ImR_Utils::stringToActivationMode (attrs->getValue (index++));
          this->si_->start_limit_ = ACE_OS::atoi (attrs->getValue (index++));
          this->si_->partial_ior = attrs->getValue (index++);
          this->si_->ior = attrs->getValue (index++);

          if (attrs->getLength () >= index)
            {
              this->server_started_ =
                (ACE_OS::atoi (attrs->getValue (index++)) != 0);
            }
          if (attrs->getLength () >= index)
            {
              this->si_->is_jacorb =
                (ACE_OS::atoi (attrs->getValue (index++)) != 0);
            }

          for ( ; index < attrs->getLength(); ++index)
            {
              ACE_CString name (attrs->getLocalName (index));
              ACE_CString value (attrs->getValue (index));
              if (name == KEYNAME_TAG)
                {
                  this->si_->key_name_ = value;
                }
              else if (name == ALTKEY_TAG)
                {
                  if (value.length() > 0 &&
                      this->repo_.servers ().find (value, this->si_->alt_info_) != 0)
                    {
                      Server_Info *base_si = 0;
                      ACE_NEW (base_si, Server_Info);
                      base_si->key_name_ = value;
                      this->si_->alt_info_.reset (base_si);
                      this->repo_.servers ().bind (value, this->si_->alt_info_);
                    }
                }
              else if (name == PID_TAG)
                {
                  this->si_->pid =
                    ACE_OS::atoi (attrs->getValue (index++));
                }
              else
                {
                  this->extra_params_.push_back (std::make_pair (name, value));
                }
            }
        }
    }
  else if (ACE_OS::strcasecmp (qName, ACTIVATOR_INFO_TAG) == 0)
  {
    if (attrs != 0 && attrs->getLength () >= 3)
      {
        size_t index = 0;
        const ACE_CString aname =
          ACE_TEXT_ALWAYS_CHAR(attrs->getValue (index++));
        const ACE_TString token_str = attrs->getValue (index++);
        long token = ACE_OS::atoi (token_str.c_str ());
        const ACE_CString ior =
          ACE_TEXT_ALWAYS_CHAR(attrs->getValue (index++));
        NameValues extra_params;
        for ( ; index < attrs->getLength(); ++index)
          {
            ACE_CString name (attrs->getLocalName(index));
            ACE_CString value (attrs->getValue(index));
            extra_params.push_back (std::make_pair (name, value));
          }
        this->repo_.load_activator (aname, token, ior, extra_params);
      }
  }
  else if (ACE_OS::strcasecmp (qName, ENVIRONMENT_TAG) == 0)
    {
      if (attrs != 0 && attrs->getLength () == 2)
        {
          EnvVar ev;
          ev.name = attrs->getValue ((size_t)0);
          ev.value = attrs->getValue ((size_t)1);
          this->env_vars_.push_back (ev);
        }
    }
  else if (ACE_OS::strcasecmp (qName, PEER_TAG) == 0)
    {
      if (attrs != 0)
        {
          ACE_CString peer (attrs->getValue((size_t)0));
          this->peer_list_.push_back (peer);
        }
    }
}

void
Locator_XMLHandler::endElement (const ACEXML_Char*,
                                const ACEXML_Char*,
                                const ACEXML_Char* qName)
{
  if (ACE_OS::strcasecmp (qName, SERVER_INFO_TAG) == 0)
  {
    if (this->si_->key_name_.length () == 0)
      {
        Server_Info::gen_key (this->si_->server_id,
                              this->si_->poa_name,
                              this->si_->key_name_);
      }
    convertEnvList (this->env_vars_, this->si_->env_vars);
    convertPeerList (this->peer_list_, this->si_->peers);

    this->repo_.load_server (this->si_,
                             this->server_started_,
                             this->extra_params_);
  }
  // activator info is handled in the startElement
}

bool
Locator_XMLHandler::EnvVar::operator== (const EnvVar& rhs) const
{
  return name == rhs.name && value == rhs.value;
}
bool
Locator_XMLHandler::EnvVar::operator!= (const EnvVar& rhs) const
{
  return ! (rhs == *this);
}

