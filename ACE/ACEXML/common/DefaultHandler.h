/**
 *  @file    DefaultHandler.h
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
#ifndef ACEXML_DEFAULTHANDLER_H
#define ACEXML_DEFAULTHANDLER_H

#include /**/ "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/ContentHandler.h"
#include "ACEXML/common/DTDHandler.h"
#include "ACEXML/common/EntityResolver.h"
#include "ACEXML/common/ErrorHandler.h"

/**
 * @class ACEXML_DefaultHandler
 *
 * @brief ACEXML_DefaultHandler
 *
 * This class is available as a convenience base class for SAX2
 * applications: it provides default implementations for all of the
 * callbacks in the four core SAX2 handler classes:
 *
 * - EntityResolver
 * - DTDHandler
 * - ContentHandler
 * - ErrorHandler
 *
 * Application writers can extend this class when they need to implement
 * only part of an interface; parser writers can instantiate this class to
 * provide default handlers when the application has not supplied its own.
 */
class ACEXML_Export ACEXML_DefaultHandler
  : public ACEXML_ContentHandler,
    public ACEXML_DTDHandler,
    public ACEXML_EntityResolver,
    public ACEXML_ErrorHandler
{
public:
  ACEXML_DefaultHandler ();

  ~ACEXML_DefaultHandler () override;

  /// @name Methods inherited from ACEXML_ContentHandler.
  /// @{

  /// Receive notification of character data.
  void characters (ACEXML_Char const *ch,
                   size_t start,
                   size_t length) override;

  /// Receive notification of the end of a document.
  void endDocument () override;

  /// Receive notification of the end of an element.
  void endElement (ACEXML_Char const *namespaceURI,
                   ACEXML_Char const *localName,
                   ACEXML_Char const *qName) override;

  /// End the scope of a prefix-URI mapping.
  void endPrefixMapping (ACEXML_Char const *prefix) override;

  /// Receive notification of ignorable whitespace in element content.
  void ignorableWhitespace (ACEXML_Char const *ch,
                            int start,
                            int length) override;

  /// Receive notification of a processing instruction.
  void processingInstruction (ACEXML_Char const *target,
                              ACEXML_Char const *data) override;

  /// Receive an object for locating the origin of SAX document events.
  void setDocumentLocator (ACEXML_Locator *locator) override;

  /// Receive notification of a skipped entity.
  void skippedEntity (ACEXML_Char const *name) override;

  /// Receive notification of the beginning of a document.
  void startDocument () override;

  /// Receive notification of the beginning of an element.
  void startElement (ACEXML_Char const *namespaceURI,
                     ACEXML_Char const *localName,
                     ACEXML_Char const *qName,
                     ACEXML_Attributes *atts) override;

  /// Begin the scope of a prefix-URI Namespace mapping.
  void startPrefixMapping (ACEXML_Char const *prefix,
                           ACEXML_Char const *uri) override;

  /// @}
  /// @name Methods inherited from ACEXML_DTDHandler.
  /// @{

  /// Receive notification of a notation declaration event.
  void notationDecl (ACEXML_Char const *name,
                     ACEXML_Char const *publicId,
                     ACEXML_Char const *systemId) override;

  /// Receive notification of an unparsed entity declaration event.
  void unparsedEntityDecl (ACEXML_Char const *name,
                           ACEXML_Char const *publicId,
                           ACEXML_Char const *systemId,
                           ACEXML_Char const *notationName) override;

  /// @}
  /// @name Methods inherited from ACEXML_EntityResolver.
  /// @{

  /// Allow the application to resolve external entities.
  ACEXML_InputSource *resolveEntity (ACEXML_Char const *publicId,
                                     ACEXML_Char const *systemId) override;

  /// @}
  /// @name Methods inherited from ACEXML_ErrorHandler.
  /// @{

  /// Receive notification of a recoverable error.
  void error (ACEXML_SAXParseException &exception) override;

  /// Receive notification of a non-recoverable error.
  void fatalError (ACEXML_SAXParseException &exception) override;

  /// Receive notification of a warning.
  void warning (ACEXML_SAXParseException &exception) override;
  /// @}
};

#include /**/ "ace/post.h"

#endif /* ACEXML_DEFAULTHANDLER_H */
