// -*- C++ -*-  $Id$

#include "Print_Handler.h"
#include "ace/ACE.h"
#if !defined (__ACEXML_INLINE__)
# include "Print_Handler.i"
#endif /* __ACEXML_INLINE__ */

ACEXML_Print_Handler::ACEXML_Print_Handler (void)
{
  // no-op
}

ACEXML_Print_Handler::~ACEXML_Print_Handler (void)
{
  // no-op
}

void
ACEXML_Print_Handler::characters (const ACEXML_Char *cdata,
                                  int start,
                                  int end,
                                  ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG (xmlenv);

  cout << "* Event characters () ** start: " << start
       << "  end: " << end << " ***************" << endl;
  cout << cdata << endl;
  cout << "- End event characters () ---------------" << endl;
}

void
ACEXML_Print_Handler::endDocument (ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG (xmlenv);

  cout << "* Event endDocument () ***************" << endl;
}

void
ACEXML_Print_Handler::endElement (const ACEXML_Char *uri,
                                  const ACEXML_Char *name,
                                  const ACEXML_Char *qName,
                                  ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG (xmlenv);

  cout << "* Event endElement (" << uri << ", " << name << ", "
       << qName << ") ***************" << endl;
}

void
ACEXML_Print_Handler::endPrefixMapping (const ACEXML_Char *prefix,
                                        ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG (xmlenv);

  cout << "* Event endPrefixMapping (" << prefix << ") ***************" << endl;
}

void
ACEXML_Print_Handler::ignorableWhitespace (const ACEXML_Char *,
                                           int,
                                           int,
                                           ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG (xmlenv);

  cout << "* Event ignorableWhitespace () ***************" << endl;
}

void
ACEXML_Print_Handler::processingInstruction (const ACEXML_Char *target,
                                             const ACEXML_Char *data,
                                             ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG (xmlenv);

  cout << "* Event processingInstruction (" << target << ", "
       << data << ") ***************" << endl;
}

void
ACEXML_Print_Handler::setDocumentLocator (ACEXML_Locator *,
                                          ACEXML_Env &xmlenv)
{
  ACE_UNUSED_ARG (xmlenv);

  cout << "* Event setDocumentLocator () ***************" << endl;
}

void
ACEXML_Print_Handler::skippedEntity (const ACEXML_Char *name,
                                     ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG (xmlenv);

  cout << "* Event skippedEntity (" << name << ") ***************" << endl;
}

void
ACEXML_Print_Handler::startDocument (ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG (xmlenv);

  cout << "* Event startDocument () ***************" << endl;
}

void
ACEXML_Print_Handler::startElement (const ACEXML_Char *uri,
                                    const ACEXML_Char *name,
                                    const ACEXML_Char *qName,
                                    ACEXML_Attributes *alist,
                                    ACEXML_Env &xmlenv)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  ACE_UNUSED_ARG (xmlenv);

  cout << "* Event startElement (" << uri << ", "
       << name << ", " << qName << ") ***************" << endl;
  if (alist != 0)
    for (size_t i = 0; i < alist->getLength (); ++i)
      {
        cout << "   " << alist->getQName (i) << " = \""
             << alist->getValue (i) << "\"" << endl;
      }
}

void
ACEXML_Print_Handler::startPrefixMapping (const ACEXML_Char *,
                                          const ACEXML_Char *,
                                          ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

// *** Methods inherit from ACEXML_DTDHandler.

void
ACEXML_Print_Handler::notationDecl (const ACEXML_Char *name,
                                    const ACEXML_Char *publicID,
                                    const ACEXML_Char *systemID,
                                    ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  cout << "* Event notationDecl: (" << name << ") ";

  if (publicID == 0)
    cout << "SYSTEM " << systemID << endl;
  else if (systemID == 0)
    cout << "PUBLIC " << publicID << endl;
  else
    cout << "PUBLIC " << publicID << " " << systemID << endl;
}

void
ACEXML_Print_Handler::unparsedEntityDecl (const ACEXML_Char *name,
                                          const ACEXML_Char *publicID,
                                          const ACEXML_Char *systemID,
                                          const ACEXML_Char *notationName,
                                          ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  cout << "* Unparsed Entity: " << name;

  if (publicID == 0)
    cout << " SYSTEM " << systemID;
  else
    cout << " PUBLIC " << publicID << " " << systemID;

  cout << " NDATA " << notationName << endl;
}

// Methods inherit from ACEXML_EnitityResolver.

ACEXML_InputSource *
ACEXML_Print_Handler::resolveEntity (const ACEXML_Char *,
                                     const ACEXML_Char *,
                                     ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
  return 0;
}

// Methods inherit from ACEXML_ErrorHandler.

  /*
   * Receive notification of a recoverable error.
   */
void
ACEXML_Print_Handler::error (ACEXML_SAXParseException &,
                             ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_Print_Handler::fatalError (ACEXML_SAXParseException &,
                                  ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_Print_Handler::warning (ACEXML_SAXParseException &,
                               ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}
