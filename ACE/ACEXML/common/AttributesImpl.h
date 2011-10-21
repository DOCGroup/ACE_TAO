// -*- C++ -*-

//=============================================================================
/**
 *  @file    AttributesImpl.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACEXML_ATTRIBUTESIMPL_H
#define ACEXML_ATTRIBUTESIMPL_H

#include /**/ "ace/pre.h"
#include "ACEXML/common/ACEXML_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/Attributes.h"
#include "ace/Containers_T.h"

#if !defined ACEXML_AttributesImpl_Default_Size
#define ACEXML_AttributesImpl_Default_Size 20
#endif /* ACEXML_AttributesImpl_Default_Size */

class ACEXML_AttributesImpl;

/**
 * @class ACEXML_Attribute
 *
 * @brief ACEXML_Attribute defines the data structure of an attribute
 *
 * @sa ACEXML_AttributesImpl
 */
class ACEXML_Export ACEXML_Attribute
{
public:
  friend class ACEXML_AttributesImpl;

  /// Default constructor.
  ACEXML_Attribute (void);

  /// Copy constructor.
  ACEXML_Attribute (const ACEXML_Attribute &attr);

  /// Initialize all constructor.
  ACEXML_Attribute (const ACEXML_Char *uri,
                    const ACEXML_Char *localName,
                    const ACEXML_Char *qName,
                    const ACEXML_Char *type,
                    const ACEXML_Char *value);

  /// Destructor.
  ~ACEXML_Attribute (void);

  /// Set all members.
  void setAttribute (const ACEXML_Char *uri,
                     const ACEXML_Char *localName,
                     const ACEXML_Char *qName,
                     const ACEXML_Char *type,
                     const ACEXML_Char *value);

  /// Get \a uri_.
  const ACEXML_Char *uri (void) const;

  /// Set \a uri_.
  void uri (const ACEXML_Char *uri);

  /// Get \a localName_.
  const ACEXML_Char *localName (void) const;

  /// Set \a localName_.
  void localName (const ACEXML_Char *localName);

  /// Get \a qName_.
  const ACEXML_Char *qName (void) const;

  /// Set \a qName_.
  void qName (const ACEXML_Char *qName);

  /// Get \a type_.
  const ACEXML_Char *type (void) const;

  /// Set \a type_.
  void type (const ACEXML_Char *type);

  /// Get \a value_.
  const ACEXML_Char *value (void) const;

  /// Set \a value_.
  void value (const ACEXML_Char *value);

  /// Assignment operator.
  ACEXML_Attribute &operator= (const ACEXML_Attribute &rhs);

  /// Comparison operator.
  bool operator!= (const ACEXML_Attribute&rhs) const;

private:
  /// Namespace URI of an attribute
  ACEXML_Char *uri_;

  ACEXML_Char *localName_;
  ACEXML_Char *qName_;
  ACEXML_Char *type_;
  ACEXML_Char *value_;
};

/**
 * @typedef ACE_Array<ACEXML_Attribute> ACEXML_Attribute_Array
 */
typedef ACE_Array<ACEXML_Attribute> ACEXML_Attribute_Array;

/**
 * @class ACEXML_AttributesImpl AttributesImpl.h "ACEXML/common/AttributesImpl.h"
 *
 * @brief ACEXML_AttributesImpl provides the default implementation
 * of interface ACEXML_Attributes.
 *
 * This class provides a default implementation of the SAX2 Attributes
 * interface, with the addition of manipulators so that the list can
 * be modified or reused.
 *
 * There are two typical uses of this class:
 *
 * - to take a persistent snapshot of an Attributes object in a
 *   startElement event; or
 * - to construct or modify an Attributes object in a SAX2 driver or filter.
 *
 * This class replaces the now-deprecated SAX1 AttributeListImpl
 * class; in addition to supporting the updated Attributes interface
 * rather than the deprecated AttributeList interface, it also
 * includes a much more efficient implementation using a single array
 * rather than a set of Vectors.
 *
 * @sa ACEXML_Attributes
 */
class ACEXML_Export ACEXML_AttributesImpl
  : public ACEXML_Attributes
{
public:
  /**
   * Initialize an AttributesImpl that holds @a size attributes.
   */
  ACEXML_AttributesImpl (int size = ACEXML_AttributesImpl_Default_Size);
  ACEXML_AttributesImpl (const ACEXML_AttributesImpl &attrs);
  virtual ~ACEXML_AttributesImpl (void);

  /**
   * Add a new attribute using the argument(s) supplied.
   * Return -1 if an attribute with the same name already exists.
   */
  virtual int addAttribute (const ACEXML_Char *uri,
                            const ACEXML_Char *localName,
                            const ACEXML_Char *qName,
                            const ACEXML_Char *type,
                            const ACEXML_Char *value);
  virtual int addAttribute (const ACEXML_Attribute &att);

  /**
   *  Check for duplicate attributes.
   */
  virtual int isDuplicate (const ACEXML_Char *uri,
                              const ACEXML_Char *localName,
                              const ACEXML_Char *qName);
  /**
   * Remove an attribute from the array.  Notice that this
   * operation can invalidate previously acquired @a index
   * value.  (It will repack the array.)
   */
  virtual int removeAttribute (size_t index);


  /**
   * Look up the index of an attribute by XML 1.0 qualified name.
   * Return -1 if we fail to find a match.
   */
  virtual int getIndex (const ACEXML_Char *qName);

  /**
   * Look up the index of an attribute by Namespace name.
   * Return -1 if we fail to find a match.
   */
  virtual int getIndex (const ACEXML_Char *uri,
                        const ACEXML_Char *localPart);

  /**
   * Return the number of attributes in the list.
   */
  virtual size_t getLength (void);

  /**
   * Look up an attribute's local name by index.
   * Return 0 if index is out of range.
   */
  virtual const ACEXML_Char *getLocalName (size_t index);

  /**
   * Look up an attribute's XML 1.0 qualified name by index.
   * Return 0 if index is out of range.
   */
  virtual const ACEXML_Char *getQName (size_t index);

  /**
   * Look up an attribute's type by index.
   * Return 0 if index is out of range.
   */
  virtual const ACEXML_Char *getType (size_t index);

  /**
   * Look up an attribute's type by XML 1.0 qualified name.
   * Return 0 if we fail to find a match.
   */
  virtual const ACEXML_Char *getType (const ACEXML_Char *qName);

  /**
   * Look up an attribute's type by Namespace name.
   * Return 0 if we fail to find a match.
   */
  virtual const ACEXML_Char *getType (const ACEXML_Char *uri,
                                      const ACEXML_Char *localPart);

  /**
   * Look up an attribute's Namespace URI by index.
   * Return 0 if index is out of range.
   */
  virtual const ACEXML_Char *getURI (size_t index);

  /**
   * Look up an attribute's value by index.
   * Return 0 if index is out of range.
   */
  virtual const ACEXML_Char *getValue (size_t index);

  /**
   * Look up an attribute's value by XML 1.0 qualified name.
   * Return 0 if we fail to find a match.
   */
  virtual const ACEXML_Char *getValue (const ACEXML_Char *qName);

  /**
   * Look up an attribute's value by Namespace name.
   * Return 0 if we fail to find a match.
   */
  virtual const ACEXML_Char *getValue (const ACEXML_Char *uri,
                                       const ACEXML_Char *localPart);

  /**
   * Set an attribute at index.  Return -1 if index is out of
   * range.
   */
  virtual int setAttribute (size_t index,
                            const ACEXML_Char *uri,
                            const ACEXML_Char *localName,
                            const ACEXML_Char *qName,
                            const ACEXML_Char *type,
                            const ACEXML_Char *value);

  /**
   * Set the localName of the attribute at @a index.
   * return -1 if @a index is out of range.
   */
  virtual int setLocalName (size_t index,
                            const ACEXML_Char *localName);

  /**
   * Set the qName of the attribute at @a index.
   * return -1 if @a index is out of range.
   */
  virtual int setQName (size_t index,
                        const ACEXML_Char *qName);

  /**
   * Set the URI of the attribute at @a index.
   * return -1 if @a index is out of range.
   */
  virtual int setURI (size_t index,
                      const ACEXML_Char *uri);

  /**
   * Set the type of the attribute at @a index.
   * return -1 if @a index is out of range.
   */
  virtual int setType (size_t index,
                       const ACEXML_Char *type);

  /**
   * Set the value of the attribute at @a index.
   * return -1 if @a index is out of range.
   */
  virtual int setValue (size_t index,
                        const ACEXML_Char *value);
private:
  /// Container for all attributes.
  ACEXML_Attribute_Array attrs_;
};

#if defined (__ACEXML_INLINE__)
# include "ACEXML/common/AttributesImpl.inl"
#endif /* __ACEXML_INLINE__ */

#include /**/ "ace/post.h"

#endif /* ACEXML_ATTRIBUTESIMPL_H */
