// $Id$


#include "SvcConf.h"
#include "common/FileCharStream.h"
#include "common/StrCharStream.h"
#include "parser/parser/Parser.h"

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
}

ACEXML_Svcconf_Parser::~ACEXML_Svcconf_Parser ()
{

}

void *
ACEXML_Svcconf_Parser::operator new (size_t s)
{
  return ::new char[s];
}

void
ACEXML_Svcconf_Parser::operator delete (void *p)
{
  delete[] p;
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

  this->parser_.parse (&this->input_stream_, this->env_);
  if (this->env_.exception ())
    {
      this->env_.exception ()->print ();
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
  ACE_NEW_RETURN (stm,
                  ACEXML_StrCharStream (str),
                  -1);

  this->input_stream_.setCharStream (stm);

  this->parser_.parse (&this->input_stream_, this->env_);
  if (this->env_.exception ())
    {
      this->env_.exception ()->print ();
      return -1;
    }
  return 0;
}
