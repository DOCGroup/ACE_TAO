// -*- C++ -*- $Id$

#ifndef _ACEXML_CONTENTHANDLER_H_
#define _ACEXML_CONTENTHANDLER_H_

#include "common/Env.h"
#include "common/SAXExceptions.h"
#include "common/Locator.h"
#include "common/Attributes.h"

class ACEXML_Export ACEXML_ContentHandler
{
public:
  /*
   * Receive notification of character data.
   */
  virtual void characters (const ACEXML_Char *ch,
                           int start,
                           int length,
                           ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
    = 0;

  /*
   * Receive notification of the end of a document.
   */
  virtual void endDocument (ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
    = 0;

  /*
   * Receive notification of the end of an element.
   */
  virtual void endElement (const ACEXML_Char *namespaceURI,
                           const ACEXML_Char *localName,
                           const ACEXML_Char *qName,
                           ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
    = 0;

  /*
   * End the scope of a prefix-URI mapping.
   */
  virtual void endPrefixMapping (const ACEXML_Char *prefix,
                                 ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
    = 0;

  /*
   * Receive notification of ignorable whitespace in element content.
   */
  virtual void ignorableWhitespace (const ACEXML_Char *ch,
                                    int start,
                                    int length,
                                    ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
    = 0;

  /*
   * Receive notification of a processing instruction.
   */
  virtual void processingInstruction (const ACEXML_Char *target,
                                      const ACEXML_Char *data,
                                      ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
    = 0;

  /*
   * Receive an object for locating the origin of SAX document events.
   */
  virtual void setDocumentLocator (ACEXML_Locator *locator,
                                   ACEXML_Env &xmlenv) = 0;

  /*
   * Receive notification of a skipped entity.
   */
  virtual void skippedEntity (const ACEXML_Char *name,
                              ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
    = 0;

  /*
   * Receive notification of the beginning of a document.
   */
  virtual void startDocument (ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
    = 0;

  /*
   * Receive notification of the beginning of an element.
   */
  virtual void startElement (const ACEXML_Char *namespaceURI,
                             const ACEXML_Char *localName,
                             const ACEXML_Char *qName,
                             ACEXML_Attributes *atts,
                             ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
    = 0;

  /*
   * Begin the scope of a prefix-URI Namespace mapping.
   */
  virtual void startPrefixMapping (const ACEXML_Char *prefix,
                                   const ACEXML_Char *uri,
                                   ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
    = 0;
};

#endif /* _ACEXML_CONTENTHANDLER_H_ */
