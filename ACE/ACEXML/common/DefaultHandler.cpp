#include "ACEXML/common/DefaultHandler.h"

ACEXML_DefaultHandler::ACEXML_DefaultHandler () = default;

ACEXML_DefaultHandler::~ACEXML_DefaultHandler () = default;

void
ACEXML_DefaultHandler::characters (ACEXML_Char const *,
                                   size_t,
                                   size_t)
{
}

void
ACEXML_DefaultHandler::endDocument ()
{
}

void
ACEXML_DefaultHandler::endElement (ACEXML_Char const *,
                                   ACEXML_Char const *,
                                   ACEXML_Char const *)
{
}

void
ACEXML_DefaultHandler::endPrefixMapping (ACEXML_Char const *)
{
  // No-op.
}

void
ACEXML_DefaultHandler::ignorableWhitespace (ACEXML_Char const *,
                                            int,
                                            int)
{
  // No-op.
}

void
ACEXML_DefaultHandler::processingInstruction (ACEXML_Char const *,
                                              ACEXML_Char const *)
{
  // No-op.
}

void
ACEXML_DefaultHandler::setDocumentLocator (ACEXML_Locator * )
{
  // No-op.
}

void
ACEXML_DefaultHandler::skippedEntity (ACEXML_Char const *)
{
  // No-op.
}

void
ACEXML_DefaultHandler::startDocument ()
{
  // No-op.
}

void
ACEXML_DefaultHandler::startElement (ACEXML_Char const *,
                                     ACEXML_Char const *,
                                     ACEXML_Char const *,
                                     ACEXML_Attributes *)
{
  // No-op.
}

void
ACEXML_DefaultHandler::startPrefixMapping (ACEXML_Char const *,
                                           ACEXML_Char const *)
{
  // No-op.
}

  // *** Methods inherited from ACEXML_DTDHandler.

void
ACEXML_DefaultHandler::notationDecl (ACEXML_Char const *,
                                     ACEXML_Char const *,
                                     ACEXML_Char const *)
{
  // No-op.
}

void
ACEXML_DefaultHandler::unparsedEntityDecl (ACEXML_Char const *,
                                           ACEXML_Char const *,
                                           ACEXML_Char const *,
                                           ACEXML_Char const *)
{
  // No-op.
}

  // Methods inherited from ACEXML_EntityResolver.

ACEXML_InputSource *
ACEXML_DefaultHandler::resolveEntity (ACEXML_Char const *,
                                      ACEXML_Char const * )
{
  // No-op.
  return nullptr;
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
