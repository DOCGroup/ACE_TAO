// $Id$

//=============================================================================
/**
 *  @file    SoftPkgHandler.h
 *
 *  $Id$
 *
 *  @author Krishnakumar B <kitty@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACEXML_SOFTPKG_HANDLER_H
#define ACEXML_SOFTPKG_HANDLER_H

#include "ace/pre.h"
#include "ACEXML/compass/Compass_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/DefaultHandler.h"
#include "ACEXML/compass/SoftPkg.h"

/**
 * @class ACEXML_SoftPkgHandler
 *
 * @brief ACEXML_SoftPkgHandler is an example SAX event handler.
 *
 * This SAX event handler try to regenerate the XML document it
 * reads with correct indentation.
 */
class Compass_Export ACEXML_SoftPkgHandler : public ACEXML_DefaultHandler
{
public:
  /*
   * Default constructor.
   */
  ACEXML_SoftPkgHandler (const ACEXML_Char* name);

  /*
   * Default destructor.
   */
  virtual ~ACEXML_SoftPkgHandler (void);

  // Methods inherited from ACEXML_ContentHandler.

  /*
   * Receive notification of character data.
   */
  virtual void characters (const ACEXML_Char *ch, int start,
                           int length ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  /*
   * Receive notification of the end of a document.
   */
  virtual void endDocument (ACEXML_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  /*
   * Receive notification of the end of an element.
   */
  virtual void endElement (const ACEXML_Char *namespaceURI,
                           const ACEXML_Char *localName,
                           const ACEXML_Char *qName ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  /*
   * End the scope of a prefix-URI mapping.
   */
  virtual void endPrefixMapping (const ACEXML_Char *prefix ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  /*
   * Receive notification of ignorable whitespace in element content.
   */
  virtual void ignorableWhitespace (const ACEXML_Char *ch, int start,
                                    int length ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  /*
   * Receive notification of a processing instruction.
   */
  virtual void processingInstruction (const ACEXML_Char *target,
                                      const ACEXML_Char *data
                                      ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  /*
   * Receive an object for locating the origin of SAX document events.
   */
  virtual void setDocumentLocator (ACEXML_Locator *locator) ;

  /*
   * Receive notification of a skipped entity.
   */
  virtual void skippedEntity (const ACEXML_Char *name ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /*
   * Receive notification of the beginning of a document.
   */
  virtual void startDocument (ACEXML_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  /*
   * Receive notification of the beginning of an element.
   */
  virtual void startElement (const ACEXML_Char *namespaceURI,
                             const ACEXML_Char *localName,
                             const ACEXML_Char *qName,
                             ACEXML_Attributes *atts ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  /*
   * Begin the scope of a prefix-URI Namespace mapping.
   */
  virtual void startPrefixMapping (const ACEXML_Char *prefix,
                                   const ACEXML_Char *uri ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  // *** Methods inherited from ACEXML_DTDHandler.

  /*
   * Receive notification of a notation declaration event.
   */
  virtual void notationDecl (const ACEXML_Char *name,
                             const ACEXML_Char *publicId,
                             const ACEXML_Char *systemId ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  /*
   * Receive notification of an unparsed entity declaration event.
   */
  virtual void unparsedEntityDecl (const ACEXML_Char *name,
                                   const ACEXML_Char *publicId,
                                   const ACEXML_Char *systemId,
                                   const ACEXML_Char *notationName
                                   ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  // Methods inherited from ACEXML_EnitityResolver.

  /*
   * Allow the application to resolve external entities.
   */
  virtual ACEXML_InputSource *resolveEntity (const ACEXML_Char *publicId,
                                             const ACEXML_Char *systemId
                                             ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  // Methods inherited from ACEXML_ErrorHandler.

  /*
   * Receive notification of a recoverable error.
   */
  virtual void error (ACEXML_SAXParseException &exception ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  /*
   * Receive notification of a non-recoverable error.
   */
  virtual void fatalError (ACEXML_SAXParseException &exception ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  /*
   * Receive notification of a warning.
   */
  virtual void warning (ACEXML_SAXParseException &exception
                        ACEXML_ENV_ARG_DECL)
    ACE_THROW_SPEC ((ACEXML_SAXException));

  virtual const ACEXML_SoftPkg* get_softpkg(void) const;

private:
  ACEXML_Char* fileName_;
  ACEXML_Locator* locator_;
  ACEXML_SoftPkg* softpkg_;
  ACEXML_String cdata_;
};

#if defined (__ACEXML_INLINE__)
#include "ACEXML/compass/SoftPkgHandler.inl"
#endif /* __ACEXML_INLINE__ */

#include "ace/post.h"

#endif /* ACEXML_SOFTPKG_HANDLER_H */
