// -*- C++ -*-  $Id$

#ifndef ACEXML_ATTRIBUTESIMPL_H
#define ACEXML_ATTRIBUTESIMPL_H

#include "Common/Attributes.h"
#include "ace/Containers_T.h"

#if !defined ACEXML_AttributesImpl_Default_Size
#define ACEXML_AttributesImpl_Default_Size 20
#endif /* ACEXML_AttributesImpl_Default_Size */

class ACEXML_AttributesImpl;

class ACEXML_Export ACEXML_Attribute
{
public:
  friend class ACEXML_AttributesImpl;

  ACEXML_Attribute (void);
  ACEXML_Attribute (const ACEXML_Attribute &attr);
  ACEXML_Attribute (const ACEXML_Char *uri,
                    const ACEXML_Char *localName,
                    const ACEXML_Char *qName,
                    const ACEXML_Char *type,
                    const ACEXML_Char *value);

  ~ACEXML_Attribute (void);

  void setAttribute (const ACEXML_Char *uri,
                     const ACEXML_Char *localName,
                     const ACEXML_Char *qName,
                     const ACEXML_Char *type,
                     const ACEXML_Char *value);

  const ACEXML_Char *uri (void) const;
  void uri (const ACEXML_Char *uri);

  const ACEXML_Char *localName (void) const;
  void localName (const ACEXML_Char *localName);

  const ACEXML_Char *qName (void) const;
  void qName (const ACEXML_Char *qName);

  const ACEXML_Char *type (void) const;
  void type (const ACEXML_Char *type);

  const ACEXML_Char *value (void) const;
  void value (const ACEXML_Char *value);

  ACEXML_Attribute &operator= (const ACEXML_Attribute &rhs);
  int operator!= (const ACEXML_Attribute&rhs) const;

private:
  ACEXML_Char *uri_;
  ACEXML_Char *localName_;
  ACEXML_Char *qName_;
  ACEXML_Char *type_;
  ACEXML_Char *value_;
};

typedef ACE_Array<ACEXML_Attribute> ACEXML_Attribute_Array;

class ACEXML_Export ACEXML_AttributesImpl
  : public ACEXML_Attributes
{
public:
  /*
   * Initialize an AttributesImpl that holds <size> attributes.
   */
  ACEXML_AttributesImpl (int size = ACEXML_AttributesImpl_Default_Size);
  ACEXML_AttributesImpl (const ACEXML_AttributesImpl &attrs);
  virtual ~ACEXML_AttributesImpl (void);

  /*
   * Add a new attribute using the argument(s) supplied.
   * Return -1 if an attribute with the same name already exists.
   */
  virtual int addAttribute (const ACEXML_Char *uri,
                            const ACEXML_Char *localName,
                            const ACEXML_Char *qName,
                            const ACEXML_Char *type,
                            const ACEXML_Char *value);
  virtual int addAttribute (const ACEXML_Attribute &att);

  /*
   * Remove an attribute from the array.  Notice that this
   * operation can invalidate previously acquired <index>
   * value.  (It will repack the array.)
   */
  virtual int removeAttribute (size_t index);


  /*
   * Look up the index of an attribute by XML 1.0 qualified name.
   * Return -1 if we fail to find a match.
   */
  virtual int getIndex (const ACEXML_Char *qName);

  /*
   * Look up the index of an attribute by Namespace name.
   * Return -1 if we fail to find a match.
   */
  virtual int getIndex (const ACEXML_Char *uri,
                        const ACEXML_Char *localPart);

  /*
   * Return the number of attributes in the list.
   */
  virtual size_t getLength (void);

  /*
   * Look up an attribute's local name by index.
   * Return 0 if index is out of range.
   */
  virtual const ACEXML_Char *getLocalName (size_t index);

  /*
   * Look up an attribute's XML 1.0 qualified name by index.
   * Return 0 if index is out of range.
   */
  virtual const ACEXML_Char *getQName (size_t index);

  /*
   * Look up an attribute's type by index.
   * Return 0 if index is out of range.
   */
  virtual const ACEXML_Char *getType (size_t index);

  /*
   * Look up an attribute's type by XML 1.0 qualified name.
   * Return 0 if we fail to find a match.
   */
  virtual const ACEXML_Char *getType (const ACEXML_Char *qName);

  /*
   * Look up an attribute's type by Namespace name.
   * Return 0 if we fail to find a match.
   */
  virtual const ACEXML_Char *getType (const ACEXML_Char *uri,
                                      const ACEXML_Char *localPart);

  /*
   * Look up an attribute's Namespace URI by index.
   * Return 0 if index is out of range.
   */
  virtual const ACEXML_Char *getURI (size_t index);

  /*
   * Look up an attribute's value by index.
   * Return 0 if index is out of range.
   */
  virtual const ACEXML_Char *getValue (size_t index);

  /*
   * Look up an attribute's value by XML 1.0 qualified name.
   * Return 0 if we fail to find a match.
   */
  virtual const ACEXML_Char *getValue (const ACEXML_Char *qName);

  /*
   * Look up an attribute's value by Namespace name.
   * Return 0 if we fail to find a match.
   */
  virtual const ACEXML_Char *getValue (const ACEXML_Char *uri,
                                       const ACEXML_Char *localPart);

  /*
   * Set an attribute at index.  Return -1 if index is out of
   * range.
   */
  virtual int setAttribute (size_t index,
                            const ACEXML_Char *uri,
                            const ACEXML_Char *localName,
                            const ACEXML_Char *qName,
                            const ACEXML_Char *type,
                            const ACEXML_Char *value);

  /*
   * Set the localName of the attribute at <index>.
   * return -1 if <index> is out of range.
   */
  virtual int setLocalName (size_t index,
                            const ACEXML_Char *localName);

  /*
   * Set the qName of the attribute at <index>.
   * return -1 if <index> is out of range.
   */
  virtual int setQName (size_t index,
                        const ACEXML_Char *qName);

  /*
   * Set the URI of the attribute at <index>.
   * return -1 if <index> is out of range.
   */
  virtual int setURI (size_t index,
                      const ACEXML_Char *uri);

  /*
   * Set the type of the attribute at <index>.
   * return -1 if <index> is out of range.
   */
  virtual int setType (size_t index,
                       const ACEXML_Char *type);

  /*
   * Set the value of the attribute at <index>.
   * return -1 if <index> is out of range.
   */
  virtual int setValue (size_t index,
                        const ACEXML_Char *value);
private:
  ACEXML_Attribute_Array attrs_;
};

#if defined (__ACEXML_INLINE__)
# include "Common/AttributesImpl.i"
#endif /* __ACEXML_INLINE__ */
#endif /* ACEXML_ATTRIBUTESIMPL_H */
