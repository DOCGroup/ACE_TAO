// -*- C++ -*-

//=============================================================================
/**
 *  @file   Debug_Attributes_Builder.h
 *
 *  $Id$
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================
#ifndef _ACEXML_DEBUG_ATTRIBUTES_BUILDER_H_
#define _ACEXML_DEBUG_ATTRIBUTES_BUILDER_H_

#include "common/Attributes_Def_Builder.h"
#include "parser/debug_validator/Debug_DTD_Manager_Export.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Unbounded_Queue.h"

typedef ACE_Unbounded_Queue<ACEXML_String> ACEXML_STRING_QUEUE;
typedef ACE_Unbounded_Queue_Iterator<ACEXML_String> ACEXML_STRING_QUEUE_ITERATOR;

/**
 * @ class ACEXML_Debug_Attribute_Builder Debug_Attributes_Builder.h "parser/debug_validator/Debug_Attributes_Builder.h"
 *
 * This class prints out the Attribute definition for debugging purpose.
 */
class ACEXML_DEBUG_DTD_MANAGER_Export ACEXML_Debug_Attribute_Builder
  : public ACEXML_Attribute_Def_Builder
{
public:
  ACEXML_Debug_Attribute_Builder ();

  ACEXML_Debug_Attribute_Builder (const ACEXML_Debug_Attribute_Builder &rhs);

  virtual ~ACEXML_Debug_Attribute_Builder ();

  /**
   * Specify the name of the attribute.
   */
  virtual int setName (const ACEXML_Char *n);
  virtual const ACEXML_Char *getName (void);

  /**
   * Set the attribute type.
   */
  virtual int setAttType (const ATT_TYPE type,
                          ACEXML_Env &xmlenv)
    // ACE_THORW_SPEC ((ACEXML_SAXException))
    ;

  /**
   * Insert an element for NOTATION or ENUMERATION type attribute.
   */
  virtual int insertList (const ACEXML_Char *Name,
                          ACEXML_Env &xmlenv)
    // ACE_THORW_SPEC ((ACEXML_SAXException))
    ;

  /**
   * Set default attribute declaration.
   */
  virtual int setDefault (const DEFAULT_DECL def,
                          const ACEXML_Char *value,
                          ACEXML_Env &xmlenv)
    // ACE_THORW_SPEC ((ACEXML_SAXException))
    ;

  /**
   * Check validity of the current attribute definition being built.
   *
   * @retval 0 if the attribute is not a valid combo.
   */
  virtual int validAttr (void);


  /**
   * Dump the content of the attribute definition.
   */
  virtual void dump (void);
private:
  /// Attribute name.
  ACEXML_String name_;

  /// Type of attribute.
  ATT_TYPE type_;

  /// Default value type.
  DEFAULT_DECL default_decl_;

  /// Default attribute value.
  ACEXML_String default_value_;

  /// Holds a queue of enumerated attribute values.
  ACEXML_STRING_QUEUE att_value_queue_;
};

typedef ACE_Hash_Map_Entry<ACEXML_String,
                           ACEXML_Debug_Attribute_Builder> ACEXML_ATT_MAP_ENTRY;

typedef ACE_Hash_Map_Manager_Ex <ACEXML_String,
                                 ACEXML_Debug_Attribute_Builder,
                                 ACE_Hash<ACEXML_String>,
                                 ACE_Equal_To<ACEXML_String>,
                                 ACE_Null_Mutex> ACEXML_ATT_MAP;

typedef ACE_Hash_Map_Iterator_Ex<ACEXML_String,
                                 ACEXML_Debug_Attribute_Builder,
                                 ACE_Hash<ACEXML_String>,
                                 ACE_Equal_To<ACEXML_String>,
                                 ACE_Null_Mutex> ACEXML_ATT_MAP_ITER;

typedef ACE_Hash_Map_Reverse_Iterator_Ex<ACEXML_String,
                                         ACEXML_Debug_Attribute_Builder,
                                         ACE_Hash<ACEXML_String>,
                                         ACE_Equal_To<ACEXML_String>,
                                         ACE_Null_Mutex> ACEXML_ATT_MAP_REVERSE_ITER;

/**
 * @ class ACEXML_Debug_Attributes_Builder Debug_Attributes_Builder.h "parser/debug_validator/Debug_Attributes_Builder.h"
 *
 * This class prints out Attribute definitions for debugging purpose.
 */
class ACEXML_DEBUG_DTD_MANAGER_Export ACEXML_Debug_Attributes_Builder
  : public ACEXML_Attributes_Def_Builder
{
public:
  ACEXML_Debug_Attributes_Builder ();

  virtual ~ACEXML_Debug_Attributes_Builder ();

  /**
   * Set the element name that the attribute builder applies.
   *
   * @retval 0 if valid, -1 otherwise.
   */
  virtual int setElement (const ACEXML_Char *namespaceURI,
                          const ACEXML_Char *localName,
                          const ACEXML_Char *qName,
                          ACEXML_Env &xmlenv)
    //    ACE_THROW_SPEC ((ACEXML_SAXException))
    ;

  /**
   * Acquire an Attribute_Builder.
   */
  virtual ACEXML_Attribute_Def_Builder *getAttribute_Def_Builder ();

  /**
   * Add a definition for one attribute.
   */
  virtual int insertAttribute (ACEXML_Attribute_Def_Builder *def,
                               ACEXML_Env &xmlenv);


  /**
   * Dump the content of the attribute definition.
   */
  virtual void dump (void);
protected:
  /// The name of the element type these attributes applied.
  ACEXML_String element_name_;

  /// Collection of attributes.
  ACEXML_ATT_MAP attributes_;
};



#endif /* _ACEXML_DEBUG_ATTRIBUTES_BUILDER_H_ */
