// -*- C++ -*-  $Id$

#include "Test_Handler.h"
#include "ace/ACE.h"
#include "ace/Log_Msg.h"

// TODO Create test_config_t's based on XML using entities described in testconfig.dtd

Test_Handler::Test_Handler (ACEXML_Char* fileName)
  : configs_(0),
    fileName_(ACE::strnew (fileName))
{

}

Test_Handler::~Test_Handler (void)
{
  delete[] this->fileName_;

  for(size_t i=0; i<configs_.size(); ++i) {
    delete configs_[i];
  }
}

const TestConfig::Test_Config_Set &
Test_Handler::get_configs (void) const
{
  return this->configs_;
}


void
Test_Handler::characters (const ACEXML_Char *cdata,
                                  int start,
                                  int end ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{


  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event characters () ** start: %d  end: %d ***************\n%s\n- End event characters () ---------------\n"),
              start, end, cdata));
}

void
Test_Handler::endDocument (ACEXML_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{


  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event endDocument () ***************\n")));
}

void
Test_Handler::endElement (const ACEXML_Char *uri,
                                  const ACEXML_Char *name,
                                  const ACEXML_Char *qName
                                  ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{


  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event endElement (%s, %s, %s) ***************\n"),
              uri, name, qName));
}

void
Test_Handler::endPrefixMapping (const ACEXML_Char *prefix
                                        ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{


  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event endPrefixMapping (%s) ***************\n"),
              prefix));
}

void
Test_Handler::ignorableWhitespace (const ACEXML_Char *,
                                           int,
                                           int
                                           ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{


  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event ignorableWhitespace () ***************\n")));
}

void
Test_Handler::processingInstruction (const ACEXML_Char *target,
                                             const ACEXML_Char *data
                                             ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{


  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event processingInstruction (%s, %s) ***************\n"),
              target, data));
}

void
Test_Handler::setDocumentLocator (ACEXML_Locator * locator)
{

  this->locator_ = locator;
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event setDocumentLocator () ***************\n")));
}

void
Test_Handler::skippedEntity (const ACEXML_Char *name
                                     ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{


  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event skippedEntity (%s) ***************\n"),
              name));
}

void
Test_Handler::startDocument (ACEXML_ENV_SINGLE_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{


  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event startDocument () ***************\n")));
}

void
Test_Handler::startElement (const ACEXML_Char *uri,
                                    const ACEXML_Char *name,
                                    const ACEXML_Char *qName,
                                    ACEXML_Attributes *alist
                                    ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{


  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event startElement (%s, %s, %s) ***************\n"),
              uri, name, qName));

  if (alist != 0)
    for (size_t i = 0; i < alist->getLength (); ++i)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("    %s = \"%s\"\n"),
                    alist->getQName (i), alist->getValue (i)));
      }
}

void
Test_Handler::startPrefixMapping (const ACEXML_Char * prefix,
                                          const ACEXML_Char * uri ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event startPrefixMapping () ***************\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Prefix = %s, URI = %s\n"), prefix, uri));
}

// Methods inherited from ACEXML_ErrorHandler.

/*
 * Receive notification of a recoverable error.
 */
void
Test_Handler::error (ACEXML_SAXParseException & ex ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{

  ACE_DEBUG ((LM_DEBUG, "%s:%d:%d ", this->fileName_,
              this->locator_->getLineNumber(),
              this->locator_->getColumnNumber()));
  ex.print();
}

void
Test_Handler::fatalError (ACEXML_SAXParseException& ex ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{

  ACE_DEBUG ((LM_DEBUG, "%s:%d:%d ", this->fileName_,
              this->locator_->getLineNumber(),
              this->locator_->getColumnNumber()));
  ex.print();
}

void
Test_Handler::warning (ACEXML_SAXParseException & ACEXML_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}
