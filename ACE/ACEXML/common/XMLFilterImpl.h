/**
 *  @file    XMLFilterImpl.h
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
#ifndef ACEXML_XMLFILTERIMPL_H
#define ACEXML_XMLFILTERIMPL_H
#include /**/ "ace/pre.h"

#include "ACEXML/common/XMLFilter.h"
#include "ACEXML/common/XMLReader.h"
#include "ACEXML/common/Locator.h"
#include "ACEXML/common/ContentHandler.h"
#include "ACEXML/common/DTDHandler.h"
#include "ACEXML/common/EntityResolver.h"
#include "ACEXML/common/ErrorHandler.h"

/**
 * @class ACEXML_XMLFilterImpl
 *
 * @brief ACEXML_XMLFilterImpl
 *
 * This class is designed to sit between an XMLReader and the client
 * application's event handlers. By default, it does nothing but pass
 * requests up to the reader and events on to the handlers unmodified, but
 * subclasses can override specific methods to modify the event stream or
 * the configuration requests as they pass through.
 */
class ACEXML_Export ACEXML_XMLFilterImpl
  : public ACEXML_XMLFilter,
    public ACEXML_ContentHandler,
    public ACEXML_DTDHandler,
    public ACEXML_EntityResolver,
    public ACEXML_ErrorHandler
{
public:
  /// Create a filter with no parent.
  ACEXML_XMLFilterImpl ();

  /**
   * Construct an XML filter with the specified parent.
   */
  explicit ACEXML_XMLFilterImpl (ACEXML_XMLReader *parent);

  ~ACEXML_XMLFilterImpl () override;
  ACEXML_XMLFilterImpl (ACEXML_XMLFilterImpl const &) = delete;
  ACEXML_XMLFilterImpl (ACEXML_XMLFilterImpl &&) = delete;
  ACEXML_XMLFilterImpl &operator= (ACEXML_XMLFilterImpl const &) = delete;
  ACEXML_XMLFilterImpl &operator= (ACEXML_XMLFilterImpl &&) = delete;

  /// Look up the value of a feature.
  int getFeature (ACEXML_Char const *name) override;

  /// Look up the value of a property.
  void * getProperty (ACEXML_Char const *name) override;

  /// Parse an XML document.
  void parse (ACEXML_InputSource *input) override;

  /// Parse an XML document from a system identifier (URI).
  void parse (ACEXML_Char const *systemId) override;

  /// Set the state of a feature.
  void setFeature (ACEXML_Char const *name, int boolean_value) override;

  /// Set the value of a property.
  void setProperty (ACEXML_Char const *name, void *value) override;

  /// Get the parent reader.
  ACEXML_XMLReader *getParent () const override;

  /// Set the parent reader.
  void setParent (ACEXML_XMLReader *parent) override;

  /// Get the current DTD event handler.
  ACEXML_DTDHandler *getDTDHandler () const override;

  /// Get the current content event handler.
  ACEXML_ContentHandler *getContentHandler () const override;

  /// Get the current entity resolver.
  ACEXML_EntityResolver *getEntityResolver () const override;

  /// Get the current error event handler.
  ACEXML_ErrorHandler *getErrorHandler () const override;

  /// Set the DTD event handler.
  void setDTDHandler (ACEXML_DTDHandler *handler) override;

  /// Set the content event handler.
  void setContentHandler (ACEXML_ContentHandler *handler) override;

  /// Set the entity resolver.
  void setEntityResolver (ACEXML_EntityResolver *handler) override;

  /// Set the error event handler.
  void setErrorHandler (ACEXML_ErrorHandler *handler) override;

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

  /// Receive notification of a notation declaration event.
  void notationDecl (ACEXML_Char const *name,
                     ACEXML_Char const *publicId,
                     ACEXML_Char const *systemId) override;

  /// Receive notification of an unparsed entity declaration event.
  void unparsedEntityDecl (ACEXML_Char const *name,
                           ACEXML_Char const *publicId,
                           ACEXML_Char const *systemId,
                           ACEXML_Char const *notationName) override;

  /// Allow the application to resolve external entities.
  ACEXML_InputSource *resolveEntity (ACEXML_Char const *publicId,
                                     ACEXML_Char const *systemId) override;

  /// Receive notification of a recoverable error.
  void error (ACEXML_SAXParseException &exception) override;

  /// Receive notification of a non-recoverable error.
  void fatalError (ACEXML_SAXParseException &exception) override;

  /// Receive notification of a warning.
  void warning (ACEXML_SAXParseException &exception) override;

protected:
  /// Configure the parent reader to send events through this filter.
  int setupParser ();

private:
  /// Non-owning pointer to the parent reader.
  ACEXML_XMLReader *parent_ {};
  /// Non-owning event handlers installed on the parent reader.
  ACEXML_EntityResolver *entityResolver_ {};
  ACEXML_DTDHandler *dtdHandler_ {};
  ACEXML_ContentHandler *contentHandler_ {};
  ACEXML_ErrorHandler *errorHandler_ {};
};

#if defined (__ACEXML_INLINE__)
# include "ACEXML/common/XMLFilterImpl.inl"
#endif /* __ACEXML_INLINE__ */

#include /**/ "ace/post.h"

#endif /* ACEXML_XMLFILTERIMPL_H */
