// -*- C++ -*-

//=============================================================================
/**
 *  @file    DefaultHandler.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================
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
  /**
   * Default constructor.
   */
  ACEXML_DefaultHandler (void);

  /**
   * destructor.
   */
  virtual ~ACEXML_DefaultHandler (void);

  // Methods inherit from ACEXML_ContentHandler.

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
};

#include /**/ "ace/post.h"

#endif /* ACEXML_DEFAULTHANDLER_H */
