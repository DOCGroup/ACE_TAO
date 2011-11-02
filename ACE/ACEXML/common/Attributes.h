// -*- C++ -*-

//=============================================================================
/**
 *  @file    Attributes.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================

#ifndef _ACEXML_ATTRIBUTES_H_
#define _ACEXML_ATTRIBUTES_H_
#include /**/ "ace/pre.h"

#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/XML_Types.h"

/**
 * @class ACEXML_Attributes
 *
 * @brief ACEXML_Attributes defines a collection of attributes of an XML element.
 *
 * This is an abstract class (interface in Java) that defines
 * the API for accessing attributes of an XML entity.
 *
 * This interface allows access to a list of attributes in three different ways:
 *
 * - by attribute index;
 * - by Namespace-qualified name; or
 * - by qualified (prefixed) name.
 *
 * The list will not contain attributes that were declared #IMPLIED
 * but not specified in the start tag. It will also not contain
 * attributes used as Namespace declarations (xmlns*) unless the
 * http://xml.org/sax/features/namespace-prefixes feature is set to
 * true (it is false by default).
 *
 * If the namespace-prefixes feature (see above) is false, access by
 * qualified name may not be available; if the
 * http://xml.org/sax/features/namespaces feature is false, access by
 * Namespace-qualified names may not be available.
 *
 * This interface replaces the now-deprecated SAX1 AttributeList
 * interface, which does not contain Namespace support. In addition to
 * Namespace support, it adds the getIndex methods (below).
 *
 * The order of attributes in the list is unspecified, and will vary
 * from implementation to implementation.
 */
class ACEXML_Export ACEXML_Attributes
{
public:

  /// Destructor.
  virtual ~ACEXML_Attributes (void);

  /**
   * Look up the index of an attribute by XML 1.0 qualified name.
   */
  virtual int getIndex (const ACEXML_Char *qName) = 0;

  /**
   * Look up the index of an attribute by Namespace name.
   */
  virtual int getIndex (const ACEXML_Char *uri,
                        const ACEXML_Char *localPart) = 0;

  /**
   * Return the number of attributes in the list.
   */
  virtual size_t getLength (void) = 0;

  /**
   * Look up an attribute's local name by index.
   */
  virtual const ACEXML_Char *getLocalName (size_t index) = 0;

  /**
   * Look up an attribute's XML 1.0 qualified name by index.
   */
  virtual const ACEXML_Char *getQName (size_t index) = 0;

  /**
   * Look up an attribute's type by index.
   */
  virtual const ACEXML_Char *getType (size_t index) = 0;

  /**
   * Look up an attribute's type by XML 1.0 qualified name.
   */
  virtual const ACEXML_Char *getType (const ACEXML_Char *qName) = 0;

  /**
   * Look up an attribute's type by Namespace name.
   */
  virtual const ACEXML_Char *getType (const ACEXML_Char *uri,
                                      const ACEXML_Char *localPart) = 0;

  /**
   * Look up an attribute's Namespace URI by index.
   */
  virtual const ACEXML_Char *getURI (size_t index) = 0;

  /**
   * Look up an attribute's value by index.
   */
  virtual const ACEXML_Char *getValue (size_t index) = 0;

  /**
   * Look up an attribute's value by XML 1.0 qualified name.
   */
  virtual const ACEXML_Char *getValue (const ACEXML_Char *qName) = 0;

  /**
   * Look up an attribute's value by Namespace name.
   */
  virtual const ACEXML_Char *getValue (const ACEXML_Char *uri,
                                       const ACEXML_Char *localPart) = 0;
};

#include /**/ "ace/post.h"

#endif /* _ACEXML_ ATTRIBUTES_H_ */
