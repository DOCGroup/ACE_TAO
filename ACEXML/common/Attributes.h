// -*- C++ -*- $Id$

#ifndef _ACEXML_ATTRIBUTES_H_
#define _ACEXML_ATTRIBUTES_H_

#include "common/XML_Types.h"

class ACEXML_Export ACEXML_Attributes
{
  // This is an abstract class (interface in Java) that defines
  // the API for accessing attributes of an XML entity.
public:
  /*
   * Look up the index of an attribute by XML 1.0 qualified name.
   */
  virtual int getIndex (const ACEXML_Char *qName) = 0;

  /*
   * Look up the index of an attribute by Namespace name.
   */
  virtual int getIndex (const ACEXML_Char *uri,
                        const ACEXML_Char *localPart) = 0;

  /*
   * Return the number of attributes in the list.
   */
  virtual size_t getLength (void) = 0;

  /*
   * Look up an attribute's local name by index.
   */
  virtual const ACEXML_Char *getLocalName (size_t index) = 0;

  /*
   * Look up an attribute's XML 1.0 qualified name by index.
   */
  virtual const ACEXML_Char *getQName (size_t index) = 0;

  /*
   * Look up an attribute's type by index.
   */
  virtual const ACEXML_Char *getType (size_t index) = 0;

  /*
   * Look up an attribute's type by XML 1.0 qualified name.
   */
  virtual const ACEXML_Char *getType (const ACEXML_Char *qName) = 0;

  /*
   * Look up an attribute's type by Namespace name.
   */
  virtual const ACEXML_Char *getType (const ACEXML_Char *uri,
                                      const ACEXML_Char *localPart) = 0;

  /*
   * Look up an attribute's Namespace URI by index.
   */
  virtual const ACEXML_Char *getURI (size_t index) = 0;

  /*
   * Look up an attribute's value by index.
   */
  virtual const ACEXML_Char *getValue (size_t index) = 0;

  /*
   * Look up an attribute's value by XML 1.0 qualified name.
   */
  virtual const ACEXML_Char *getValue (const ACEXML_Char *qName) = 0;

  /*
   * Look up an attribute's value by Namespace name.
   */
  virtual const ACEXML_Char *getValue (const ACEXML_Char *uri,
                                       const ACEXML_Char *localPart) = 0;
};
#endif /* _ACEXML_ ATTRIBUTES_H_ */
