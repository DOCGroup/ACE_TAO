// -*- C++ -*-  $Id$

#include "common/DefaultHandler.h"
#if !defined (__ACEXML_INLINE__)
# include "common/DefaultHandler.i"
#endif /* __ACEXML_INLINE__ */

ACEXML_DefaultHandler::ACEXML_DefaultHandler (void)
{
  // no-op
}

ACEXML_DefaultHandler::~ACEXML_DefaultHandler (void)
{
  // no-op
}

void
ACEXML_DefaultHandler::characters (const ACEXML_Char *,
                                   int,
                                   int,
                                   ACEXML_Env &)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_DefaultHandler::endDocument (ACEXML_Env &)
  //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_DefaultHandler::endElement (const ACEXML_Char *,
                                   const ACEXML_Char *,
                                   const ACEXML_Char *,
                                   ACEXML_Env &)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_DefaultHandler::endPrefixMapping (const ACEXML_Char *,
                                         ACEXML_Env &)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_DefaultHandler::ignorableWhitespace (const ACEXML_Char *,
                                            int,
                                            int,
                                            ACEXML_Env &)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_DefaultHandler::processingInstruction (const ACEXML_Char *,
                                              const ACEXML_Char *,
                                              ACEXML_Env &)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_DefaultHandler::setDocumentLocator (ACEXML_Locator *,
                                           ACEXML_Env &)
{
  // No-op.
}

void
ACEXML_DefaultHandler::skippedEntity (const ACEXML_Char *,
                                      ACEXML_Env &)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_DefaultHandler::startDocument (ACEXML_Env &)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_DefaultHandler::startElement (const ACEXML_Char *,
                                     const ACEXML_Char *,
                                     const ACEXML_Char *,
                                     ACEXML_Attributes *,
                                     ACEXML_Env &)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_DefaultHandler::startPrefixMapping (const ACEXML_Char *,
                                           const ACEXML_Char *,
                                           ACEXML_Env &)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

  // *** Methods inherit from ACEXML_DTDHandler.

void
ACEXML_DefaultHandler::notationDecl (const ACEXML_Char *,
                                     const ACEXML_Char *,
                                     const ACEXML_Char *,
                                     ACEXML_Env &)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_DefaultHandler::unparsedEntityDecl (const ACEXML_Char *,
                                           const ACEXML_Char *,
                                           const ACEXML_Char *,
                                           const ACEXML_Char *,
                                           ACEXML_Env &)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

  // Methods inherit from ACEXML_EnitityResolver.

ACEXML_InputSource *
ACEXML_DefaultHandler::resolveEntity (const ACEXML_Char *,
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
ACEXML_DefaultHandler::error (ACEXML_SAXParseException &,
                              ACEXML_Env &)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_DefaultHandler::fatalError (ACEXML_SAXParseException &,
                                   ACEXML_Env &)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}

void
ACEXML_DefaultHandler::warning (ACEXML_SAXParseException &,
                                ACEXML_Env &)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
{
  // No-op.
}
