
#include "Svcconf.h"
#include "ACEXML/common/FileCharStream.h"
#include "ACEXML/common/StrCharStream.h"
#include "ACEXML/parser/parser/Parser.h"
#include <memory>

#if (ACE_USES_CLASSIC_SVC_CONF == 0)

extern "C" ACE_Proper_Export_Flag ACE_XML_Svc_Conf *
_ACEXML_create_XML_Svc_Conf_Object ()
{
  ACE_XML_Svc_Conf *retp = nullptr;

  ACE_NEW_RETURN (retp,
                  ACEXML_Svcconf_Parser (),
                  nullptr);

  return retp;
}

ACEXML_Svcconf_Parser::ACEXML_Svcconf_Parser ()
{
  this->parser_.setContentHandler (&this->svcconf_handler_);
  this->parser_.setDTDHandler (&this->svcconf_handler_);
  this->parser_.setErrorHandler (&this->svcconf_handler_);
  this->parser_.setEntityResolver (&this->svcconf_handler_);
  try
    {
      this->parser_.setFeature (ACE_TEXT ("http://xml.org/sax/features/validation"),
                                false);
    }
  catch (const ACEXML_SAXException& ex)
    {
      ex.print ();              // Can't do much except printing the error.
    }
}

int
ACEXML_Svcconf_Parser::parse_file (ACE_TCHAR const file[])
{
  if (file == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR, "ACEXML_Svcconf_Parser: No filename specified\n"), -1);

  std::unique_ptr<ACEXML_FileCharStream> fstm =
    std::make_unique<ACEXML_FileCharStream> ();

  if (fstm->open (file) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("ACEXML_Svcconf_Parser: Fail to open XML file: %s\n"),
                       file),
                      -1);

  this->input_stream_.setCharStream (fstm.release ());

  try
    {
      this->parser_.parse (&this->input_stream_);
    }
  catch (const ACEXML_SAXException& ex)
    {
      ex.print ();
      return -1;
    }
  return 0;
}


int
ACEXML_Svcconf_Parser::parse_string (ACE_TCHAR const str[])
{
  if (str == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR, "ACEXML_Svcconf_Parser: Can't parse a null string\n"), -1);

  std::unique_ptr<ACEXML_StrCharStream> stm =
    std::make_unique<ACEXML_StrCharStream> ();
  if (stm->open (str, ACE_TEXT ("Svcconf")) < 0)
    ACE_ERROR_RETURN ((LM_ERROR, "ACEXML_Svcconf_Parser: Unable to create "
                       "input stream.\n"), -1);

  this->input_stream_.setCharStream (stm.release ());
  try
    {
      this->parser_.parse (&this->input_stream_);
    }
  catch (const ACEXML_SAXException& ex)
    {
      // If there was a problem parsing the stream, set the errno
      // to EINVAL to indicate to upper levels that the stream was
      // invalid.
      ACE_OS::last_error (EINVAL);
      ex.print ();
      return -1;
    }
  return 0;
}

#endif /* ACE_USES_CLASSIC_SVC_CONF == 0 */
