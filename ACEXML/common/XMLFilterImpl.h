// -*- C++ -*-

//=============================================================================
/**
 *  @file    XMLFilterImpl.h
 *
 *  $Id$
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
  ACEXML_XMLFilterImpl (void);

  /**
   * Construct an XML filter with the specified parent.
   */
  ACEXML_XMLFilterImpl (ACEXML_XMLReader *parent);

  /**
   * Destructor.
   */
  virtual ~ACEXML_XMLFilterImpl (void);

  /*
   * Look up the value of a feature.
   */
  virtual int getFeature (const ACEXML_Char *name);

  /*
   * Look up the value of a property.
   */
  virtual void * getProperty (const ACEXML_Char *name);

  /*
   * Parse an XML document.
   */
  virtual void parse (ACEXML_InputSource *input);

  /*
   * Parse an XML document from a system identifier (URI).
   */
  virtual void parse (const ACEXML_Char *systemId);

  /*
   * Set the state of a feature.
   */
  virtual void setFeature (const ACEXML_Char *name, int boolean_value);

  /*
   * Set the value of a property.
   */
  virtual void setProperty (const ACEXML_Char *name, void *value);

  /*
   * Get the parent reader.
   */
  virtual ACEXML_XMLReader *getParent (void) const;

  /*
   * Set the parent reader.
   */
  virtual void setParent (ACEXML_XMLReader *parent);

  /*
   * Get the current DTD event handler.
   */
  virtual ACEXML_DTDHandler *getDTDHandler (void) const;

  /*
   * Get the current content event handler.
   */
  virtual ACEXML_ContentHandler *getContentHandler (void) const;

  /*
   * Get the current entity resolver.
   */
  virtual ACEXML_EntityResolver *getEntityResolver (void) const;

  /*
   * Get the current error event handler.
   */
  virtual ACEXML_ErrorHandler *getErrorHandler (void) const;

  /*
   * Set the DTD event handler.
   */
  virtual void setDTDHandler (ACEXML_DTDHandler *handler);

  /*
   * Set the content event handler.
   */
  virtual void setContentHandler (ACEXML_ContentHandler *handler);

  /*
   * Set the entity resolver.
   */
  virtual void setEntityResolver (ACEXML_EntityResolver *handler);

  /*
   * Set the error event handler.
   */
  virtual void setErrorHandler (ACEXML_ErrorHandler *handler);

  /*
   * Receive notification of character data.
   */
  virtual void characters (const ACEXML_Char *ch,
                           size_t start,
                           size_t length);

  /*
   * Receive notification of the end of a document.
   */
  virtual void endDocument (void);

  /*
   * Receive notification of the end of an element.
   */
  virtual void endElement (const ACEXML_Char *namespaceURI,
                           const ACEXML_Char *localName,
                           const ACEXML_Char *qName);

  /*
   * End the scope of a prefix-URI mapping.
   */
  virtual void endPrefixMapping (const ACEXML_Char *prefix);

  /*
   * Receive notification of ignorable whitespace in element content.
   */
  virtual void ignorableWhitespace (const ACEXML_Char *ch,
                                    int start,
                                    int length);

  /*
   * Receive notification of a processing instruction.
   */
  virtual void processingInstruction (const ACEXML_Char *target,
                                      const ACEXML_Char *data);

  /*
   * Receive an object for locating the origin of SAX document events.
   */
  virtual void setDocumentLocator (ACEXML_Locator *locator) ;

  /*
   * Receive notification of a skipped entity.
   */
  virtual void skippedEntity (const ACEXML_Char *name);

  /*
   * Receive notification of the beginning of a document.
   */
  virtual void startDocument (void);

  /*
   * Receive notification of the beginning of an element.
   */
  virtual void startElement (const ACEXML_Char *namespaceURI,
                             const ACEXML_Char *localName,
                             const ACEXML_Char *qName,
                             ACEXML_Attributes *atts);

  /*
   * Begin the scope of a prefix-URI Namespace mapping.
   */
  virtual void startPrefixMapping (const ACEXML_Char *prefix,
                                   const ACEXML_Char *uri);

  // *** Methods inherit from ACEXML_DTDHandler.

  /*
   * Receive notification of a notation declaration event.
   */
  virtual void notationDecl (const ACEXML_Char *name,
                             const ACEXML_Char *publicId,
                             const ACEXML_Char *systemId);

  /*
   * Receive notification of an unparsed entity declaration event.
   */
  virtual void unparsedEntityDecl (const ACEXML_Char *name,
                                   const ACEXML_Char *publicId,
                                   const ACEXML_Char *systemId,
                                   const ACEXML_Char *notationName);

  // Methods inherit from ACEXML_EnitityResolver.

  /*
   * Allow the application to resolve external entities.
   */
  virtual ACEXML_InputSource *resolveEntity (const ACEXML_Char *publicId,
                                             const ACEXML_Char *systemId);

  // Methods inherit from ACEXML_ErrorHandler.

  /*
   * Receive notification of a recoverable error.
   */
  virtual void error (ACEXML_SAXParseException &exception);

  /*
   * Receive notification of a non-recoverable error.
   */
  virtual void fatalError (ACEXML_SAXParseException &exception);

  /*
   * Receive notification of a warning.
   */
  virtual void warning (ACEXML_SAXParseException &exception);
protected:
  int setupParser (void);
  // Set up the event handlers of parent parser to this.
  // Returns -1 if no valid parent is set.

private:
  ACEXML_XMLReader *parent_;
  ACEXML_Locator *locator_;
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
