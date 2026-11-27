/**
 *  @file    Attributes.h
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */

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
 * The list will not contain attributes declared with the @c IMPLIED keyword
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
  virtual ~ACEXML_Attributes ();

  /**
   * Look up the index of an attribute by XML 1.0 qualified name.
   */
  virtual int getIndex (ACEXML_Char const *qName) = 0;

  /**
   * Look up the index of an attribute by Namespace name.
   */
  virtual int getIndex (ACEXML_Char const *uri,
                        ACEXML_Char const *localPart) = 0;

  /**
   * Return the number of attributes in the list.
   */
  virtual size_t getLength () = 0;

  /**
   * Look up an attribute's local name by index.
   */
  virtual ACEXML_Char const *getLocalName (size_t index) = 0;

  /**
   * Look up an attribute's XML 1.0 qualified name by index.
   */
  virtual ACEXML_Char const *getQName (size_t index) = 0;

  /**
   * Look up an attribute's type by index.
   */
  virtual ACEXML_Char const *getType (size_t index) = 0;

  /**
   * Look up an attribute's type by XML 1.0 qualified name.
   */
  virtual ACEXML_Char const *getType (ACEXML_Char const *qName) = 0;

  /**
   * Look up an attribute's type by Namespace name.
   */
  virtual ACEXML_Char const *getType (ACEXML_Char const *uri,
                                      ACEXML_Char const *localPart) = 0;

  /**
   * Look up an attribute's Namespace URI by index.
   */
  virtual ACEXML_Char const *getURI (size_t index) = 0;

  /**
   * Look up an attribute's value by index.
   */
  virtual ACEXML_Char const *getValue (size_t index) = 0;

  /**
   * Look up an attribute's value by XML 1.0 qualified name.
   */
  virtual ACEXML_Char const *getValue (ACEXML_Char const *qName) = 0;

  /**
   * Look up an attribute's value by Namespace name.
   */
  virtual ACEXML_Char const *getValue (ACEXML_Char const *uri,
                                       ACEXML_Char const *localPart) = 0;
};

#include /**/ "ace/post.h"

#endif /* _ACEXML_ ATTRIBUTES_H_ */
