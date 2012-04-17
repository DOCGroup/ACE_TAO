// $Id$


#include "Svcconf.h"
#include "ACEXML/common/FileCharStream.h"
#include "ACEXML/common/StrCharStream.h"
#include "ACEXML/parser/parser/Parser.h"

#if (ACE_USES_CLASSIC_SVC_CONF == 0)

extern "C" ACE_Proper_Export_Flag ACE_XML_Svc_Conf *
_ACEXML_create_XML_Svc_Conf_Object (void)
{
  ACE_XML_Svc_Conf *retp = 0;

  ACE_NEW_RETURN (retp,
                  ACEXML_Svcconf_Parser (),
                  0);

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
                                0);
    }
  catch (const ACEXML_SAXException& ex)
    {
      ex.print ();              // Can't do much except printing the error.
    }
}

ACEXML_Svcconf_Parser::~ACEXML_Svcconf_Parser ()
{
}

int
ACEXML_Svcconf_Parser::parse_file (const ACE_TCHAR file[])
{
  if (file == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "ACEXML_Svcconf_Parser: No filename specified\n"), -1);

  ACEXML_FileCharStream *fstm = 0;
  ACE_NEW_RETURN (fstm,
                  ACEXML_FileCharStream (),
                  1);

  if (fstm->open (file) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("ACEXML_Svcconf_Parser: Fail to open XML file: %s\n"),
                       file),
                      -1);

  this->input_stream_.setCharStream (fstm);

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
ACEXML_Svcconf_Parser::parse_string (const ACE_TCHAR str[])
{
  if (str == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "ACEXML_Svcconf_Parser: Can't parse a null string\n"), -1);

  ACEXML_StrCharStream *stm = 0;
  ACE_NEW_RETURN (stm, ACEXML_StrCharStream, -1);
  if (stm->open (str, ACE_TEXT ("Svcconf")) < 0)
    ACE_ERROR_RETURN ((LM_ERROR, "ACEXML_Svcconf_Parser: Unable to create "
                       "input stream.\n"), -1);

  this->input_stream_.setCharStream (stm);
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
