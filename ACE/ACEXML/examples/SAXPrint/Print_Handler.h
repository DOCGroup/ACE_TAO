//=============================================================================
/**
 *  @file    Print_Handler.h
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACEXML_PRINT_HANDLER_H
#define ACEXML_PRINT_HANDLER_H

#include "ACEXML/common/DefaultHandler.h"
#include <string>

/**
 * @class ACEXML_Print_Handler
 *
 * @brief ACEXML_Print_Handler is an example SAX event handler.
 *
 * This SAX event handler prints out a detailed event report
 * on every event it receives.
 */
class ACEXML_Print_Handler : public ACEXML_DefaultHandler
{
public:
  /*
   * Default constructor.
   */
  explicit ACEXML_Print_Handler (ACEXML_Char const *fileName);

  /*
   * Default destructor.
   */
  ~ACEXML_Print_Handler () override = default;

  ACEXML_Print_Handler (const ACEXML_Print_Handler&) = delete;
  ACEXML_Print_Handler (ACEXML_Print_Handler&&) = delete;
  ACEXML_Print_Handler& operator= (const ACEXML_Print_Handler&) = delete;
  ACEXML_Print_Handler& operator= (ACEXML_Print_Handler&&) = delete;

  // Methods inherited from ACEXML_ContentHandler.

  /*
   * Receive notification of character data.
   */
  void characters (ACEXML_Char const *ch,
                   size_t start,
                   size_t length) override;

  /*
   * Receive notification of the end of a document.
   */
  void endDocument () override;

  /*
   * Receive notification of the end of an element.
   */
  void endElement (ACEXML_Char const *namespaceURI,
                   ACEXML_Char const *localName,
                   ACEXML_Char const *qName) override;

  /*
   * End the scope of a prefix-URI mapping.
   */
  void endPrefixMapping (ACEXML_Char const *prefix) override;

  /*
   * Receive notification of ignorable whitespace in element content.
   */
  void ignorableWhitespace (ACEXML_Char const *ch,
                            int start,
                            int length) override;

  /*
   * Receive notification of a processing instruction.
   */
  void processingInstruction (ACEXML_Char const *target,
                              ACEXML_Char const *data) override;

  /*
   * Receive an object for locating the origin of SAX document events.
   */
  void setDocumentLocator (ACEXML_Locator *locator) override;

  /*
   * Receive notification of a skipped entity.
   */
  void skippedEntity (ACEXML_Char const *name) override;

  /*
   * Receive notification of the beginning of a document.
   */
  void startDocument () override;

  /*
   * Receive notification of the beginning of an element.
   */
  void startElement (ACEXML_Char const *namespaceURI,
                     ACEXML_Char const *localName,
                     ACEXML_Char const *qName,
                     ACEXML_Attributes *atts) override;

  /*
   * Begin the scope of a prefix-URI Namespace mapping.
   */
  void startPrefixMapping (ACEXML_Char const *prefix,
                           ACEXML_Char const *uri) override;

  // *** Methods inherit from ACEXML_DTDHandler.

  /*
   * Receive notification of a notation declaration event.
   */
  void notationDecl (ACEXML_Char const *name,
                     ACEXML_Char const *publicId,
                     ACEXML_Char const *systemId) override;

  /*
   * Receive notification of an unparsed entity declaration event.
   */
  void unparsedEntityDecl (ACEXML_Char const *name,
                           ACEXML_Char const *publicId,
                           ACEXML_Char const *systemId,
                           ACEXML_Char const *notationName) override;

  // Methods inherit from ACEXML_EnitityResolver.

  /*
   * Allow the application to resolve external entities.
   */
  ACEXML_InputSource *resolveEntity (ACEXML_Char const *publicId,
                                     ACEXML_Char const *systemId) override;

  // Methods inherit from ACEXML_ErrorHandler.

  /*
   * Receive notification of a recoverable error.
   */
  void error (ACEXML_SAXParseException &exception) override;

  /*
   * Receive notification of a non-recoverable error.
   */
  void fatalError (ACEXML_SAXParseException &exception) override;

  /*
   * Receive notification of a warning.
   */
  void warning (ACEXML_SAXParseException &exception) override;
private:
  std::basic_string<ACEXML_Char> file_name_;
  ACEXML_Locator* locator_; // Non-owning; the parser owns the locator.
};

#endif /* ACEXML_PRINT_HANDLER_H */
