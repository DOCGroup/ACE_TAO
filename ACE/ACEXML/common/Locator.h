// -*- C++ -*-

//=============================================================================
/**
 *  @file    Locator.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef _ACEXML_LOCATOR_H_
#define _ACEXML_LOCATOR_H_
#include /**/ "ace/pre.h"

#include "ACEXML/common/ACEXML_Export.h"
#include "ACEXML/common/XML_Types.h"

/**
 * @class ACEXML_Locator
 *
 * @brief ACEXML_Locator defines operations that an XML locator should support.
 *
 * If a SAX parser provides location information to the SAX
 * application, it does so by implementing this interface and then
 * passing an instance to the application using the content handler's
 * setDocumentLocator method. The application can use the object to
 * obtain the location of any other content handler event in the XML
 * source document.
 *
 * Note that the results returned by the object will be valid only
 * during the scope of each content handler method: the application
 * will receive unpredictable results if it attempts to use the
 * locator at any other time.
 *
 * SAX parsers are not required to supply a locator, but they are very
 * strongly encouraged to do so. If the parser supplies a locator, it
 * must do so before reporting any other document events. If no
 * locator has been set by the time the application receives the
 * startDocument event, the application should assume that a locator
 * is not available.
 */
class ACEXML_Export ACEXML_Locator
{
public:

  virtual ~ACEXML_Locator (void);

  /*
   * Return the column number where the current document event ends.
   */
  virtual int getColumnNumber (void) const = 0;

  /*
   * Return the line number where the current document event ends.
   */
  virtual int getLineNumber (void) const = 0;

  /*
   * Return the public identifier for the current document event.
   */
  virtual const ACEXML_Char *getPublicId (void) const = 0;

  /*
   * Return the system identifier for the current document event.
   */
  virtual const ACEXML_Char *getSystemId (void) const = 0;
};


#include /**/ "ace/post.h"

#endif /* _ACEXML_LOCATOR_H_ */
