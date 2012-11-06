// -*- C++ -*-

//=============================================================================
/**
 *  @file    ContentHandler.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================
#ifndef _ACEXML_CONTENTHANDLER_H_
#define _ACEXML_CONTENTHANDLER_H_

#include /**/ "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/SAXExceptions.h"
#include "ACEXML/common/Locator.h"
#include "ACEXML/common/Attributes.h"

/**
 * @class ACEXML_ContentHandler
 *
 * @brief ACEXML_ContentHandler
 *
 * This is the main interface that most SAX applications implement: if the
 * application needs to be informed of basic parsing events, it implements
 * this interface and registers an instance with the SAX parser using the
 * setContentHandler method. The parser uses the instance to report basic
 * document-related events like the start and end of elements and character
 * data.
 *
 * The order of events in this interface is very important, and mirrors the
 * order of information in the document itself. For example, all of an
 * element's content (character data, processing instructions, and/or
 * subelements) will appear, in order, between the startElement event and
 * the corresponding endElement event.
 */
class ACEXML_Export ACEXML_ContentHandler
{
public:

  virtual ~ACEXML_ContentHandler (void);

  /**
   * Receive notification of character data.
   */
  virtual void characters (const ACEXML_Char *ch,
                           size_t start,
                           size_t length) = 0;

  /**
   * Receive notification of the end of a document.
   */
  virtual void endDocument (void) = 0;

  /**
   * Receive notification of the end of an element.
   */
  virtual void endElement (const ACEXML_Char *namespaceURI,
                           const ACEXML_Char *localName,
                           const ACEXML_Char *qName) = 0;

  /**
   * End the scope of a prefix-URI mapping.
   */
  virtual void endPrefixMapping (const ACEXML_Char *prefix) = 0;

  /**
   * Receive notification of ignorable whitespace in element content.
   */
  virtual void ignorableWhitespace (const ACEXML_Char *ch,
                                    int start,
                                    int length) = 0;

  /**
   * Receive notification of a processing instruction.
   */
  virtual void processingInstruction (const ACEXML_Char *target,
                                      const ACEXML_Char *data) = 0;

  /**
   * Receive an object for locating the origin of SAX document events.
   */
  virtual void setDocumentLocator (ACEXML_Locator *locator) = 0;

  /**
   * Receive notification of a skipped entity.
   */
  virtual void skippedEntity (const ACEXML_Char *name) = 0;

  /**
   * Receive notification of the beginning of a document.
   */
  virtual void startDocument (void) = 0;

  /**
   * Receive notification of the beginning of an element.
   */
  virtual void startElement (const ACEXML_Char *namespaceURI,
                             const ACEXML_Char *localName,
                             const ACEXML_Char *qName,
                             ACEXML_Attributes *atts) = 0;

  /**
   * Begin the scope of a prefix-URI Namespace mapping.
   */
  virtual void startPrefixMapping (const ACEXML_Char *prefix,
                                   const ACEXML_Char *uri) = 0;
};

#include /**/ "ace/post.h"


#endif /* _ACEXML_CONTENTHANDLER_H_ */
