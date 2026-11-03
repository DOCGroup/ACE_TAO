//=============================================================================
/**
 *  @file    SAXPrint_Handler.h
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACEXML_SAXPRINT_HANDLER_H
#define ACEXML_SAXPRINT_HANDLER_H

#include "ACEXML/common/DefaultHandler.h"
#include <string>

/**
 * @class ACEXML_SAXPrint_Handler
 *
 * @brief ACEXML_SAXPrint_Handler is an example SAX event handler.
 *
 * This SAX event handler try to regenerate the XML document it
 * reads with correct indentation.
 */
class ACEXML_SAXPrint_Handler : public ACEXML_DefaultHandler
{
public:
  /*
   * Default constructor.
   */
  explicit ACEXML_SAXPrint_Handler (ACEXML_Char const *name);

  /*
   * Default destructor.
   */
  ~ACEXML_SAXPrint_Handler () override = default;

  ACEXML_SAXPrint_Handler (const ACEXML_SAXPrint_Handler&) = delete;
  ACEXML_SAXPrint_Handler (ACEXML_SAXPrint_Handler&&) = delete;
  ACEXML_SAXPrint_Handler& operator= (const ACEXML_SAXPrint_Handler&) = delete;
  ACEXML_SAXPrint_Handler& operator= (ACEXML_SAXPrint_Handler&&) = delete;

  // Methods inherit from ACEXML_ContentHandler.

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

  void inc_indent ();
  void dec_indent ();
  void print_indent ();

private:
  size_t indent_;
  std::basic_string<ACEXML_Char> file_name_;
  ACEXML_Locator* locator_; // Non-owning; the parser owns the locator.
};

#if defined (__ACEXML_INLINE__)
# include "SAXPrint_Handler.inl"
#endif /* __ACEXML_INLINE__ */
#endif /* ACEXML_SAXPRINT_HANDLER_H */
