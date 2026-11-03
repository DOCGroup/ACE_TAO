// -*- C++ -*-

//=============================================================================
/**
 *  @file   Debug_Attributes_Builder.h
 *
 *  @author Nanbor Wang <nanbor@cs.wustl.edu>
 */
//=============================================================================
#ifndef _ACEXML_DEBUG_ATTRIBUTES_BUILDER_H_
#define _ACEXML_DEBUG_ATTRIBUTES_BUILDER_H_

#include /**/ "ace/pre.h"
#include "ACEXML/parser/debug_validator/Debug_DTD_Manager_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/Attributes_Def_Builder.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Unbounded_Queue.h"

using ACEXML_STRING_QUEUE = ACE_Unbounded_Queue<ACEXML_String>;
using ACEXML_STRING_QUEUE_ITERATOR = ACE_Unbounded_Queue_Iterator<ACEXML_String>;

/**
 * @class ACEXML_Debug_Attribute_Builder Debug_Attributes_Builder.h "parser/debug_validator/Debug_Attributes_Builder.h"
 *
 * This class prints out the Attribute definition for debugging purpose.
 */
class ACEXML_DEBUG_DTD_MANAGER_Export ACEXML_Debug_Attribute_Builder
  : public ACEXML_Attribute_Def_Builder
{
public:
  ACEXML_Debug_Attribute_Builder ();

  ACEXML_Debug_Attribute_Builder (ACEXML_Debug_Attribute_Builder const &rhs);

  ~ACEXML_Debug_Attribute_Builder () override;

  /**
   * Specify the name of the attribute.
   */
  int setName (ACEXML_Char const *n) override;
  ACEXML_Char const *getName () override;

  /**
   * Set the attribute type.
   */
  int setAttType (ATT_TYPE type) override;

  /**
   * Insert an element for NOTATION or ENUMERATION type attribute.
   */
  int insertList (ACEXML_Char const *name) override;

  /**
   * Set default attribute declaration.
   */
  int setDefault (DEFAULT_DECL decl, ACEXML_Char const *value) override;

  /**
   * Check validity of the current attribute definition being built.
   *
   * @retval 0 if the attribute is not a valid combo.
   */
  int validAttr () override;


  /**
   * Dump the content of the attribute definition.
   */
  void dump () override;
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

using ACEXML_ATT_MAP_ENTRY = ACE_Hash_Map_Entry<ACEXML_String,
                                                 ACEXML_Debug_Attribute_Builder>;

using ACEXML_ATT_MAP = ACE_Hash_Map_Manager_Ex<ACEXML_String,
                                               ACEXML_Debug_Attribute_Builder,
                                               ACE_Hash<ACEXML_String>,
                                               ACE_Equal_To<ACEXML_String>,
                                               ACE_Null_Mutex>;

using ACEXML_ATT_MAP_ITER = ACE_Hash_Map_Iterator_Ex<ACEXML_String,
                                                     ACEXML_Debug_Attribute_Builder,
                                                     ACE_Hash<ACEXML_String>,
                                                     ACE_Equal_To<ACEXML_String>,
                                                     ACE_Null_Mutex>;

using ACEXML_ATT_MAP_REVERSE_ITER = ACE_Hash_Map_Reverse_Iterator_Ex<ACEXML_String,
                                                                     ACEXML_Debug_Attribute_Builder,
                                                                     ACE_Hash<ACEXML_String>,
                                                                     ACE_Equal_To<ACEXML_String>,
                                                                     ACE_Null_Mutex>;

/**
 * @class ACEXML_Debug_Attributes_Builder Debug_Attributes_Builder.h "parser/debug_validator/Debug_Attributes_Builder.h"
 *
 * This class prints out Attribute definitions for debugging purpose.
 */
class ACEXML_DEBUG_DTD_MANAGER_Export ACEXML_Debug_Attributes_Builder
  : public ACEXML_Attributes_Def_Builder
{
public:
  ACEXML_Debug_Attributes_Builder ();

  ~ACEXML_Debug_Attributes_Builder () override;
  ACEXML_Debug_Attributes_Builder (ACEXML_Debug_Attributes_Builder const &) = delete;
  ACEXML_Debug_Attributes_Builder (ACEXML_Debug_Attributes_Builder &&) = delete;
  ACEXML_Debug_Attributes_Builder &operator= (ACEXML_Debug_Attributes_Builder const &) = delete;
  ACEXML_Debug_Attributes_Builder &operator= (ACEXML_Debug_Attributes_Builder &&) = delete;

  /**
   * Set the element name that the attribute builder applies.
   *
   * @retval 0 if valid, -1 otherwise.
   */
  int setElement (ACEXML_Char const *namespaceURI,
                  ACEXML_Char const *localName,
                  ACEXML_Char const *qName ACEXML_ENV_ARG_DECL) override;

  /**
   * Acquire an Attribute_Builder.
   */
  ACEXML_Attribute_Def_Builder *getAttribute_Def_Builder () override;

  /**
   * Add a definition for one attribute.
   */
  int insertAttribute (ACEXML_Attribute_Def_Builder *def ACEXML_ENV_ARG_DECL) override;


  /**
   * Dump the content of the attribute definition.
   */
  void dump () override;
protected:
  /// The name of the element type these attributes applied.
  ACEXML_String element_name_;

  /// Collection of attributes.
  ACEXML_ATT_MAP attributes_;
};


#include /**/ "ace/post.h"

#endif /* _ACEXML_DEBUG_ATTRIBUTES_BUILDER_H_ */
