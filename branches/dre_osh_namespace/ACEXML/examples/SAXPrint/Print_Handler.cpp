// -*- C++ -*-  $Id$

#include "Print_Handler.h"
#include "ace/ACE.h"
#include "ace/Log_Msg.h"

#if !defined (__ACEXML_INLINE__)
# include "Print_Handler.i"
#endif /* __ACEXML_INLINE__ */

ACEXML_Print_Handler::ACEXML_Print_Handler (ACEXML_Char* fileName)
  : fileName_(ACE::strnew (fileName))
{

}

ACEXML_Print_Handler::~ACEXML_Print_Handler (void)
{
  delete[] this->fileName_;
}

void
ACEXML_Print_Handler::characters (const ACEXML_Char *cdata,
                                  int start,
                                  int length ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{


  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event characters () ** start: %d  end: %d ***************\n%s\n- End event characters () ---------------\n"),
              start, length, cdata));
}

void
ACEXML_Print_Handler::endDocument (ACEXML_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{


  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event endDocument () ***************\n")));
}

void
ACEXML_Print_Handler::endElement (const ACEXML_Char *uri,
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
ACEXML_Print_Handler::endPrefixMapping (const ACEXML_Char *prefix
                                        ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{


  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event endPrefixMapping (%s) ***************\n"),
              prefix));
}

void
ACEXML_Print_Handler::ignorableWhitespace (const ACEXML_Char *,
                                           int,
                                           int
                                           ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  //   ACE_DEBUG ((LM_DEBUG,
  //               ACE_TEXT ("* Event ignorableWhitespace () ***************\n")));
}

void
ACEXML_Print_Handler::processingInstruction (const ACEXML_Char *target,
                                             const ACEXML_Char *data
                                             ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{


  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event processingInstruction (%s, %s) ***************\n"),
              target, data));
}

void
ACEXML_Print_Handler::setDocumentLocator (ACEXML_Locator * locator)
{

  this->locator_ = locator;
  // ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("* Event setDocumentLocator () ***************\n")));
}

void
ACEXML_Print_Handler::skippedEntity (const ACEXML_Char *name
                                     ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{


  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event skippedEntity (%s) ***************\n"),
              name));
}

void
ACEXML_Print_Handler::startDocument (ACEXML_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{


  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event startDocument () ***************\n")));
}

void
ACEXML_Print_Handler::startElement (const ACEXML_Char *uri,
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
ACEXML_Print_Handler::startPrefixMapping (const ACEXML_Char * prefix,
                                          const ACEXML_Char * uri ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event startPrefixMapping () ***************\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Prefix = %s, URI = %s\n"), prefix, uri));
}

// *** Methods inherit from ACEXML_DTDHandler.

void
ACEXML_Print_Handler::notationDecl (const ACEXML_Char *name,
                                    const ACEXML_Char *publicID,
                                    const ACEXML_Char *systemID ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Event notationDecl: (%s) "),
              name));

  if (publicID == 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("SYSTEM %s\n"),
                systemID));
  else if (systemID == 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("PUBLIC %s\n"),
                publicID));
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("PUBLIC %s %s\n"),
                publicID, systemID));
}

void
ACEXML_Print_Handler::unparsedEntityDecl (const ACEXML_Char *name,
                                          const ACEXML_Char *publicID,
                                          const ACEXML_Char *systemID,
                                          const ACEXML_Char *notationName ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("* Unparsed Entity: %s"),
              name));

  if (publicID == 0)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT (" SYSTEM %s"),
                systemID));
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT (" PUBLIC %s %s"),
                publicID, systemID));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT (" NDATA %s\n"),
              notationName));
}

// Methods inherit from ACEXML_EnitityResolver.

ACEXML_InputSource *
ACEXML_Print_Handler::resolveEntity (const ACEXML_Char *,
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
ACEXML_Print_Handler::error (ACEXML_SAXParseException & ex ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_DEBUG ((LM_DEBUG, "%s: line: %d col: %d ",
              (this->locator_->getSystemId() == 0 ? this->fileName_ : this->locator_->getSystemId()),
              this->locator_->getLineNumber(),
              this->locator_->getColumnNumber()));
  ex.print();
}

void
ACEXML_Print_Handler::fatalError (ACEXML_SAXParseException& ex ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_DEBUG ((LM_DEBUG, "%s: line: %d col: %d ",
              (this->locator_->getSystemId() == 0 ? this->fileName_ : this->locator_->getSystemId()),
              this->locator_->getLineNumber(),
              this->locator_->getColumnNumber()));
  ex.print();

}

void
ACEXML_Print_Handler::warning (ACEXML_SAXParseException & ex ACEXML_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_DEBUG ((LM_DEBUG, "%s: line: %d col: %d ",
              (this->locator_->getSystemId() == 0 ? this->fileName_ : this->locator_->getSystemId()),
              this->locator_->getLineNumber(),
              this->locator_->getColumnNumber()));
  ex.print();
}
