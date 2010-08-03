// -*- C++ -*-  $Id$

#include "ACEXML/common/DefaultHandler.h"

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
                                   size_t,
                                   size_t)
{
  // No-op.
}

void
ACEXML_DefaultHandler::endDocument (void)
{
  // No-op.
}

void
ACEXML_DefaultHandler::endElement (const ACEXML_Char *,
                                   const ACEXML_Char *,
                                   const ACEXML_Char *)
{
  // No-op.
}

void
ACEXML_DefaultHandler::endPrefixMapping (const ACEXML_Char *)
{
  // No-op.
}

void
ACEXML_DefaultHandler::ignorableWhitespace (const ACEXML_Char *,
                                            int,
                                            int)
{
  // No-op.
}

void
ACEXML_DefaultHandler::processingInstruction (const ACEXML_Char *,
                                              const ACEXML_Char *)
{
  // No-op.
}

void
ACEXML_DefaultHandler::setDocumentLocator (ACEXML_Locator * )
{
  // No-op.
}

void
ACEXML_DefaultHandler::skippedEntity (const ACEXML_Char *)
{
  // No-op.
}

void
ACEXML_DefaultHandler::startDocument (void)
{
  // No-op.
}

void
ACEXML_DefaultHandler::startElement (const ACEXML_Char *,
                                     const ACEXML_Char *,
                                     const ACEXML_Char *,
                                     ACEXML_Attributes *)
{
  // No-op.
}

void
ACEXML_DefaultHandler::startPrefixMapping (const ACEXML_Char *,
                                           const ACEXML_Char *)
{
  // No-op.
}

  // *** Methods inherited from ACEXML_DTDHandler.

void
ACEXML_DefaultHandler::notationDecl (const ACEXML_Char *,
                                     const ACEXML_Char *,
                                     const ACEXML_Char *)
{
  // No-op.
}

void
ACEXML_DefaultHandler::unparsedEntityDecl (const ACEXML_Char *,
                                           const ACEXML_Char *,
                                           const ACEXML_Char *,
                                           const ACEXML_Char *)
{
  // No-op.
}

  // Methods inherited from ACEXML_EntityResolver.

ACEXML_InputSource *
ACEXML_DefaultHandler::resolveEntity (const ACEXML_Char *,
                                      const ACEXML_Char * )
{
  // No-op.
  return 0;
}

  // Methods inherited from ACEXML_ErrorHandler.

  /*
   * Receive notification of a recoverable error.
   */
void
ACEXML_DefaultHandler::error (ACEXML_SAXParseException &)
{
  // No-op.
}

void
ACEXML_DefaultHandler::fatalError (ACEXML_SAXParseException &)
{
  // No-op.
}

void
ACEXML_DefaultHandler::warning (ACEXML_SAXParseException &)
{
  // No-op.
}
