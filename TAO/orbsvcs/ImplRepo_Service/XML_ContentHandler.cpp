#include "XML_ContentHandler.h"
#include "ace/OS_NS_strings.h"
#include "ace/Log_Msg.h"

ACE_RCSID (ImplRepo_Service,
           XML_ContentHandler,
           "$Id$")

const char* XML_ContentHandler::ROOT_TAG = "SERVER_INFOS";
const char* XML_ContentHandler::SERVER_INFO_TAG = "SERVER_INFORMATION";
const char* XML_ContentHandler::SERVER_TAG = "Server";
const char* XML_ContentHandler::STARTUP_OPTIONS_TAG = "StartupOptions";
const char* XML_ContentHandler::COMMAND_LINE_TAG = "Command_Line";
const char* XML_ContentHandler::WORKING_DIR_TAG = "WorkingDir";
const char* XML_ContentHandler::ACTIVATION_TAG = "Activation";
const char* XML_ContentHandler::ENVIRONMENT_TAG = "Environment";
const char* XML_ContentHandler::PARTIAL_IOR_TAG = "PartialIOR";
const char* XML_ContentHandler::IOR_TAG = "ServerObjectIOR";

XML_ContentHandler::XML_ContentHandler (Callback& cb)
: callback_(cb)
, tag_state_(tsInvalid)
{
}

void
XML_ContentHandler::startElement (const ACEXML_Char*,
                                  const ACEXML_Char*,
                                  const ACEXML_Char* qName,
                                  ACEXML_Attributes* attrs ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_ASSERT(qName != 0);
  if (ACE_OS::strcasecmp (qName, SERVER_INFO_TAG) == 0) 
  {
    this->reset_server_info();
    this->tag_state_ = tsServerInfo;
  }
  else if (ACE_OS::strcasecmp (qName, SERVER_TAG) == 0)
  {
    tag_state_ = tsServer;
  }
  else if (ACE_OS::strcasecmp (qName, COMMAND_LINE_TAG) == 0)
  {
    tag_state_ = tsCommandLine;
  }
  else if (ACE_OS::strcasecmp (qName, WORKING_DIR_TAG) == 0)
  {
    tag_state_ = tsWorkingDir;
  }
  else if (ACE_OS::strcasecmp (qName, ACTIVATION_TAG) == 0)
  {
    tag_state_ = tsActivation;
  }
  else if (ACE_OS::strcasecmp (qName, ENVIRONMENT_TAG) == 0)
  {
    tag_state_ = tsEnvironment;
    if (attrs != 0 && attrs->getLength() == 2) 
    {
      this->env_vars_ += "name=\"";
      this->env_vars_ += attrs->getValue((size_t)0);
      this->env_vars_ += "\" value=\"";
      this->env_vars_ += attrs->getValue((size_t)1);
      this->env_vars_ += "\"\n";
    }
  }
  else if (ACE_OS::strcasecmp (qName, PARTIAL_IOR_TAG) == 0)
  {
    tag_state_ = tsLocation;
  }
  else if (ACE_OS::strcasecmp (qName, IOR_TAG) == 0)
  {
    tag_state_ = tsIOR;
  }
}

void
XML_ContentHandler::endElement (const ACEXML_Char*,
                                const ACEXML_Char*,
                                const ACEXML_Char* qName ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_ASSERT(qName != 0);
  this->tag_state_ = tsInvalid;
  if (ACE_OS::strcasecmp (qName, SERVER_INFO_TAG) == 0) 
  {
    this->callback_.next_server(this->server_name_, this->command_line_, 
      this->env_vars_, this->working_dir_, this->activation_, 
      this->partial_ior_, this->server_object_ior_);
    this->reset_server_info();
  }
}

void
XML_ContentHandler::characters (const ACEXML_Char* cdata,
                                int,
                                int 
                                ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_ASSERT(cdata != 0);
  switch (this->tag_state_) 
  {
  case tsServer:
    this->server_name_ = cdata;
    break;
  case tsCommandLine:
    this->command_line_ = cdata;
    break;
  case tsWorkingDir:
    this->working_dir_ = cdata;
    break;
  case tsActivation:
    this->activation_ = cdata;
    break;
  case tsLocation:
    this->partial_ior_ = cdata;
    break;
  case tsIOR:
    this->server_object_ior_ = cdata;
    break;
  default:
    break;// ignore tsServerInfo, tsInvalid, tsStartupOptions, and anything else
  }
}

void
XML_ContentHandler::reset_server_info (void) 
{
  this->server_name_ = "";
  this->command_line_ = "";
  this->activation_ = "";
  this->working_dir_ = "";
  this->env_vars_ = "";
  this->server_object_ior_ = "";
  this->partial_ior_ = "";
  this->tag_state_ = tsInvalid;
}
