#include "XML_ContentHandler.h"


ACE_RCSID (ImplRepo_Service,
           XML_ContentHandler,
           "$Id$")


XML_ContentHandler::XML_ContentHandler (const char *server_name)
  : server_name_ (server_name),
    found_server_entry_ (false),
    command_line_entry_ (false),
    working_dir_entry_ (false),
    environment_vars_entry_ (false),
    activation_entry_ (false),
    server_object_ior_entry_ (false),
    location_entry_ (false),
    startup_value_ (false)
{
  // no-op
}

XML_ContentHandler::~XML_ContentHandler (void)
{
  // no-op
}

void
XML_ContentHandler::characters (const ACEXML_Char *cdata,
                                int /* start */,
                                int /* end */
                                ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (ACE_OS::strcmp (cdata, this->server_name_.c_str ()) == 0)
    {
      this->found_server_entry_ = true;
    }

  if (this->command_line_entry_ )
    {
      this->command_line_ = cdata;
      this->command_line_entry_ = false;
    }
  else if (this->working_dir_entry_ )
    {
      this->working_dir_ = cdata;
      this->working_dir_entry_ = false;
    }
  else if (this->activation_entry_ )
    {
      this->activation_ = cdata;
      this->activation_entry_ = false;
    }
  else if (this->environment_vars_entry_ )
    {
      this->environment_vars_ = cdata;
      this->environment_vars_entry_ = false;
    }
  else if (this->location_entry_ )
    {
      this->location_ = cdata;
      this->location_entry_ = false;
    }
  else if (this->server_object_ior_entry_ )
    {
      this->server_object_ior_ = cdata;
      this->server_object_ior_entry_ = false;
    }
}

void
XML_ContentHandler::endDocument ( ACEXML_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
}

void
XML_ContentHandler::endElement (const ACEXML_Char *,
                                const ACEXML_Char *,
                                const ACEXML_Char * /* qName */ ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
}

void
XML_ContentHandler::endPrefixMapping (const ACEXML_Char * /* prefix */ ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  //  cout << "* Event endPrefixMapping (" << prefix << ") ***************" << endl;
}

void
XML_ContentHandler::ignorableWhitespace (const ACEXML_Char *,
                                         int,
                                         int ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  //  cout << "* Event ignorableWhitespace () ***************" << endl;
}

void
XML_ContentHandler::processingInstruction (const ACEXML_Char * /* target */,
                                           const ACEXML_Char * /* data */ ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  //  this->print_indent ();
//   cout << "<?" << target << " "
//        << data << "?>" << endl;
}

void
XML_ContentHandler::setDocumentLocator (ACEXML_Locator *)
{
  //  cout << "* Event setDocumentLocator () ***************" << endl;
}

void
XML_ContentHandler::skippedEntity (const ACEXML_Char * /* name */ ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  //  cout << "* Event skippedEntity (" << name << ") ***************" << endl;
}

void
XML_ContentHandler::startDocument ( ACEXML_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  //  cout << "* Event startDocument () ***************" << endl;
}

void
XML_ContentHandler::startElement (const ACEXML_Char *,
                                  const ACEXML_Char *,
                                  const ACEXML_Char *qName,
                                  ACEXML_Attributes * ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  if (this->found_server_entry_ )
    {
      if (ACE_OS::strcmp (qName, "Command_Line") == 0)
        {
          this->command_line_entry_ = true;
        }
      else if (ACE_OS::strcmp (qName, "WorkingDir") == 0)
        {
          this->working_dir_entry_ = true;
        }
      else if (ACE_OS::strcmp (qName, "Activation") == 0)
        {
          this->activation_entry_ = true;
        }
      else if (ACE_OS::strcmp (qName, "Command_Line") == 0)
        {
          this->environment_vars_entry_ = true;
        }
      else if (ACE_OS::strcmp (qName, "Location") == 0)
        {
          this->location_entry_ = true;
        }
      else if (ACE_OS::strcmp (qName, "Server_Object_IOR") == 0)
        {
          this->server_object_ior_entry_ = true;
        }
    }
}

void
XML_ContentHandler::startPrefixMapping (const ACEXML_Char *,
                                        const ACEXML_Char * ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

// *** Methods inherit from ACEXML_DTDHandler.

void
XML_ContentHandler::notationDecl (const ACEXML_Char *,
                                       const ACEXML_Char *,
                                       const ACEXML_Char * ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
XML_ContentHandler::unparsedEntityDecl (const ACEXML_Char *,
                                             const ACEXML_Char *,
                                             const ACEXML_Char *,
                                             const ACEXML_Char * ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

// Methods inherit from ACEXML_EnitityResolver.

ACEXML_InputSource *
XML_ContentHandler::resolveEntity (const ACEXML_Char *,
                                   const ACEXML_Char * ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
  return 0;
}

// Methods inherit from ACEXML_ErrorHandler.

  /*
   * Receive notification of a recoverable error.
   */
void
XML_ContentHandler::error (ACEXML_SAXParseException & ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
XML_ContentHandler::fatalError (ACEXML_SAXParseException & ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
XML_ContentHandler::warning (ACEXML_SAXParseException & ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

int
XML_ContentHandler::get_startup_information (ACE_CString &logical_server_name,
                                             ACE_CString &startup_command,
                                             // ImplementationRepository::EnvironmentList
                                             //&environment_vars,
                                             ACE_CString &working_dir,
                                             //ImplementationRepository::ActivationMode
                                             ACE_CString &activation)
{
  startup_command = this->command_line_;

  logical_server_name = this->server_name_;

  working_dir = this->working_dir_;

  activation = this->activation_;

  return 0;
}

int
XML_ContentHandler::get_running_information (ACE_CString POA_name,
                                             ACE_CString &location,
                                             ACE_CString &server_object_ior)
{
  if (ACE_OS::strcmp (POA_name.c_str (), this->server_name_.c_str ()) == 0)
    {
      location = this->location_;
      server_object_ior = this->server_object_ior_;
    }
  return 0;
}

void
XML_ContentHandler::set_startup_value (ACE_CString POA_name,
                                       int new_value)
{
  this->server_name_ = POA_name;
  this->startup_value_ = new_value != 0;
}

void
XML_ContentHandler::get_startup_value (ACE_CString /* POA_name */,
                                       int &new_value)
{
  new_value = this->startup_value_ ? 1 : 0;
}

void
XML_ContentHandler::update_running_information (ACE_CString /* POA_name */,
                                                ACE_CString location,
                                                ACE_CString server_object_ior)
{
  this->location_ = location;
  this->server_object_ior_ = server_object_ior;
}
