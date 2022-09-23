// -*- C++ -*-

//=============================================================================
/**
 *  @file    XMLFilterImpl.h
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================
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
  /**
   * Default constructor.  Create with no parent.
   */
  ACEXML_XMLFilterImpl ();

  /**
   * Construct an XML filter with the specified parent.
   */
  ACEXML_XMLFilterImpl (ACEXML_XMLReader *parent);

  /**
   * Destructor.
   */
  ~ACEXML_XMLFilterImpl () override;

  /*
   * Look up the value of a feature.
   */
  int getFeature (const ACEXML_Char *name) override;

  /*
   * Look up the value of a property.
   */
  void * getProperty (const ACEXML_Char *name) override;

  /*
   * Parse an XML document.
   */
  void parse (ACEXML_InputSource *input) override;

  /*
   * Parse an XML document from a system identifier (URI).
   */
  void parse (const ACEXML_Char *systemId) override;

  /*
   * Set the state of a feature.
   */
  void setFeature (const ACEXML_Char *name, int boolean_value) override;

  /*
   * Set the value of a property.
   */
  void setProperty (const ACEXML_Char *name, void *value) override;

  /*
   * Get the parent reader.
   */
  ACEXML_XMLReader *getParent () const override;

  /*
   * Set the parent reader.
   */
  void setParent (ACEXML_XMLReader *parent) override;

  /*
   * Get the current DTD event handler.
   */
  ACEXML_DTDHandler *getDTDHandler () const override;

  /*
   * Get the current content event handler.
   */
  ACEXML_ContentHandler *getContentHandler () const override;

  /*
   * Get the current entity resolver.
   */
  ACEXML_EntityResolver *getEntityResolver () const override;

  /*
   * Get the current error event handler.
   */
  ACEXML_ErrorHandler *getErrorHandler () const override;

  /*
   * Set the DTD event handler.
   */
  void setDTDHandler (ACEXML_DTDHandler *handler) override;

  /*
   * Set the content event handler.
   */
  void setContentHandler (ACEXML_ContentHandler *handler) override;

  /*
   * Set the entity resolver.
   */
  void setEntityResolver (ACEXML_EntityResolver *handler) override;

  /*
   * Set the error event handler.
   */
  void setErrorHandler (ACEXML_ErrorHandler *handler) override;

  /*
   * Receive notification of character data.
   */
  void characters (const ACEXML_Char *ch,
                           size_t start,
                           size_t length) override;

  /*
   * Receive notification of the end of a document.
   */
  void endDocument () override;

  /*
   * Receive notification of the end of an element.
   */
  void endElement (const ACEXML_Char *namespaceURI,
                           const ACEXML_Char *localName,
                           const ACEXML_Char *qName) override;

  /*
   * End the scope of a prefix-URI mapping.
   */
  void endPrefixMapping (const ACEXML_Char *prefix) override;

  /*
   * Receive notification of ignorable whitespace in element content.
   */
  void ignorableWhitespace (const ACEXML_Char *ch,
                                    int start,
                                    int length) override;

  /*
   * Receive notification of a processing instruction.
   */
  void processingInstruction (const ACEXML_Char *target,
                                      const ACEXML_Char *data) override;

  /*
   * Receive an object for locating the origin of SAX document events.
   */
  void setDocumentLocator (ACEXML_Locator *locator) override ;

  /*
   * Receive notification of a skipped entity.
   */
  void skippedEntity (const ACEXML_Char *name) override;

  /*
   * Receive notification of the beginning of a document.
   */
  void startDocument () override;

  /*
   * Receive notification of the beginning of an element.
   */
  void startElement (const ACEXML_Char *namespaceURI,
                             const ACEXML_Char *localName,
                             const ACEXML_Char *qName,
                             ACEXML_Attributes *atts) override;

  /*
   * Begin the scope of a prefix-URI Namespace mapping.
   */
  void startPrefixMapping (const ACEXML_Char *prefix,
                                   const ACEXML_Char *uri) override;

  // *** Methods inherit from ACEXML_DTDHandler.

  /*
   * Receive notification of a notation declaration event.
   */
  void notationDecl (const ACEXML_Char *name,
                             const ACEXML_Char *publicId,
                             const ACEXML_Char *systemId) override;

  /*
   * Receive notification of an unparsed entity declaration event.
   */
  void unparsedEntityDecl (const ACEXML_Char *name,
                                   const ACEXML_Char *publicId,
                                   const ACEXML_Char *systemId,
                                   const ACEXML_Char *notationName) override;

  // Methods inherit from ACEXML_EnitityResolver.

  /*
   * Allow the application to resolve external entities.
   */
  ACEXML_InputSource *resolveEntity (const ACEXML_Char *publicId,
                                             const ACEXML_Char *systemId) override;

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
protected:
  int setupParser ();
  // Set up the event handlers of parent parser to this.
  // Returns -1 if no valid parent is set.

private:
  ACEXML_XMLReader *parent_;
  // ACEXML_Locator *locator_;
  ACEXML_EntityResolver *entityResolver_;
  ACEXML_DTDHandler *dtdHandler_;
  ACEXML_ContentHandler *contentHandler_;
  ACEXML_ErrorHandler *errorHandler_;
};

#if defined (__ACEXML_INLINE__)
# include "ACEXML/common/XMLFilterImpl.inl"
#endif /* __ACEXML_INLINE__ */

#include /**/ "ace/post.h"

#endif /* ACEXML_XMLFILTERIMPL_H */
